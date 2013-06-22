namespace MyUniverseControlTest
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.Play = new System.Windows.Forms.Button();
            this.Pause = new System.Windows.Forms.Button();
            this.Test = new System.Windows.Forms.Button();
            this.RotateClockwiseTrue = new System.Windows.Forms.Button();
            this.RotateClockwiseFalse = new System.Windows.Forms.Button();
            this.RotatingTrue = new System.Windows.Forms.Button();
            this.RotatingFalse = new System.Windows.Forms.Button();
            this.GetRotateRate = new System.Windows.Forms.Button();
            this.SetRotateRate = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.GetPoleLatitude = new System.Windows.Forms.Button();
            this.SetPoleLatitude = new System.Windows.Forms.Button();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.GetPoleLongitude = new System.Windows.Forms.Button();
            this.SetPoleLongitude = new System.Windows.Forms.Button();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.ChapterAndPageNames = new System.Windows.Forms.Button();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // Play
            // 
            this.Play.Location = new System.Drawing.Point(24, 17);
            this.Play.Name = "Play";
            this.Play.Size = new System.Drawing.Size(75, 23);
            this.Play.TabIndex = 0;
            this.Play.Text = "Play";
            this.Play.UseVisualStyleBackColor = true;
            this.Play.Click += new System.EventHandler(this.Play_Click);
            // 
            // Pause
            // 
            this.Pause.Location = new System.Drawing.Point(24, 46);
            this.Pause.Name = "Pause";
            this.Pause.Size = new System.Drawing.Size(75, 23);
            this.Pause.TabIndex = 1;
            this.Pause.Text = "Pause";
            this.Pause.UseVisualStyleBackColor = true;
            this.Pause.Click += new System.EventHandler(this.Pause_Click);
            // 
            // Test
            // 
            this.Test.Location = new System.Drawing.Point(24, 75);
            this.Test.Name = "Test";
            this.Test.Size = new System.Drawing.Size(75, 21);
            this.Test.TabIndex = 2;
            this.Test.Text = "Test";
            this.Test.UseVisualStyleBackColor = true;
            this.Test.Click += new System.EventHandler(this.Test_Click);
            // 
            // RotateClockwiseTrue
            // 
            this.RotateClockwiseTrue.Location = new System.Drawing.Point(228, 17);
            this.RotateClockwiseTrue.Name = "RotateClockwiseTrue";
            this.RotateClockwiseTrue.Size = new System.Drawing.Size(148, 23);
            this.RotateClockwiseTrue.TabIndex = 3;
            this.RotateClockwiseTrue.Text = "RotateClockwiseTrue";
            this.RotateClockwiseTrue.UseVisualStyleBackColor = true;
            this.RotateClockwiseTrue.Click += new System.EventHandler(this.RotateClockwiseTrue_Click);
            // 
            // RotateClockwiseFalse
            // 
            this.RotateClockwiseFalse.Location = new System.Drawing.Point(228, 46);
            this.RotateClockwiseFalse.Name = "RotateClockwiseFalse";
            this.RotateClockwiseFalse.Size = new System.Drawing.Size(148, 23);
            this.RotateClockwiseFalse.TabIndex = 4;
            this.RotateClockwiseFalse.Text = "RotateClockwiseFalse";
            this.RotateClockwiseFalse.UseVisualStyleBackColor = true;
            this.RotateClockwiseFalse.Click += new System.EventHandler(this.RotateClockwiseFalse_Click);
            // 
            // RotatingTrue
            // 
            this.RotatingTrue.Location = new System.Drawing.Point(228, 75);
            this.RotatingTrue.Name = "RotatingTrue";
            this.RotatingTrue.Size = new System.Drawing.Size(148, 23);
            this.RotatingTrue.TabIndex = 5;
            this.RotatingTrue.Text = "RotatingTrue";
            this.RotatingTrue.UseVisualStyleBackColor = true;
            this.RotatingTrue.Click += new System.EventHandler(this.RotatingTrue_Click);
            // 
            // RotatingFalse
            // 
            this.RotatingFalse.Location = new System.Drawing.Point(228, 105);
            this.RotatingFalse.Name = "RotatingFalse";
            this.RotatingFalse.Size = new System.Drawing.Size(148, 23);
            this.RotatingFalse.TabIndex = 6;
            this.RotatingFalse.Text = "RotatingFalse";
            this.RotatingFalse.UseVisualStyleBackColor = true;
            this.RotatingFalse.Click += new System.EventHandler(this.RotatingFalse_Click);
            // 
            // GetRotateRate
            // 
            this.GetRotateRate.Location = new System.Drawing.Point(228, 135);
            this.GetRotateRate.Name = "GetRotateRate";
            this.GetRotateRate.Size = new System.Drawing.Size(148, 23);
            this.GetRotateRate.TabIndex = 7;
            this.GetRotateRate.Text = "GetRotateRate";
            this.GetRotateRate.UseVisualStyleBackColor = true;
            this.GetRotateRate.Click += new System.EventHandler(this.GetRotateRate_Click);
            // 
            // SetRotateRate
            // 
            this.SetRotateRate.Location = new System.Drawing.Point(228, 165);
            this.SetRotateRate.Name = "SetRotateRate";
            this.SetRotateRate.Size = new System.Drawing.Size(148, 23);
            this.SetRotateRate.TabIndex = 8;
            this.SetRotateRate.Text = "SetRotateRate";
            this.SetRotateRate.UseVisualStyleBackColor = true;
            this.SetRotateRate.Click += new System.EventHandler(this.SetRotateRate_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(383, 165);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(52, 21);
            this.textBox1.TabIndex = 9;
            // 
            // GetPoleLatitude
            // 
            this.GetPoleLatitude.Location = new System.Drawing.Point(228, 195);
            this.GetPoleLatitude.Name = "GetPoleLatitude";
            this.GetPoleLatitude.Size = new System.Drawing.Size(148, 23);
            this.GetPoleLatitude.TabIndex = 10;
            this.GetPoleLatitude.Text = "GetPoleLatitude";
            this.GetPoleLatitude.UseVisualStyleBackColor = true;
            // 
            // SetPoleLatitude
            // 
            this.SetPoleLatitude.Location = new System.Drawing.Point(228, 225);
            this.SetPoleLatitude.Name = "SetPoleLatitude";
            this.SetPoleLatitude.Size = new System.Drawing.Size(148, 23);
            this.SetPoleLatitude.TabIndex = 11;
            this.SetPoleLatitude.Text = "SetPoleLatitude";
            this.SetPoleLatitude.UseVisualStyleBackColor = true;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(383, 225);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(52, 21);
            this.textBox2.TabIndex = 12;
            // 
            // GetPoleLongitude
            // 
            this.GetPoleLongitude.Location = new System.Drawing.Point(228, 255);
            this.GetPoleLongitude.Name = "GetPoleLongitude";
            this.GetPoleLongitude.Size = new System.Drawing.Size(148, 23);
            this.GetPoleLongitude.TabIndex = 13;
            this.GetPoleLongitude.Text = "GetPoleLongitude";
            this.GetPoleLongitude.UseVisualStyleBackColor = true;
            // 
            // SetPoleLongitude
            // 
            this.SetPoleLongitude.Location = new System.Drawing.Point(228, 285);
            this.SetPoleLongitude.Name = "SetPoleLongitude";
            this.SetPoleLongitude.Size = new System.Drawing.Size(148, 23);
            this.SetPoleLongitude.TabIndex = 14;
            this.SetPoleLongitude.Text = "SetPoleLongitude";
            this.SetPoleLongitude.UseVisualStyleBackColor = true;
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(383, 285);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(52, 21);
            this.textBox3.TabIndex = 15;
            // 
            // ChapterAndPageNames
            // 
            this.ChapterAndPageNames.Location = new System.Drawing.Point(24, 314);
            this.ChapterAndPageNames.Name = "ChapterAndPageNames";
            this.ChapterAndPageNames.Size = new System.Drawing.Size(150, 23);
            this.ChapterAndPageNames.TabIndex = 16;
            this.ChapterAndPageNames.Text = "ChapterAndPageNames";
            this.ChapterAndPageNames.UseVisualStyleBackColor = true;
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(181, 314);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(152, 21);
            this.textBox4.TabIndex = 17;
            // 
            // textBox5
            // 
            this.textBox5.Location = new System.Drawing.Point(340, 314);
            this.textBox5.Name = "textBox5";
            this.textBox5.Size = new System.Drawing.Size(179, 21);
            this.textBox5.TabIndex = 18;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(531, 350);
            this.Controls.Add(this.textBox5);
            this.Controls.Add(this.textBox4);
            this.Controls.Add(this.ChapterAndPageNames);
            this.Controls.Add(this.textBox3);
            this.Controls.Add(this.SetPoleLongitude);
            this.Controls.Add(this.GetPoleLongitude);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.SetPoleLatitude);
            this.Controls.Add(this.GetPoleLatitude);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.SetRotateRate);
            this.Controls.Add(this.GetRotateRate);
            this.Controls.Add(this.RotatingFalse);
            this.Controls.Add(this.RotatingTrue);
            this.Controls.Add(this.RotateClockwiseFalse);
            this.Controls.Add(this.RotateClockwiseTrue);
            this.Controls.Add(this.Test);
            this.Controls.Add(this.Pause);
            this.Controls.Add(this.Play);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button Play;
        private System.Windows.Forms.Button Pause;
        private System.Windows.Forms.Button Test;
        private System.Windows.Forms.Button RotateClockwiseTrue;
        private System.Windows.Forms.Button RotateClockwiseFalse;
        private System.Windows.Forms.Button RotatingTrue;
        private System.Windows.Forms.Button RotatingFalse;
        private System.Windows.Forms.Button GetRotateRate;
        private System.Windows.Forms.Button SetRotateRate;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button GetPoleLatitude;
        private System.Windows.Forms.Button SetPoleLatitude;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Button GetPoleLongitude;
        private System.Windows.Forms.Button SetPoleLongitude;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.Button ChapterAndPageNames;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TextBox textBox5;
    }
}

