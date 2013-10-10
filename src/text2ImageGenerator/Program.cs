using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.IO;

namespace text2ImageGenerator
{
    static class TextStyleCollection
    {
        public static Dictionary<string, TextStyle> textStyles;
        static TextStyleCollection()
        {
            textStyles = new Dictionary<string, TextStyle>();
            textStyles.Add("default", new TextStyle()
            {
                atPoint = new Point(150, 50),
                color = Color.FromKnownColor(KnownColor.Red),
                fontFamily = new FontFamily("Arial"),
                fontSize = 32.0f,
                fontStyle = FontStyle.Regular,
                overAllAlignment = "BottomRight",
                lineAlignment = StringAlignment.Center,
                stringAlignment = StringAlignment.Center
            });
            textStyles.Add("cityname", new TextStyle()
            {
                atPoint = new Point(50, 50),
                color = Color.FromArgb(255, 255, 255, 255),
                fontFamily = new FontFamily("Arial"),
                fontSize = 40.0f,
                fontStyle = FontStyle.Regular,
                overAllAlignment = "TopLeft",
                lineAlignment = StringAlignment.Far,
                stringAlignment = StringAlignment.Far
            });
            textStyles.Add("localtime", new TextStyle()
            { 
                atPoint = new Point(20, 20),
                color = Color.FromArgb(255, 255, 255, 255),
                fontFamily = new FontFamily("Arial"),
                fontSize = 24.0f,
                overAllAlignment = "TopRight",
                fontStyle = FontStyle.Regular,
                lineAlignment = StringAlignment.Near,
                stringAlignment = StringAlignment.Near
            });
        }
    }

    class Program
    {
        /// <summary>
        /// Generate image from input text
        /// </summary>
        /// <param name="args"></param>
        static int Main(string[] args)
        {
            foreach (string arg in args)
                System.Diagnostics.Trace.WriteLine("kennnyzx: " + arg);

            if (args.Length != 1 && args.Length != 3)
                return -1;

            else if (args.Length == 1)
            {
                //TODO: generate a default image
                string myString = args[0];
                return -1;
            }
            else if (args.Length == 3)
            {
                string bgImageFile = args[0];
                if (File.Exists(bgImageFile))
                {
                    string myString = args[2];
                    switch (args[1])
                    {
                        case "1":
                            SaveAsImage(TextStyleCollection.textStyles["cityname"], bgImageFile, myString);
                            break;
                        case "2":
                            SaveAsImage(TextStyleCollection.textStyles["localtime"], bgImageFile, myString);
                            break;
                        case "3":
                            SaveAsImage(TextStyleCollection.textStyles["default"], bgImageFile, myString);
                            break;
                        case "4":
                            SaveAsImage(TextStyleCollection.textStyles["default"], bgImageFile, myString);
                            break;
                        case "5":
                            SaveAsImage(TextStyleCollection.textStyles["default"], bgImageFile, myString);
                            break;
                    }

                    return 0;
                }
                else
                    return -1;
            }
            else
                return -1;
        }

        static void SaveAsImage(TextStyle textStyle, string bgImageFileName, string myString)
        {
            using (System.Drawing.Image bitmap = (System.Drawing.Image)Bitmap.FromFile(bgImageFileName))
            {
                Font font = new Font(textStyle.fontFamily, textStyle.fontSize, textStyle.fontStyle, GraphicsUnit.Pixel);

                Point atpoint;
                atpoint = new Point(textStyle.atPoint.X, textStyle.atPoint.Y);
                //if (textStyle.overAllAlignment == "TopLeft")
                //    atpoint = new Point(textStyle.atPoint.X, textStyle.atPoint.Y);
                //else if (textStyle.overAllAlignment == "TopRight")
                //    atpoint = new Point(bitmap.Width - textStyle.atPoint.X, textStyle.atPoint.Y);
                //else if (textStyle.overAllAlignment == "BottomLeft")
                //    atpoint = new Point(textStyle.atPoint.X, bitmap.Height - textStyle.atPoint.Y);
                //else 
                //    atpoint = new Point(bitmap.Width - textStyle.atPoint.X, bitmap.Height - textStyle.atPoint.Y);

                SolidBrush brush = new SolidBrush(textStyle.color);

                StringFormat sf = new StringFormat();
                sf.Alignment = textStyle.stringAlignment;
                sf.LineAlignment = textStyle.lineAlignment;

                Graphics graphics = Graphics.FromImage(bitmap);
                graphics.DrawString(myString, font, brush, atpoint, sf);
                graphics.Dispose();
                using (FileStream fs = new FileStream("temp.jpg", FileMode.Create, FileAccess.Write))
                {
                    bitmap.Save(fs, System.Drawing.Imaging.ImageFormat.Jpeg);
                    fs.Dispose();
                }
            }
        }
    }
}
