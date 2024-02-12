namespace Lab_5.Properties
{
    partial class SettingDialog
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
            this.ErrorLabel = new System.Windows.Forms.Label();
            this.CountInt = new System.Windows.Forms.TextBox();
            this.StartInt = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.ok_button = new System.Windows.Forms.Button();
            this.cancel_button = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // ErrorLabel
            // 
            this.ErrorLabel.AutoSize = true;
            this.ErrorLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 16F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ErrorLabel.Location = new System.Drawing.Point(202, 198);
            this.ErrorLabel.Name = "ErrorLabel";
            this.ErrorLabel.Size = new System.Drawing.Size(696, 37);
            this.ErrorLabel.TabIndex = 13;
            this.ErrorLabel.Text = "Please Enter a Positive Integer Within Range";
            // 
            // CountInt
            // 
            this.CountInt.Location = new System.Drawing.Point(785, 121);
            this.CountInt.Name = "CountInt";
            this.CountInt.Size = new System.Drawing.Size(124, 26);
            this.CountInt.TabIndex = 12;
            // 
            // StartInt
            // 
            this.StartInt.Location = new System.Drawing.Point(390, 121);
            this.StartInt.Name = "StartInt";
            this.StartInt.Size = new System.Drawing.Size(124, 26);
            this.StartInt.TabIndex = 11;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(617, 121);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(162, 20);
            this.label2.TabIndex = 10;
            this.label2.Text = "Enter Count (1 - 100):";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(50, 121);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(339, 20);
            this.label1.TabIndex = 9;
            this.label1.Text = "Entering a Starting Integer (0 - 1,000,000,000):";
            // 
            // ok_button
            // 
            this.ok_button.Location = new System.Drawing.Point(638, 280);
            this.ok_button.Name = "ok_button";
            this.ok_button.Size = new System.Drawing.Size(130, 48);
            this.ok_button.TabIndex = 8;
            this.ok_button.Text = "OK";
            this.ok_button.UseVisualStyleBackColor = true;
            // 
            // cancel_button
            // 
            this.cancel_button.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancel_button.Location = new System.Drawing.Point(332, 280);
            this.cancel_button.Name = "cancel_button";
            this.cancel_button.Size = new System.Drawing.Size(130, 48);
            this.cancel_button.TabIndex = 7;
            this.cancel_button.Text = "CANCEL";
            this.cancel_button.UseVisualStyleBackColor = true;
            // 
            // SettingDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1019, 446);
            this.ControlBox = false;
            this.Controls.Add(this.ErrorLabel);
            this.Controls.Add(this.CountInt);
            this.Controls.Add(this.StartInt);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.ok_button);
            this.Controls.Add(this.cancel_button);
            this.Name = "SettingDialog";
            this.Text = "SettingDialog";
            this.Load += new System.EventHandler(this.SettingDialog_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label ErrorLabel;
        private System.Windows.Forms.TextBox CountInt;
        private System.Windows.Forms.TextBox StartInt;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button ok_button;
        private System.Windows.Forms.Button cancel_button;
    }
}