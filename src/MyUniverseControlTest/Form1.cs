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
            //ctrl.Test();
        }

        private void RotateClockwiseTrue_Click(object sender, EventArgs e)
        {
            ctrl.RotateClockwise = true;
        }

        private void RotateClockwiseFalse_Click(object sender, EventArgs e)
        {
            ctrl.RotateClockwise = false;
        }

        private void RotatingTrue_Click(object sender, EventArgs e)
        {
            ctrl.Rotating = true;
        }

        private void RotatingFalse_Click(object sender, EventArgs e)
        {
            ctrl.Rotating = false;
        }

        private void GetRotateRate_Click(object sender, EventArgs e)
        {
            double f = ctrl.RotationRate;
            MessageBox.Show(f.ToString());
        }

        private void SetRotateRate_Click(object sender, EventArgs e)
        {
            ctrl.RotationRate = double.Parse(this.TextBox_rotRate.Text);
        }

        private void GetPoleLatitude_Click(object sender, EventArgs e)
        {
            double l = ctrl.PoleLatitude;
            MessageBox.Show(l.ToString());
        }

        private void SetPoleLatitude_Click(object sender, EventArgs e)
        {
            ctrl.PoleLatitude = double.Parse(this.textBox_PoleLatitude.Text);
        }

        private void GetPoleLongitude_Click(object sender, EventArgs e)
        {
            double l = ctrl.PoleLongitude;
            MessageBox.Show(l.ToString());
        }

        private void SetPoleLongitude_Click(object sender, EventArgs e)
        {
            ctrl.PoleLongitude = double.Parse(this.textBox_PoleLongitude.Text);
        }

        private void GetChapterAndPageNames_Click(object sender, EventArgs e)
        {
            //string chapter_name="",page_name="";
            //ctrl.GetChapterAndPageNames(ref chapter_name, ref page_name);
            //string msg = chapter_name + " ";
            //msg += page_name;
            //MessageBox.Show(msg);
        }

        private void SetChapterAndPageNames_Click(object sender, EventArgs e)
        {
            ctrl.ChapterAndPageNames(textBox_chapter.Text, textBox_page.Text);
        }
    }
}
