namespace PolyEx
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
            this.dogb = new System.Windows.Forms.Button();
            this.catb = new System.Windows.Forms.Button();
            this.birdb = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.SuspendLayout();
            // 
            // dogb
            // 
            this.dogb.Location = new System.Drawing.Point(22, 24);
            this.dogb.Name = "dogb";
            this.dogb.Size = new System.Drawing.Size(75, 23);
            this.dogb.TabIndex = 0;
            this.dogb.Text = "Add dog";
            this.dogb.UseVisualStyleBackColor = true;
            this.dogb.Click += new System.EventHandler(this.dogb_Click);
            // 
            // catb
            // 
            this.catb.Location = new System.Drawing.Point(118, 24);
            this.catb.Name = "catb";
            this.catb.Size = new System.Drawing.Size(75, 23);
            this.catb.TabIndex = 1;
            this.catb.Text = "Add cat";
            this.catb.UseVisualStyleBackColor = true;
            this.catb.Click += new System.EventHandler(this.catb_Click);
            // 
            // birdb
            // 
            this.birdb.Location = new System.Drawing.Point(214, 23);
            this.birdb.Name = "birdb";
            this.birdb.Size = new System.Drawing.Size(75, 23);
            this.birdb.TabIndex = 2;
            this.birdb.Text = "Add bird";
            this.birdb.UseVisualStyleBackColor = true;
            this.birdb.Click += new System.EventHandler(this.birdb_Click);
            // 
            // panel1
            // 
            this.panel1.Location = new System.Drawing.Point(22, 70);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(267, 171);
            this.panel1.TabIndex = 3;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(323, 262);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.birdb);
            this.Controls.Add(this.catb);
            this.Controls.Add(this.dogb);
            this.Name = "Form1";
            this.Text = "Polymorphism example";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button dogb;
        private System.Windows.Forms.Button catb;
        private System.Windows.Forms.Button birdb;
        private System.Windows.Forms.Panel panel1;
    }
}

