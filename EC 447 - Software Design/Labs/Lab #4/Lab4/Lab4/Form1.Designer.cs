namespace Lab4
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.GameHintsCheckBox = new System.Windows.Forms.CheckBox();
            this.ClearButton = new System.Windows.Forms.Button();
            this.QueensLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // GameHintsCheckBox
            // 
            this.GameHintsCheckBox.AutoSize = true;
            this.GameHintsCheckBox.Location = new System.Drawing.Point(20, 18);
            this.GameHintsCheckBox.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.GameHintsCheckBox.Name = "GameHintsCheckBox";
            this.GameHintsCheckBox.Size = new System.Drawing.Size(72, 24);
            this.GameHintsCheckBox.TabIndex = 0;
            this.GameHintsCheckBox.Text = "Hints";
            this.GameHintsCheckBox.UseVisualStyleBackColor = true;
            this.GameHintsCheckBox.CheckedChanged += new System.EventHandler(this.hintsCheckBox_CheckedChanged);
            // 
            // ClearButton
            // 
            this.ClearButton.Location = new System.Drawing.Point(140, 14);
            this.ClearButton.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.ClearButton.Name = "ClearButton";
            this.ClearButton.Size = new System.Drawing.Size(112, 35);
            this.ClearButton.TabIndex = 1;
            this.ClearButton.Text = "Clear";
            this.ClearButton.UseVisualStyleBackColor = true;
            this.ClearButton.Click += new System.EventHandler(this.ClearButton_Click);
            // 
            // QueensLabel
            // 
            this.QueensLabel.AutoSize = true;
            this.QueensLabel.Location = new System.Drawing.Point(297, 22);
            this.QueensLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.QueensLabel.Name = "QueensLabel";
            this.QueensLabel.Size = new System.Drawing.Size(51, 20);
            this.QueensLabel.TabIndex = 2;
            this.QueensLabel.Text = "label1";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(910, 894);
            this.Controls.Add(this.QueensLabel);
            this.Controls.Add(this.ClearButton);
            this.Controls.Add(this.GameHintsCheckBox);
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Eight Queens by Dylan Ramdhan";
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.Form1_Paint);
            this.MouseClick += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseClick);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox GameHintsCheckBox;
        private System.Windows.Forms.Button ClearButton;
        private System.Windows.Forms.Label QueensLabel;
    }
}

