namespace Lab2
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.Connect = new System.Windows.Forms.CheckBox();
            this.ConnectAll = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // Connect
            // 
            this.Connect.AutoSize = true;
            this.Connect.Location = new System.Drawing.Point(47, 37);
            this.Connect.Name = "Connect";
            this.Connect.Size = new System.Drawing.Size(95, 24);
            this.Connect.TabIndex = 0;
            this.Connect.Text = "Connect";
            this.Connect.UseVisualStyleBackColor = true;
            this.Connect.CheckedChanged += new System.EventHandler(this.Connect_CheckedChanged);
            // 
            // ConnectAll
            // 
            this.ConnectAll.AutoSize = true;
            this.ConnectAll.Location = new System.Drawing.Point(47, 67);
            this.ConnectAll.Name = "ConnectAll";
            this.ConnectAll.Size = new System.Drawing.Size(112, 24);
            this.ConnectAll.TabIndex = 1;
            this.ConnectAll.Text = "ConnectAll";
            this.ConnectAll.UseVisualStyleBackColor = true;
            this.ConnectAll.CheckedChanged += new System.EventHandler(this.ConnectAll_CheckedChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1113, 612);
            this.Controls.Add(this.ConnectAll);
            this.Controls.Add(this.Connect);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "Lab #2 by Dylan Ramdhan";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox Connect;
        private System.Windows.Forms.CheckBox ConnectAll;
    }
}

