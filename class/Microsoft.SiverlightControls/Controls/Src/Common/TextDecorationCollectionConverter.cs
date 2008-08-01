﻿// Copyright © Microsoft Corporation. 
// This source is subject to the Microsoft Source License for Silverlight Controls (March 2008 Release).
// Please see http://go.microsoft.com/fwlink/?LinkID=111693 for details.
// All other rights reserved. 

using System;
using System.ComponentModel; 
using System.Collections.Generic; 
using System.Windows.Controlsb1;
 
namespace System.Windows
{
    /// <summary> 
    /// Converts instances of the String type to TextDecorationCollection
    /// instances.
    /// </summary> 
    public sealed partial class TextDecorationCollectionConverter : TypeConverter 
    {
        /// <summary> 
        /// TextDecorationCollection known values.
        /// </summary>
        private static Dictionary<string, TextDecorationCollection> KnownValues = 
            new Dictionary<string, TextDecorationCollection>(2, StringComparer.OrdinalIgnoreCase)
            {
                { string.Empty, null }, 
                { "Underline", TextDecorations.Underline } 
            };
 
        /// <summary>
        /// Initializes a new instance of the TextDecorationCollectionConverter
        /// class. 
        /// </summary>
        public TextDecorationCollectionConverter()
        { 
        } 

        /// <summary> 
        /// Returns a value that indicates whether this converter can convert an
        /// object of the given type to an instance of TextDecorationCollection.
        /// </summary> 
        /// <param name="sourceType">
        /// The type of the source that is being evaluated for conversion.
        /// </param> 
        /// <returns> 
        /// true if the converter can convert the provided type to an instance
        /// of TextDecorationCollection; otherwise, false. 
        /// </returns>
        public override bool CanConvertFrom(Type sourceType)
        { 
            return TypeConverters.CanConvertFrom<TextDecorationCollection>(sourceType);
        }
 
        /// <summary> 
        /// Attempts to convert a specified object to an instance of
        /// TextDecorationCollection. 
        /// </summary>
        /// <param name="value">The object being converted.</param>
        /// <returns> 
        /// The instance of TextDecorationCollection created from the converted
        /// object.
        /// </returns> 
        public override object ConvertFrom(object value) 
        {
            return TypeConverters.ConvertFrom<TextDecorationCollection>(this, value); 
        }

        /// <summary> 
        /// Attempts to convert a specified String to an instance of
        /// TextDecorationCollection.
        /// </summary> 
        /// <param name="text"> 
        /// The String to be converted into the TextDecorationCollection object.
        /// </param> 
        /// <returns>
        /// The instance of TextDecorationCollection created from the converted
        /// text. 
        /// </returns>
        public override object ConvertFromString(string text)
        { 
            if (text == null) 
            {
                return null; 
            }
            return TypeConverters.ConvertFromString(text, KnownValues);
        } 
    }
}
