using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MyUniverseControl;

namespace MyUniverseControlTest
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        public MyUniverseControl.MyUniverseControl ctrl = new MyUniverseControl.MyUniverseControl();

        private void Play_Click(object sender, EventArgs e)
        {
            
            //MessageBox.Show("Clicked Play");
            ctrl.Play();
        }

        private void Pause_Click(object sender, EventArgs e)
        {
            //MessageBox.Show("Clicked Pause");
            ctrl.Pause();
        }

        private void Test_Click(object sender, EventArgs e)
        {
            ctrl.Test();
        }
    }
}
