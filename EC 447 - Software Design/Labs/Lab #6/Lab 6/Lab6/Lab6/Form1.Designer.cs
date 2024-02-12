namespace Lab6
{
    partial class Lab6
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Lab6));
            this.EncryptionButton = new System.Windows.Forms.Button();
            this.DecryptionButton = new System.Windows.Forms.Button();
            this.BrowsingButton = new System.Windows.Forms.Button();
            this.FileNameTextBox = new System.Windows.Forms.TextBox();
            this.KeyTextBox = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // EncryptionButton
            // 
            this.EncryptionButton.Location = new System.Drawing.Point(90, 231);
            this.EncryptionButton.Name = "EncryptionButton";
            this.EncryptionButton.Size = new System.Drawing.Size(114, 34);
            this.EncryptionButton.TabIndex = 0;
            this.EncryptionButton.Text = "Encrypt";
            this.EncryptionButton.UseVisualStyleBackColor = true;
            this.EncryptionButton.Click += new System.EventHandler(this.EncryptionButton_Click);
            // 
            // DecryptionButton
            // 
            this.DecryptionButton.Location = new System.Drawing.Point(317, 231);
            this.DecryptionButton.Name = "DecryptionButton";
            this.DecryptionButton.Size = new System.Drawing.Size(114, 34);
            this.DecryptionButton.TabIndex = 1;
            this.DecryptionButton.Text = "Decrypt";
            this.DecryptionButton.UseVisualStyleBackColor = true;
            this.DecryptionButton.Click += new System.EventHandler(this.DecryptionButton_Click);
            // 
            // BrowsingButton
            // 
            this.BrowsingButton.Location = new System.Drawing.Point(430, 72);
            this.BrowsingButton.Name = "BrowsingButton";
            this.BrowsingButton.Size = new System.Drawing.Size(114, 34);
            this.BrowsingButton.TabIndex = 2;
            this.BrowsingButton.Text = "Browse";
            this.BrowsingButton.UseVisualStyleBackColor = true;
            this.BrowsingButton.Click += new System.EventHandler(this.BrowsingButton_Click);
            // 
            // FileNameTextBox
            // 
            this.FileNameTextBox.Location = new System.Drawing.Point(90, 76);
            this.FileNameTextBox.Name = "FileNameTextBox";
            this.FileNameTextBox.Size = new System.Drawing.Size(301, 26);
            this.FileNameTextBox.TabIndex = 3;
            // 
            // KeyTextBox
            // 
            this.KeyTextBox.Location = new System.Drawing.Point(90, 166);
            this.KeyTextBox.Name = "KeyTextBox";
            this.KeyTextBox.Size = new System.Drawing.Size(169, 26);
            this.KeyTextBox.TabIndex = 4;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(86, 53);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(84, 20);
            this.label1.TabIndex = 5;
            this.label1.Text = "File Name:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(86, 143);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(39, 20);
            this.label2.TabIndex = 6;
            this.label2.Text = "Key:";
            // 
            // Lab6
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(590, 343);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.KeyTextBox);
            this.Controls.Add(this.FileNameTextBox);
            this.Controls.Add(this.BrowsingButton);
            this.Controls.Add(this.DecryptionButton);
            this.Controls.Add(this.EncryptionButton);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Lab6";
            this.Text = "Lab #6 by Dylan Ramdhan";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button EncryptionButton;
        private System.Windows.Forms.Button DecryptionButton;
        private System.Windows.Forms.Button BrowsingButton;
        private System.Windows.Forms.TextBox FileNameTextBox;
        private System.Windows.Forms.TextBox KeyTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
    }
}

