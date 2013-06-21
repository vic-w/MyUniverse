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
    //public struct GlbCommand
    //{
    //    int command;
    //    int ctrl1_ret0;
    //    int iParam1;
    //    int iParam2;
    //    double fParam1;
    //    double fParam2;
    //    char[] cParam1;
    //    char[] cParam2;
    //}
    public class MyUniverseControl
    {
        public void Play()
        {
            //MessageBox.Show("Play");
            SendCmd(1, 0, 0, 0, 0, "", "");
        }
        public void Pause()
        {
            //MessageBox.Show("Pause");
            SendCmd(2, 0, 0, 0, 0, "", "");
        }
        public void Test()
        {
            //MessageBox.Show("Test");
            SendCmd(3, 2, 3, 4.0, 5.0, "6", "7测试");
            byte[] Buffer = new byte[1056];
            UdpRecv(9161, Buffer, 1056);
        }

        private static void WriteString(IntPtr basePtr, string value, int offset, int length)
        {
            int pos = 0;
            byte[] bytes = Encoding.Default.GetBytes(value);
            while (pos < length)
            {
                if (pos < bytes.Length)
                    Marshal.WriteByte(basePtr, offset, bytes[pos]);
                else
                    Marshal.WriteByte(basePtr, offset, 0);

                pos++;
                offset++;
            }
        } 
        public unsafe void SendCmd(
            int command,
            int iParam1, int iParam2,
            double fParam1, double fParam2,
            string cParam1, string cParam2)
        {
            IntPtr ptr = Marshal.AllocHGlobal(1056);
            Marshal.WriteInt32(ptr, 0, command);    //command
            Marshal.WriteInt32(ptr, 4, 1);          //ctrl1_ret0
            Marshal.WriteInt32(ptr, 8, iParam1);    //iParam1
            Marshal.WriteInt32(ptr, 12, iParam2);   //iParam2
            double* pfParam1 = (double*)(ptr + 16); //fParam1
            *pfParam1 = fParam1;
            double* pfParam2 = (double*)(ptr + 24); //fParam2
            *pfParam2 = fParam2;
            WriteString(ptr, cParam1, 32, 512);     //cParam1
            WriteString(ptr, cParam2, 544, 512);    //cParam2
            
            byte[] buffer = new byte[1056];
            Marshal.Copy(ptr, buffer, 0, 1056);
            UdpSend("127.0.0.1", 9160, buffer, 1056);
            Marshal.FreeHGlobal(ptr);
        }

        public unsafe void UdpSend(string sIP, int iPort, byte[] Buffer, int length)
        {
            //设置服务IP，设置TCP端口号
            IPEndPoint ip = new IPEndPoint(IPAddress.Parse(sIP), iPort);

            //定义网络类型，数据连接类型和网络协议UDP
            Socket server = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            server.SendTo(Buffer, length, SocketFlags.None, ip);
            server.Close();
        }

        public unsafe void UdpRecv(int iPort, byte[] Buffer, int length)
        {
            IPEndPoint MyIP = new IPEndPoint(IPAddress.Any, iPort);
            IPEndPoint SenderIP = new IPEndPoint(IPAddress.Any, 0);
            EndPoint Remote = (EndPoint)SenderIP;

            Socket server = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            server.Bind(MyIP);

            server.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReceiveTimeout, 1000);
            int recv = 0;

            try
            {
                recv = server.ReceiveFrom(Buffer, ref Remote);
            }
            catch (SocketException e)
            {
                if (e.SocketErrorCode == SocketError.TimedOut)
                {
                    MessageBox.Show("Time out");
                }
            }
            if (recv == 1056)
            {
                MessageBox.Show("1056 Received");
            }
            server.Close();            
        }
    }
}
