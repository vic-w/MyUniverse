using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace text2ImageGenerator
{
    struct TextStyle
    {
        public FontFamily fontFamily;
        public float fontSize;
        public FontStyle fontStyle;
        public Color color;
        public Point atPoint;

        public string overAllAlignment;
        public StringAlignment stringAlignment;
        public StringAlignment lineAlignment;
     }
}
