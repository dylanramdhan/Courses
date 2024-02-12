using System;
using System.IO;
using System.Windows.Forms;

namespace Lab6
{
    public partial class Lab6 : Form
    {
        public Lab6()
        {
            InitializeComponent();
        }



        // Browsing Button
        private void BrowsingButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog Dialog = new OpenFileDialog();

            // Allowing User to Access Files on Their Computer
            Dialog.Filter = "All Files (*.*)|*.*|Encrypted Files (*.enc)|*.enc";

            if (Dialog.ShowDialog() == DialogResult.OK)
            {
                FileNameTextBox.Text = Dialog.FileName;
            }
        }





        // Checking if user's inputted KEY & FILE DESTINATIONS are VALID
        private bool ValidInput(string button)
        {
            // Checking if a Key is Entered
            if (string.IsNullOrEmpty(KeyTextBox.Text))
            {
                MessageBox.Show("Please enter a key.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            // Checking if a File Exists
            if (!File.Exists(FileNameTextBox.Text))
            {
                MessageBox.Show("Could not open source or destination file.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            if (button == "Decrypt")
            {
                if (!FileNameTextBox.Text.EndsWith(".enc", StringComparison.OrdinalIgnoreCase))
                {
                    MessageBox.Show("Not a '.enc' file.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
            }

            return true;
        }






        // Crypting Algorithm for both Encryption & Decryption
        private void CryptionAlgo(string inFile, string outFile)
        {
            string key = KeyTextBox.Text;

            // Microsoft Documentation Used:
            //  https://learn.microsoft.com/en-us/dotnet/api/system.io.file?view=net-7.0


            int pos = 0; // Positioning of the Key String
            int length = key.Length; // Length of the Key
            byte kbyte, ebyte; // Encrypted Byte

            using (FileStream fileIn = File.OpenRead(inFile))
            using (FileStream fileOut = File.OpenWrite(outFile))
            {
                int rbyte;
                while ((rbyte = fileIn.ReadByte()) != -1)
                {
                    kbyte = (byte)key[pos];
                    ebyte = (byte)(rbyte ^ kbyte);

                    fileOut.WriteByte(ebyte);

                    ++pos;
                    if (pos == length)
                    {
                        pos = 0;
                    }
                }
            }
        }






        // Encryption Button
        private void EncryptionButton_Click(object sender, EventArgs e)
        {
            string file = FileNameTextBox.Text;
            if (ValidInput("N/A"))
            {
                DialogResult result = MessageBox.Show("Output file exists. Overwrite?", "File Exists", MessageBoxButtons.YesNo);

                if (result == DialogResult.No)
                {
                    return;
                }

                CryptionAlgo(file, file + ".enc");
                MessageBox.Show("Operation Completed Successfully.");
            }
        }




        // Decryption Button
        private void DecryptionButton_Click(object sender, EventArgs e)
        {
            string file = FileNameTextBox.Text;

            if (ValidInput("Decrypt"))
            {
                string outputFileName = file.EndsWith(".enc", StringComparison.OrdinalIgnoreCase)
                    ? file.Substring(0, file.Length - 4)
                    : file + ".dec";

                if (File.Exists(outputFileName))
                {
                    DialogResult result = MessageBox.Show("Output File Exists. Overwrite?", "File Exists", MessageBoxButtons.YesNo, MessageBoxIcon.Question);

                    if (result == DialogResult.No)
                    {
                        return;
                    }
                }

                CryptionAlgo(file, outputFileName);
                MessageBox.Show("Operation Completed Successfully.");
            }




        }
    }
}
