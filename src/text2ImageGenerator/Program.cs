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
                atPoint = new Point(10, 10),
                color = Color.FromKnownColor(KnownColor.Red),
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
                fontSize = 20.0f,
                fontStyle = FontStyle.Regular,
                overAllAlignment = "BottomRight",
                lineAlignment = StringAlignment.Far,
                stringAlignment = StringAlignment.Far
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
// TESTROUTINE
            //SaveAsImage(TextStyleCollection.textStyles["description"], @"image\cities\beijing.jpg", "北京当前气温为22摄氏度北京当前气温为22摄氏度");
            //System.Diagnostics.Process.Start("temp.jpg"); //open the image with photo viewer
            //return 0;
// TESTROUTINE end

            System.Diagnostics.Stopwatch watch = System.Diagnostics.Stopwatch.StartNew();
            foreach (string arg in args)
                System.Diagnostics.Trace.WriteLine("kennnyzx: " + arg);

            if (args.Length == 0)
                return -1;

            else if (args.Length == 1)
            {
                //TODO: generate a default image
                string myString = args[0];
                return -1;
            }
            else if (args.Length == 2)
            {
                string bgImageFile = args[0];
                if (File.Exists(bgImageFile))
                {
                    string myString = args[1];
                    SaveAsImage(TextStyleCollection.textStyles["default"], bgImageFile, myString);
                    return 0;
                }
                else
                    return -1;
            }
            else
            {
                string bgImageFile = args[0];
                if (File.Exists(bgImageFile))
                {
                    StringBuilder sbMyString = new StringBuilder();
                    for (int index = 2; index < args.Length; index++)
                        sbMyString.Append(args[index]);
                    string myString = sbMyString.ToString();

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
                        default:
                             SaveAsImage(TextStyleCollection.textStyles["default"], bgImageFile, myString);
                             break;
                    }
                    System.Diagnostics.Trace.WriteLine(watch.ElapsedMilliseconds);
                    return 0;
                }
                else
                {
                    System.Diagnostics.Trace.WriteLine(watch.ElapsedMilliseconds);
                    return -1;
                }
            }
        }

        static void SaveAsImage(TextStyle textStyle, string bgImageFileName, string myString)
        {
            using (System.Drawing.Image bitmap = (System.Drawing.Image)Bitmap.FromFile(bgImageFileName))
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
                using (FileStream fs = new FileStream("temp.jpg", FileMode.Create, FileAccess.Write))
                {
                    bitmap.Save(fs, System.Drawing.Imaging.ImageFormat.Jpeg);
                    fs.Dispose();
                }
            }
        }
    }
}
