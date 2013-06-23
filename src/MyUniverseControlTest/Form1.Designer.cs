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
            this.TextBox_rotRate = new System.Windows.Forms.TextBox();
            this.GetPoleLatitude = new System.Windows.Forms.Button();
            this.SetPoleLatitude = new System.Windows.Forms.Button();
            this.textBox_PoleLatitude = new System.Windows.Forms.TextBox();
            this.GetPoleLongitude = new System.Windows.Forms.Button();
            this.SetPoleLongitude = new System.Windows.Forms.Button();
            this.textBox_PoleLongitude = new System.Windows.Forms.TextBox();
            this.SetChapterAndPageNames = new System.Windows.Forms.Button();
            this.textBox_chapter = new System.Windows.Forms.TextBox();
            this.textBox_page = new System.Windows.Forms.TextBox();
            this.GetChapterAndPageNames = new System.Windows.Forms.Button();
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
            // TextBox_rotRate
            // 
            this.TextBox_rotRate.Location = new System.Drawing.Point(383, 165);
            this.TextBox_rotRate.Name = "TextBox_rotRate";
            this.TextBox_rotRate.Size = new System.Drawing.Size(52, 21);
            this.TextBox_rotRate.TabIndex = 9;
            // 
            // GetPoleLatitude
            // 
            this.GetPoleLatitude.Location = new System.Drawing.Point(228, 195);
            this.GetPoleLatitude.Name = "GetPoleLatitude";
            this.GetPoleLatitude.Size = new System.Drawing.Size(148, 23);
            this.GetPoleLatitude.TabIndex = 10;
            this.GetPoleLatitude.Text = "GetPoleLatitude";
            this.GetPoleLatitude.UseVisualStyleBackColor = true;
            this.GetPoleLatitude.Click += new System.EventHandler(this.GetPoleLatitude_Click);
            // 
            // SetPoleLatitude
            // 
            this.SetPoleLatitude.Location = new System.Drawing.Point(228, 225);
            this.SetPoleLatitude.Name = "SetPoleLatitude";
            this.SetPoleLatitude.Size = new System.Drawing.Size(148, 23);
            this.SetPoleLatitude.TabIndex = 11;
            this.SetPoleLatitude.Text = "SetPoleLatitude";
            this.SetPoleLatitude.UseVisualStyleBackColor = true;
            this.SetPoleLatitude.Click += new System.EventHandler(this.SetPoleLatitude_Click);
            // 
            // textBox_PoleLatitude
            // 
            this.textBox_PoleLatitude.Location = new System.Drawing.Point(383, 225);
            this.textBox_PoleLatitude.Name = "textBox_PoleLatitude";
            this.textBox_PoleLatitude.Size = new System.Drawing.Size(52, 21);
            this.textBox_PoleLatitude.TabIndex = 12;
            // 
            // GetPoleLongitude
            // 
            this.GetPoleLongitude.Location = new System.Drawing.Point(228, 255);
            this.GetPoleLongitude.Name = "GetPoleLongitude";
            this.GetPoleLongitude.Size = new System.Drawing.Size(148, 23);
            this.GetPoleLongitude.TabIndex = 13;
            this.GetPoleLongitude.Text = "GetPoleLongitude";
            this.GetPoleLongitude.UseVisualStyleBackColor = true;
            this.GetPoleLongitude.Click += new System.EventHandler(this.GetPoleLongitude_Click);
            // 
            // SetPoleLongitude
            // 
            this.SetPoleLongitude.Location = new System.Drawing.Point(228, 285);
            this.SetPoleLongitude.Name = "SetPoleLongitude";
            this.SetPoleLongitude.Size = new System.Drawing.Size(148, 23);
            this.SetPoleLongitude.TabIndex = 14;
            this.SetPoleLongitude.Text = "SetPoleLongitude";
            this.SetPoleLongitude.UseVisualStyleBackColor = true;
            this.SetPoleLongitude.Click += new System.EventHandler(this.SetPoleLongitude_Click);
            // 
            // textBox_PoleLongitude
            // 
            this.textBox_PoleLongitude.Location = new System.Drawing.Point(383, 285);
            this.textBox_PoleLongitude.Name = "textBox_PoleLongitude";
            this.textBox_PoleLongitude.Size = new System.Drawing.Size(52, 21);
            this.textBox_PoleLongitude.TabIndex = 15;
            // 
            // SetChapterAndPageNames
            // 
            this.SetChapterAndPageNames.Location = new System.Drawing.Point(12, 347);
            this.SetChapterAndPageNames.Name = "SetChapterAndPageNames";
            this.SetChapterAndPageNames.Size = new System.Drawing.Size(150, 23);
            this.SetChapterAndPageNames.TabIndex = 16;
            this.SetChapterAndPageNames.Text = "SetChapterAndPageNames";
            this.SetChapterAndPageNames.UseVisualStyleBackColor = true;
            this.SetChapterAndPageNames.Click += new System.EventHandler(this.SetChapterAndPageNames_Click);
            // 
            // textBox_chapter
            // 
            this.textBox_chapter.Location = new System.Drawing.Point(169, 347);
            this.textBox_chapter.Name = "textBox_chapter";
            this.textBox_chapter.Size = new System.Drawing.Size(152, 21);
            this.textBox_chapter.TabIndex = 17;
            // 
            // textBox_page
            // 
            this.textBox_page.Location = new System.Drawing.Point(328, 347);
            this.textBox_page.Name = "textBox_page";
            this.textBox_page.Size = new System.Drawing.Size(179, 21);
            this.textBox_page.TabIndex = 18;
            // 
            // GetChapterAndPageNames
            // 
            this.GetChapterAndPageNames.Location = new System.Drawing.Point(13, 318);
            this.GetChapterAndPageNames.Name = "GetChapterAndPageNames";
            this.GetChapterAndPageNames.Size = new System.Drawing.Size(149, 23);
            this.GetChapterAndPageNames.TabIndex = 19;
            this.GetChapterAndPageNames.Text = "GetChapterAndPageNames";
            this.GetChapterAndPageNames.UseVisualStyleBackColor = true;
            this.GetChapterAndPageNames.Click += new System.EventHandler(this.GetChapterAndPageNames_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(531, 393);
            this.Controls.Add(this.GetChapterAndPageNames);
            this.Controls.Add(this.textBox_page);
            this.Controls.Add(this.textBox_chapter);
            this.Controls.Add(this.SetChapterAndPageNames);
            this.Controls.Add(this.textBox_PoleLongitude);
            this.Controls.Add(this.SetPoleLongitude);
            this.Controls.Add(this.GetPoleLongitude);
            this.Controls.Add(this.textBox_PoleLatitude);
            this.Controls.Add(this.SetPoleLatitude);
            this.Controls.Add(this.GetPoleLatitude);
            this.Controls.Add(this.TextBox_rotRate);
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
        private System.Windows.Forms.TextBox TextBox_rotRate;
        private System.Windows.Forms.Button GetPoleLatitude;
        private System.Windows.Forms.Button SetPoleLatitude;
        private System.Windows.Forms.TextBox textBox_PoleLatitude;
        private System.Windows.Forms.Button GetPoleLongitude;
        private System.Windows.Forms.Button SetPoleLongitude;
        private System.Windows.Forms.TextBox textBox_PoleLongitude;
        private System.Windows.Forms.Button SetChapterAndPageNames;
        private System.Windows.Forms.TextBox textBox_chapter;
        private System.Windows.Forms.TextBox textBox_page;
        private System.Windows.Forms.Button GetChapterAndPageNames;
    }
}

