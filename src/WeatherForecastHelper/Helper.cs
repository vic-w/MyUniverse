using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.ServiceModel;
using System.Xml;
using System.IO;

namespace WeatherForecastHelper
{
    public class Helper
    {
        static WeatherWebServiceHelper.WeatherWebServiceSoapClient client;
        static GlobalWeatherHelper.GlobalWeatherSoapClient globalClient;
        static Helper()
        {
            client = new WeatherWebServiceHelper.WeatherWebServiceSoapClient("WeatherWebServiceSoap12"); //国内的WebService, 要收费，不用
            globalClient = new GlobalWeatherHelper.GlobalWeatherSoapClient("GlobalWeatherSoap"); //国外的WebService, 速度较慢，需要将英文天气翻译成汉语
        }

        const string citiesXMLFile = "cities.xml";

        /// <summary>
        /// 返回值： 
        /// -1： Xml文件不存在
        /// -2： 网络连接错误
        /// -3： WebService配置错误
        /// -4： 其他错误
        /// </summary>
        /// <returns></returns>
        static int Main(string[] args)
        {
            foreach (string arg in args)
                System.Diagnostics.Trace.WriteLine("kennnyzx: " + arg);

            System.Diagnostics.Stopwatch watch = new System.Diagnostics.Stopwatch();
            watch.Start();
            try
            {
                if (!File.Exists(citiesXMLFile))
                    return -1; //Xml文件不存在

                XmlDocument xDoc = new XmlDocument();
                xDoc.Load(citiesXMLFile);
                var nav = xDoc.CreateNavigator();
                //更新天气
                var itor = nav.Select("cities/city");
                while (itor.MoveNext())
                {
                    //string[] vals = new string[0]; //国内的WebService
                    string val = string.Empty;
                    string temprature = string.Empty;
                    string weatherCondition = string.Empty;
                    string cityName = string.Empty;
                    string countryName = string.Empty;
                    var nav2 = itor.Current.Clone();
                    if (nav2.MoveToFirstAttribute())
                    {
                        while (nav2.MoveToNextAttribute())
                        {
                            //if (nav2.Name == "displayname") //国内的WebService
                            //{
                            //    cityName = nav2.Value;
                            //    vals = client.getWeatherbyCityName(cityName);
                            //}
                            if (nav2.Name == "name")
                            {
                                cityName = nav2.Value;
                            }
                            if (nav2.Name == "country")
                            {
                                countryName = nav2.Value;
                                if (countryName != "USA")
                                {
                                    if (cityName != "Hong Kong")
                                        val = globalClient.GetWeather(cityName, countryName);
                                    else
                                        val = globalClient.GetWeather("", cityName);
                                }
                                else
                                    val = globalClient.GetWeather(cityName, "");
                                
                                //以下是返回的信息的示例
                                /*<?xml version=\"1.0\" encoding=\"utf-16\"?>
                                <CurrentWeather>
                                  <Location>Beijing, China (ZBAA) 39-56N 116-17E 55M</Location>
                                  <Time>Oct 05, 2013 - 12:30 AM EDT / 2013.10.05 0430 UTC</Time>
                                  <Wind> from the ESE (110 degrees) at 4 MPH (4 KT) (direction variable):0</Wind>
                                  <Visibility> 1 mile(s):0</Visibility>
                                  <Temperature> 71 F (22 C)</Temperature>
                                  <DewPoint> 51 F (11 C)</DewPoint>
                                  <RelativeHumidity> 49%</RelativeHumidity>
                                  <Pressure> 30.12 in. Hg (1020 hPa)</Pressure>
                                  <Status>Success</Status>
                                </CurrentWeather>
                                 */

                                if (val.Contains("<Status>Success</Status>"))
                                {
                                    using (XmlTextReader xtr = new XmlTextReader(new StringReader(val)))
                                    {
                                        if (xtr.ReadToFollowing("Visibility"))
                                        {
                                            weatherCondition = "可见度：" + xtr.ReadElementContentAsString();
                                        }
                                        if (xtr.ReadToFollowing("Temperature"))
                                        {
                                            temprature = "温度：" + xtr.ReadElementContentAsString();
                                        }
                                    }
                                }
                            }
                            if (nav2.Name == "temprature")
                            {
                                //if (vals.Length == 23) ////国内的WebService 返回一个长度为23的字符串数组
                                //    nav2.SetValue(vals[5]);
                                if (temprature != string.Empty)
                                {
                                    nav2.SetValue(temprature);
                                }
                                
                            }
                            if (nav2.Name == "weatherCondition")
                            {
                                //if (vals.Length == 23) ////国内的WebService 返回一个长度为23的字符串数组
                                //    nav2.SetValue(vals[6]);

                                if (weatherCondition != string.Empty)
                                {
                                    nav2.SetValue(weatherCondition);
                                }
                            }
                        }
                    }
                }

                xDoc.Save(citiesXMLFile);
                System.Diagnostics.Trace.WriteLine("Execution duration: " + watch.ElapsedMilliseconds.ToString());
                return 0;
            }
            catch (EndpointNotFoundException e)
            {
                if (e.InnerException is System.Net.WebException)
                {
                    return -2; //网络连接错误
                }
                else
                {
                    return -3; //WebService配置错误
                }
            }
            catch (Exception e)
            {
                return -4; //其他错误
            }
            finally
            {

            }
        }
    }
}
