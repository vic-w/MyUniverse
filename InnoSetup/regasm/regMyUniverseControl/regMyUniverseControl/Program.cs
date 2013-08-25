using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using System.Diagnostics;

namespace regMyUniverseControl
{
    class Program
    {
        static void Main(string[] args)
        {
            string curDir = Path.GetDirectoryName(Application.ExecutablePath);
            string dllToRegister = Path.Combine(curDir, "MyUniverseControl.dll");
            if (!File.Exists(dllToRegister))
            {
                MessageBox.Show("Can't find " + dllToRegister);
                return;
            }

            string dirNETv2 = Path.Combine(Environment.GetEnvironmentVariable("windir"), @"Microsoft.NET\Framework\v2.0.50727");
            if (!Directory.Exists(dirNETv2))
            {
                MessageBox.Show("Can't find .NET Framework 2.0"); //should not happen.
                return;
            }

            try
            {
                //register dll
                string command = Path.Combine(dirNETv2, "regasm.exe");
                using (Process p = Process.Start(command, "\"" + dllToRegister + "\""))
                {
                    p.WaitForExit();
                }

                //install it to GAC
                //command = Path.Combine(dirNETv2, "installutil.exe");
                //using (Process p = Process.Start(command, "\"" + dllToRegister + "\""))
                //{
                //    p.WaitForExit();
                //}
            }
            catch (Exception e)
            {
                MessageBox.Show("Error when registering MyUniverseControl.dll: " + e.Message);
            }
        }
    }
}
