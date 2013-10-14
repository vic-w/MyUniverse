using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using text2ImageGenerator.Properties;

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
                atPoint = new Point(10, 10),
                color = Color.FromKnownColor(KnownColor.Red),
                fontFamily = new FontFamily("Arial"),
                fontSize = 32.0f,
                fontStyle = FontStyle.Regular,
                overAllAlignment = "BottomRight",
                lineAlignment = StringAlignment.Far,
                stringAlignment = StringAlignment.Far
            });
            textStyles.Add("weather", new TextStyle()
            {
                atPoint = new Point(10, 10),
                color = Color.FromKnownColor(KnownColor.White),
                fontFamily = new FontFamily("Arial"),
                fontSize = 32.0f,
                fontStyle = FontStyle.Regular,
                overAllAlignment = "BottomRight",
                lineAlignment = StringAlignment.Far,
                stringAlignment = StringAlignment.Far
            });
            textStyles.Add("cityname", new TextStyle()
            {
                atPoint = new Point(10, 10),
                color = Color.FromArgb(255, 255, 255, 255),
                fontFamily = new FontFamily("Arial"),
                fontSize = 40.0f,
                fontStyle = FontStyle.Regular,
                overAllAlignment = "TopLeft",
                lineAlignment = StringAlignment.Near,
                stringAlignment = StringAlignment.Near
            });
            textStyles.Add("localtime", new TextStyle()
            { 
                atPoint = new Point(10, 10),
                color = Color.FromArgb(255, 255, 255, 255),
                fontFamily = new FontFamily("Arial"),
                fontSize = 24.0f,
                overAllAlignment = "TopRight",
                fontStyle = FontStyle.Regular,
                lineAlignment = StringAlignment.Near,
                stringAlignment = StringAlignment.Far
            });
            textStyles.Add("description", new TextStyle()
            {
                atPoint = new Point(10, 10),
                color = Color.FromArgb(255, 255, 255, 255),
                fontFamily = new FontFamily("Arial"),
                fontSize = 30.0f,
                fontStyle = FontStyle.Regular,
                overAllAlignment = "TopLeft",
                lineAlignment = StringAlignment.Near,
                stringAlignment = StringAlignment.Near
            });
        }
    }

    class Program
    {
        /// <summary>
        /// Generate image from input text
        /// text2ImageGenerator "test to ri"
        /// </summary>
        /// <param name="args"></param>
        static int Main(string[] args)
        {
// TESTROUTINE
            //SaveAsImage(TextStyleCollection.textStyles["description"], @"image\cities\beijing.jpg", "北京当前气温为22摄氏度北京当前气温为22摄氏度");
            //System.Diagnostics.Process.Start("temp.jpg"); //open the image with photo viewer

            //Font font = new Font(TextStyleCollection.textStyles["description"].fontFamily, TextStyleCollection.textStyles["description"].fontSize,
            //    TextStyleCollection.textStyles["description"].fontStyle, GraphicsUnit.Pixel);
            //SizeF size = GetRequiredSize(font, "北京当前气温为22摄氏度$北京当前气温为22摄氏度");
            //return 0;
// TESTROUTINE end

            System.Diagnostics.Stopwatch watch = System.Diagnostics.Stopwatch.StartNew();
            foreach (string arg in args)
                System.Diagnostics.Trace.WriteLine("kennyzx: " + arg);

            int ret = -1;
            string myString = "";
            ImageFormat format = ImageFormat.Jpeg;

            if (args.Length == 1)
            {
                //Generate a default image, which is transparent
                myString = args[0].Replace("$", Environment.NewLine);
                format = ImageFormat.Png;
                SizeF size = GetRequiredSize(GetFontFromTextStyle(TextStyleCollection.textStyles["default"]), myString);
                using (Bitmap bitmap = new Bitmap((int)size.Width, (int)size.Height))
                {
                    SaveAsImage(TextStyleCollection.textStyles["description"], bitmap, format, myString);
                }
                ret = 0;
            }
            else if (args.Length == 2)
            {
                string bgImageFile = args[0];
                if (File.Exists(bgImageFile))
                {
                    myString = args[1].Replace("$", Environment.NewLine);                    
                    if (Path.GetExtension(bgImageFile).ToUpper() == ".PNG")
                        format = ImageFormat.Png;

                    using (Bitmap bitmap = (Bitmap)Bitmap.FromFile(bgImageFile))
                    {
                        SaveAsImage(TextStyleCollection.textStyles["default"], bitmap, format, myString);
                    }
                    ret = 0;
                }
            }
            else
            {
                string bgImageFile = args[0];
                if (File.Exists(bgImageFile))
                {
                    if (Path.GetExtension(bgImageFile).ToUpper() == ".PNG")
                        format = ImageFormat.Png;

                    StringBuilder sbMyString = new StringBuilder();
                    for (int index = 2; index < args.Length; index++)
                        sbMyString.Append(args[index]);

                    myString = sbMyString.ToString().Replace("$", Environment.NewLine);
                    using (Bitmap bitmap = (Bitmap)Bitmap.FromFile(bgImageFile))
                    {
                        switch (args[1])
                        {
                            case "1":
                                SaveAsImage(TextStyleCollection.textStyles["cityname"], bitmap, format, myString);
                                break;
                            case "2":
                                SaveAsImage(TextStyleCollection.textStyles["localtime"], bitmap, format, myString);
                                break;
                            case "3":
                                SaveAsImage(TextStyleCollection.textStyles["default"], bitmap, format, myString);
                                break;
                            case "4":
                                SaveAsImage(TextStyleCollection.textStyles["default"], bitmap, format, myString);
                                break;
                            case "5":
                                SaveAsImage(TextStyleCollection.textStyles["weather"], bitmap, format, myString);
                                break;
                            default:
                                SaveAsImage(TextStyleCollection.textStyles["default"], bitmap, format, myString);
                                break;
                        }
                    }
                    ret = 0;
                }
            }
            System.Diagnostics.Trace.WriteLine(watch.ElapsedMilliseconds);
            return ret;
        }

        private static SizeF GetRequiredSize(Font font, string myString)
        {
            if (string.IsNullOrEmpty(myString))
                return new SizeF(10, 10);

            using (Graphics g = Graphics.FromImage(Resources.textbg))
            {
                SizeF size = g.MeasureString(myString, font); 
                return size;
            }
        }

        static void SaveAsImage(TextStyle textStyle, Bitmap bitmap, ImageFormat format, string myString)
        {
            Font font = new Font(textStyle.fontFamily, textStyle.fontSize, textStyle.fontStyle, GraphicsUnit.Pixel);
            Graphics graphics = Graphics.FromImage(bitmap);
            SizeF sizeRequired = graphics.MeasureString(myString, font, bitmap.Width - 2 * textStyle.atPoint.X);
            if (sizeRequired.Height > (bitmap.Height / 2 - textStyle.atPoint.Y))
            {
                //more space is required.
            }

            Point atpoint;
            if (textStyle.overAllAlignment == "TopLeft")
                atpoint = new Point(textStyle.atPoint.X, textStyle.atPoint.Y);
            else if (textStyle.overAllAlignment == "TopRight")
                atpoint = new Point(bitmap.Width - textStyle.atPoint.X, textStyle.atPoint.Y);
            else if (textStyle.overAllAlignment == "BottomLeft")
                atpoint = new Point(textStyle.atPoint.X, bitmap.Height - textStyle.atPoint.Y);
            else
                atpoint = new Point(bitmap.Width - textStyle.atPoint.X, bitmap.Height - textStyle.atPoint.Y);

            SolidBrush brush = new SolidBrush(textStyle.color);

            StringFormat sf = new StringFormat();
            sf.Alignment = textStyle.stringAlignment;
            sf.LineAlignment = textStyle.lineAlignment;

            graphics.DrawString(myString, font, brush, atpoint, sf);
            graphics.Dispose();

            if (format == ImageFormat.Png)
            {
                using (FileStream fs = new FileStream("temp.png", FileMode.Create, FileAccess.Write))
                {
                    bitmap.Save(fs, ImageFormat.Png);
                }
            }
            else
            {
                using (FileStream fs = new FileStream("temp.jpg", FileMode.Create, FileAccess.Write))
                {
                    bitmap.Save(fs, ImageFormat.Jpeg);
                }
            }
        }

        static Font GetFontFromTextStyle(TextStyle textStyle)
        {
            Font font = new Font(textStyle.fontFamily, textStyle.fontSize,
                    textStyle.fontStyle, GraphicsUnit.Pixel);
            return font;
        }
    }
}
