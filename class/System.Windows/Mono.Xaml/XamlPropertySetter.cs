//
// XamlElement.cs
//
// Contact:
//   Moonlight List (moonlight-list@lists.ximian.com)
//
// Copyright 2007 Novell, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

using Mono;

using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Linq;
using System.Reflection;
using System.Collections;
using System.ComponentModel;
using System.Collections.Generic;

using System.Windows;
using System.Windows.Data;
using System.Windows.Markup;
using System.Windows.Controls;

namespace Mono.Xaml {

	internal abstract class XamlPropertySetter {

		protected XamlPropertySetter (XamlObjectElement element, string name, TypeConverter converter)
		{
			Element = element;
			Name = name;
			Converter = converter;
		}

		public XamlParser Parser {
			get { return Element.Parser; }
		}

		public XamlObjectElement Element {
			get;
			private set;
		}

		public string Name {
			get;
			private set;
		}

		public abstract Type Type {
			get;
		}

		public abstract Type DeclaringType {
			get;
		}

		public TypeConverter Converter {
			get;
			private set;
		}

		public void SetValue (object value)
		{
			SetValue (null, value);
		}

		public object ConvertValue (Type type, object value)
		{
			if (value == null)
				return null;

			Type valueType = value.GetType ();
			if (valueType == type)
				return value;

			TypeConverter converter = Converter;
			if (converter == null) {
				try {
					converter = new XamlTypeConverter (Parser, Element, Name, type);
				} catch {
					converter = null;
				}
			}

			if (converter != null && converter.CanConvertFrom (valueType))
				return converter.ConvertFrom (value);

			try {
				if (!valueType.IsSubclassOf (type))
					value = Convert.ChangeType (value, type, System.Globalization.CultureInfo.CurrentCulture);
			} catch {
			}
			
			// This will just let things fail
			return value;
		}

		/// Find the dependency property that corresponds to this
		/// property. Technically a property does not need a
		/// corresponding 
		public DependencyProperty LookupDependencyProperty ()
		{
			Type type = DeclaringType;

			Types.Ensure (type);

			Kind kind = Deployment.Current.Types.TypeToNativeKind (type);
			if (kind == Kind.INVALID)
				return null;

			try {
				return DependencyProperty.Lookup (kind, Name);
			} catch {
				return null;
			}
		}

		public abstract void SetValue (XamlObjectElement obj, object value);
	}

	internal class XamlReflectionPropertySetter : XamlPropertySetter {

		private object target;
		private PropertyInfo prop;

		public XamlReflectionPropertySetter (XamlObjectElement element, object target, PropertyInfo prop) : base (element, prop.Name, Helper.GetConverterFor (prop, prop.PropertyType))
		{
			this.target = target;
			this.prop = prop;
		}

		public override Type Type {
			get { return prop.PropertyType; }
		}

		public override Type DeclaringType {
			get { return prop.DeclaringType; }
		}

		public PropertyInfo PropertyInfo {
			get { return prop; }
		}

		public object GetValue ()
		{
			return prop.GetValue (target, null);
		}

		public override void SetValue (XamlObjectElement obj, object value)
		{
			// We do this first to cover the case where you are setting a list to a list or
			// a resource dictionary to a resource dictionary, binding to a binding, ect
			// as opposed to adding items to the list or dictionary.
			if (Type.IsAssignableFrom (value.GetType ())) {
				prop.SetValue (target, ConvertValue (Type, value), null);
				return;
			}

			{
				Binding binding = value as Binding;
				if (binding != null) {
					SetBinding (binding);
					return;
				}
			}

			{
				TemplateBindingExpression tb = value as TemplateBindingExpression;
				if (tb != null) {
					SetTemplateBinding (tb);
					return;
				}
			}

			if (typeof (IList).IsAssignableFrom (Type)) {
				AddToCollection (obj, value);
				return;
			}

			if (typeof (IDictionary).IsAssignableFrom (Type)) {
				AddToDictionary (obj, value);
				return;
			}

			throw Parser.ParseException ("Unable to set property {0} to value {1}.", Name, value);
		}

		private void SetBinding (Binding binding)
		{
			DependencyProperty prop = LookupDependencyProperty ();

			if (prop == null)
				throw Parser.ParseException ("Invalid Binding, can not find DependencyProperty {0}.", Name);

			DependencyObject dob = target as DependencyObject;

			if (dob == null)
				throw Parser.ParseException ("Bindings can not be used on non DependencyObject types.");

			BindingOperations.SetBinding (dob, prop, binding);
		}

		private void SetTemplateBinding (TemplateBindingExpression tb)
		{
			DependencyObject dob = target as DependencyObject;
			FrameworkElement fwe = target as FrameworkElement;

			if (dob == null)
				throw Parser.ParseException ("Invalid TemplateBinding, expressions must be bound to DependendyObjects.");

			// Applying a {TemplateBinding} to a DO which is not a FrameworkElement should silently discard the binding.
			if (fwe == null)
				return;

			if (Parser.Context == null || Parser.Context.Template == null)
				throw Parser.ParseException ("Invalid TemplateBinding, expressions can not be used outside of FrameworkTemplate.");

			FrameworkElement source = Parser.Context.TemplateBindingSource;
			if (source == null) 
				throw Parser.ParseException ("Invalid TemplateBinding, expression can not be used outside of a FrameworkTemplate.");

			DependencyProperty source_prop = DependencyProperty.Lookup (source.GetKind(), tb.SourcePropertyName);
			if (source_prop == null)
				throw Parser.ParseException ("Invalid TemplateBinding, property {0} could not be found.", tb.SourcePropertyName);

			DependencyProperty prop = LookupDependencyProperty ();
			if (prop == null)
				throw Parser.ParseException ("Invalid TemplateBinding, property {0} could not be found.", Name);

			tb.TargetProperty = prop;
			tb.SourceProperty = source_prop;

			fwe.SetTemplateBinding (prop, tb);
		}

		private void AddToCollection (XamlObjectElement obj, object value)
		{
			IList list = prop.GetValue (target, null) as IList;
			if (list == null) {
				throw Parser.ParseException ("Collection property in non collection type.");
			}

			list.Add (value);
		}

		private void AddToDictionary (XamlObjectElement obj, object value)
		{
			IDictionary rd = prop.GetValue (target, null) as IDictionary;
			if (rd == null)
				throw Parser.ParseException ("Collection property in non collection type.");

			rd.Add (obj.X_Key, value);
		}
	}

	internal class XamlAttachedPropertySetter : XamlPropertySetter {

		private MethodInfo getter;
		private MethodInfo setter;

		public XamlAttachedPropertySetter (XamlObjectElement element, string name, MethodInfo getter, MethodInfo setter) : base (element, name, Helper.GetConverterFor (setter, getter.ReturnType))
		{
			this.getter = getter;
			this.setter = setter;
		}

		public override Type Type {
			get { return getter.ReturnType; }
		}

		public override Type DeclaringType {
			get { return getter.DeclaringType; }
		}

		public override void SetValue (XamlObjectElement obj, object value)
		{
			setter.Invoke (null, new object [] { Element.Object, value });
		}
	}
}


