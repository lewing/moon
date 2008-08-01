﻿// Copyright © Microsoft Corporation. 
// This source is subject to the Microsoft Source License for Silverlight Controls (March 2008 Release).
// Please see http://go.microsoft.com/fwlink/?LinkID=111693 for details.
// All other rights reserved. 

using System;
using System.Collections.Generic; 
using System.ComponentModel; 
using System.Windows.Controlsb1;
using System.Windows;
 
namespace System.Windowsb1
{
    /// <summary> 
    /// Converts instances of the String type to FontWeight instances.
    /// </summary>
    public sealed partial class FontWeightConverter : TypeConverter 
    { 
        /// <summary>
        /// FontWeight known values. 
        /// </summary>
        private static Dictionary<string, FontWeight> KnownValues =
            new Dictionary<string, FontWeight>(10, StringComparer.OrdinalIgnoreCase) 
            {
                { "Black", FontWeights.Black },
                { "Bold", FontWeights.Bold }, 
                { "ExtraBlack", FontWeights.ExtraBlack }, 
                { "ExtraBold", FontWeights.ExtraBold },
                { "ExtraLight", FontWeights.ExtraLight }, 
                { "Light", FontWeights.Light },
                { "Medium", FontWeights.Medium },
                { "Normal", FontWeights.Normal }, 
                { "SemiBold", FontWeights.SemiBold },
                { "Thin", FontWeights.Thin }
            }; 
 
        /// <summary>
        /// Initializes a new instance of the FontWeightConverter class. 
        /// </summary>
        public FontWeightConverter()
        { 
        }

        /// <summary> 
        /// Returns a value that indicates whether this converter can convert an 
        /// object of the given type to an instance of FontWeight.
        /// </summary> 
        /// <param name="sourceType">
        /// The type of the source that is being evaluated for conversion.
        /// </param> 
        /// <returns>
        /// true if the converter can convert the provided type to an instance
        /// of FontWeight; otherwise, false. 
        /// </returns> 
        public override bool CanConvertFrom(Type sourceType)
        { 
            return TypeConverters.CanConvertFrom<FontWeight>(sourceType);
        }
 
        /// <summary>
        /// Attempts to convert a specified object to an instance of FontWeight.
        /// </summary> 
        /// <param name="value">The object being converted.</param> 
        /// <returns>
        /// The instance of FontWeight created from the converted object. 
        /// </returns>
        public override object ConvertFrom(object value)
        { 
            return TypeConverters.ConvertFrom<FontWeight>(this, value);
        }
 
        /// <summary> 
        /// Attempts to convert a specified String to an instance of FontWeight.
        /// </summary> 
        /// <param name="text">
        /// The String to be converted into the FontWeight object.
        /// </param> 
        /// <returns>
        /// The instance of FontWeight created from the converted text.
        /// </returns> 
        public override object ConvertFromString(string text) 
        {
            return TypeConverters.ConvertFromString(text, KnownValues); 
        }
    }
} 
