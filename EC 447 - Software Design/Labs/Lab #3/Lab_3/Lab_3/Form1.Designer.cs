namespace Lab_3
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
            this.AddingNum = new System.Windows.Forms.Button();
            this.SubtractNum = new System.Windows.Forms.Button();
            this.MultiplyNum = new System.Windows.Forms.Button();
            this.DivideNum = new System.Windows.Forms.Button();
            this.ClearButton = new System.Windows.Forms.Button();
            this.ResultTitle = new System.Windows.Forms.Label();
            this.InputLabel = new System.Windows.Forms.Label();
            this.InputTextBox = new System.Windows.Forms.TextBox();
            this.ResultTextBox = new System.Windows.Forms.TextBox();
            this.Title = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // AddingNum
            // 
            this.AddingNum.Location = new System.Drawing.Point(256, 387);
            this.AddingNum.Name = "AddingNum";
            this.AddingNum.Size = new System.Drawing.Size(128, 39);
            this.AddingNum.TabIndex = 0;
            this.AddingNum.Text = "Add";
            this.AddingNum.UseVisualStyleBackColor = true;
            this.AddingNum.Click += new System.EventHandler(this.AddingNum_Click);
            // 
            // SubtractNum
            // 
            this.SubtractNum.Location = new System.Drawing.Point(499, 387);
            this.SubtractNum.Name = "SubtractNum";
            this.SubtractNum.Size = new System.Drawing.Size(128, 39);
            this.SubtractNum.TabIndex = 1;
            this.SubtractNum.Text = "Subtract";
            this.SubtractNum.UseVisualStyleBackColor = true;
            this.SubtractNum.Click += new System.EventHandler(this.SubtractNum_Click);
            // 
            // MultiplyNum
            // 
            this.MultiplyNum.Location = new System.Drawing.Point(256, 461);
            this.MultiplyNum.Name = "MultiplyNum";
            this.MultiplyNum.Size = new System.Drawing.Size(128, 39);
            this.MultiplyNum.TabIndex = 2;
            this.MultiplyNum.Text = "Multiply";
            this.MultiplyNum.UseVisualStyleBackColor = true;
            this.MultiplyNum.Click += new System.EventHandler(this.MultiplyNum_Click);
            // 
            // DivideNum
            // 
            this.DivideNum.Location = new System.Drawing.Point(499, 461);
            this.DivideNum.Name = "DivideNum";
            this.DivideNum.Size = new System.Drawing.Size(128, 39);
            this.DivideNum.TabIndex = 3;
            this.DivideNum.Text = "Divide";
            this.DivideNum.UseVisualStyleBackColor = true;
            this.DivideNum.Click += new System.EventHandler(this.DivideNum_Click);
            // 
            // ClearButton
            // 
            this.ClearButton.Location = new System.Drawing.Point(649, 286);
            this.ClearButton.Name = "ClearButton";
            this.ClearButton.Size = new System.Drawing.Size(112, 30);
            this.ClearButton.TabIndex = 4;
            this.ClearButton.Text = "Clear";
            this.ClearButton.UseVisualStyleBackColor = true;
            this.ClearButton.Click += new System.EventHandler(this.ClearButton_Click);
            // 
            // ResultTitle
            // 
            this.ResultTitle.AutoSize = true;
            this.ResultTitle.Location = new System.Drawing.Point(181, 289);
            this.ResultTitle.Name = "ResultTitle";
            this.ResultTitle.Size = new System.Drawing.Size(59, 20);
            this.ResultTitle.TabIndex = 6;
            this.ResultTitle.Text = "Result:";
            // 
            // InputLabel
            // 
            this.InputLabel.AutoSize = true;
            this.InputLabel.Location = new System.Drawing.Point(181, 231);
            this.InputLabel.Name = "InputLabel";
            this.InputLabel.Size = new System.Drawing.Size(50, 20);
            this.InputLabel.TabIndex = 7;
            this.InputLabel.Text = "Input:";
            // 
            // InputTextBox
            // 
            this.InputTextBox.Location = new System.Drawing.Point(256, 231);
            this.InputTextBox.Name = "InputTextBox";
            this.InputTextBox.Size = new System.Drawing.Size(371, 26);
            this.InputTextBox.TabIndex = 8;
            // 
            // ResultTextBox
            // 
            this.ResultTextBox.BackColor = System.Drawing.SystemColors.Control;
            this.ResultTextBox.Location = new System.Drawing.Point(256, 286);
            this.ResultTextBox.Name = "ResultTextBox";
            this.ResultTextBox.ReadOnly = true;
            this.ResultTextBox.Size = new System.Drawing.Size(371, 26);
            this.ResultTextBox.TabIndex = 2;
            this.ResultTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // Title
            // 
            this.Title.AutoSize = true;
            this.Title.Font = new System.Drawing.Font("Arial Rounded MT Bold", 28F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Title.Location = new System.Drawing.Point(285, 113);
            this.Title.Name = "Title";
            this.Title.Size = new System.Drawing.Size(311, 65);
            this.Title.TabIndex = 10;
            this.Title.Text = "Calculator";
            this.Title.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Cyan;
            this.ClientSize = new System.Drawing.Size(880, 667);
            this.Controls.Add(this.Title);
            this.Controls.Add(this.ResultTextBox);
            this.Controls.Add(this.InputTextBox);
            this.Controls.Add(this.InputLabel);
            this.Controls.Add(this.ResultTitle);
            this.Controls.Add(this.ClearButton);
            this.Controls.Add(this.DivideNum);
            this.Controls.Add(this.MultiplyNum);
            this.Controls.Add(this.SubtractNum);
            this.Controls.Add(this.AddingNum);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Lab #3: Dylan Ramdhan";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button AddingNum;
        private System.Windows.Forms.Button SubtractNum;
        private System.Windows.Forms.Button MultiplyNum;
        private System.Windows.Forms.Button DivideNum;
        private System.Windows.Forms.Button ClearButton;
        private System.Windows.Forms.Label ResultTitle;
        private System.Windows.Forms.Label InputLabel;
        private System.Windows.Forms.TextBox InputTextBox;
        private System.Windows.Forms.TextBox ResultTextBox;
        private System.Windows.Forms.Label Title;
    }
}

