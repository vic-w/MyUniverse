using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Runtime.InteropServices;

namespace MyUniverseControl
{
    public struct GlbCommand
    {
        int command;
        int ctrl1_ret0;
        int iParam1;
        int iParam2;
        double fParam1;
        double fParam2;
        char[] cParam1;
        char[] cParam2;
    }
    public class MyUniverseControl
    {
        public void Play()
        {
            //MessageBox.Show("Play");

            unsafe
            {
                IntPtr ptr = Marshal.AllocHGlobal(1056);
                Marshal.WriteInt32(ptr, 1);
                Marshal.WriteInt32(ptr, 4, 1);
                byte[] buffer = new byte[1056];
                Marshal.Copy(ptr, buffer, 0, 1056);
                UdpSend("127.0.0.1", 9160, buffer);
                Marshal.FreeHGlobal(ptr);
            }

        }
        public void Pause()
        {
            //MessageBox.Show("Pause");
            unsafe
            {
                IntPtr ptr = Marshal.AllocHGlobal(1056);
                Marshal.WriteInt32(ptr, 2);
                Marshal.WriteInt32(ptr, 4, 1);
                byte[] buffer = new byte[1056];
                Marshal.Copy(ptr, buffer, 0, 1056);
                UdpSend("127.0.0.1", 9160, buffer);
                Marshal.FreeHGlobal(ptr);
            }
        }
        public unsafe void UdpSend(string sIP, int iPort, byte[] Buffer)
        {
            byte[] data = new byte[1024];
            //string input, stringData;

            //设置服务IP，设置TCP端口号
            IPEndPoint ip = new IPEndPoint(IPAddress.Parse(sIP), iPort);

            //定义网络类型，数据连接类型和网络协议UDP
            Socket server = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            //string welcome = "hello";
            //data = Encoding.ASCII.GetBytes(welcome);

            server.SendTo(Buffer, 1056, SocketFlags.None, ip);

            //IPEndPoint sender = new IPEndPoint(IPAddress.Any, 0);
            //EndPoint Remote = (EndPoint)sender;

            //data = new byte[1024];
            ////对于不存在的IP地址，加入此行代码后，可以在指定时间内解除阻塞模式限制
            //int recv = server.ReceiveFrom(data, ref Remote);
            //Console.WriteLine("Message received from {0}: ", Remote.ToString());
            //Console.WriteLine(Encoding.ASCII.GetString(data, 0, recv));
            //while (true)
            //{
            //    input = Console.ReadLine();
            //    if (input == "exit")
            //        break;
            //    server.SendTo(Encoding.ASCII.GetBytes(input), Remote);
            //    data = new byte[1024];
            //    recv = server.ReceiveFrom(data, ref Remote);
            //    stringData = Encoding.ASCII.GetString(data, 0, recv);
            //    Console.WriteLine(stringData);
            //}
            //Console.WriteLine("Stopping Client.");
            //server.Close();            
        }
    }
}
