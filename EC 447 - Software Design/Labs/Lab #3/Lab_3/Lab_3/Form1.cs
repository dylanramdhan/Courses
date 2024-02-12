using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Security.AccessControl;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace Lab_3
{
    public partial class Form1 : Form
    {
        // Instantiating Results to be 0
        double Results = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // Initializing Reset upon loading
            // *** Sets the Starting Values ***
            reset();
        }



        // Resetting the Value
        // Then calls 'setResults' to Display the Cleared Screen
        private void reset() 
        {
            Results = 0;
            setResults();
        }



        

        // setResults: Sets the Results After Clearing or Calculation
        private void setResults() 
        {
            ResultTextBox.Text = Results.ToString();
            InputTextBox.Clear();
            this.Invalidate();
        }


        // ClearButton: Calls 'reset' class to Reset the Value
        private void ClearButton_Click(object sender, EventArgs e)
        {
            reset();
        }



        // Double Checking if Inputs are Valid (Displaying 'MessageBox')
        private bool isAcceptableValue(string input)
        {
            try
            {
                double testingNum = Convert.ToDouble(input);
                return true;
            }

            catch
            {
                MessageBox.Show("Invalid or Missing Value!");
                return false; 
            }
        }
        



                // ** Functions ** //
        private void AddingNum_Click(object sender, EventArgs e)
        {
            if (isAcceptableValue(InputTextBox.Text))
            {
                Results += Convert.ToDouble(InputTextBox.Text);
                setResults();
            }
        }

        private void SubtractNum_Click(object sender, EventArgs e)
        {
           if (isAcceptableValue(InputTextBox.Text))
            {
                Results -= Convert.ToDouble(InputTextBox.Text);
                setResults();
            }
        }

        private void MultiplyNum_Click(object sender, EventArgs e)
        {
            if (isAcceptableValue(InputTextBox.Text))
            {
                Results *= Convert.ToDouble(InputTextBox.Text);
                setResults();
            }
        }

        private void DivideNum_Click(object sender, EventArgs e)
        {
            if (isAcceptableValue(InputTextBox.Text))
            {
                Results /= Convert.ToDouble(InputTextBox.Text);
                setResults();
            }
        }

        
        
        
      
        
    }
}
