using Lab_5.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab_5
{
    public partial class Form1 : Form
    {
        // Instantiations
        string IntNumbers, StartingNumbers, CountingNumbers;

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        public Form1()
        {
            InitializeComponent();
        }

        private void GenerateButton_Click(object sender, EventArgs e)
        {
            // Prompting SettingDialog.cs
            SettingDialog sd = new SettingDialog();
            sd.ShowDialog();

            // Executes Until Valid Numbers are Entered
            // ~ Doesn't if CANCELED is clicked: Value resets to 0
            IntNumbers = sd.IntNumber;
            ListVals.Items.Clear();
         
            // IntNumber is a string that is separated with an 'x'
            // Breaking Down IntNumbers into 'start' & 'count'
            if (IntNumbers != null)
            {
                string[] stringg = IntNumbers.Split('o');
                StartingNumbers = stringg[0];
                CountingNumbers = stringg[1];


                // This Generates the Palidromes
                int Palidromes = 0;
                int currentPalidrome = int.Parse(StartingNumbers);


                while (Palidromes < int.Parse(CountingNumbers))
                {
                    // Comparing the String to its Reverse Self
                    string PaliNum1 = currentPalidrome.ToString();
                    string PaliNum2 = new string(PaliNum1.Reverse().ToArray());

                    if (PaliNum1 == PaliNum2)
                    {
                        ListVals.Items.Add(currentPalidrome);
                        Palidromes++;
                    }

                    currentPalidrome++;
                }

            }
            this.Invalidate();
        }
    }
}
