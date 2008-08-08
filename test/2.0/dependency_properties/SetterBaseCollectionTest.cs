using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Collections.Generic;

using dependency_properties;

namespace MoonTest.System.Windows
{
	[TestClass]
	public class SetterBaseCollectionTest
	{
		[TestMethod]
		public void Sealed ()
		{
			Style style = new Style (typeof (UIElement));
			SetterBaseCollection c = style.Setters;
			Setter s = new Setter (Canvas.LeftProperty, 0);

			c.Add (s);

			style.Seal ();

			Assert.Throws (delegate { c.Add (new Setter (Canvas.TopProperty, 0)); }, typeof (Exception));
			Assert.Throws (delegate { c.Insert (0, new Setter (Canvas.TopProperty, 0)); }, typeof (Exception));

			/*Assert.Throws (delegate {*/ c.Remove (s);/* }, typeof (Exception));*/

			Assert.AreEqual (0, c.Count);

			// need to reinitialize things here since the
			// Remove above actually succeeded.
			style = new Style (typeof (UIElement));
			c = style.Setters;
			s = new Setter (Canvas.LeftProperty, 0);

			c.Add (s);

			style.Seal ();

			// lame, this should raise an exception too
			/*Assert.Throws (delegate {*/ c.RemoveAt (0);/* }, typeof (Exception));*/

			Assert.AreEqual (0, c.Count);

			// need to reinitialize things here since the
			// RemoveAt above actually succeeded.
			style = new Style (typeof (UIElement));
			c = style.Setters;
			s = new Setter (Canvas.LeftProperty, 0);

			c.Add (s);

			style.Seal ();

			Assert.Throws (delegate { c[0] = new Setter (Canvas.TopProperty, 0); }, typeof (Exception));
		}

		[TestMethod]
		public void SealedChildren ()
		{
			Style style = new Style (typeof (UIElement));
			SetterBaseCollection c = style.Setters;
			Setter s = new Setter (Canvas.LeftProperty, 0);

			c.Add (s);

			style.Seal ();

			Assert.Throws (delegate { s.Property = Canvas.TopProperty; }, typeof (UnauthorizedAccessException));
			Assert.Throws (delegate { s.Value = 10; }, typeof (UnauthorizedAccessException));
		}
	}
}