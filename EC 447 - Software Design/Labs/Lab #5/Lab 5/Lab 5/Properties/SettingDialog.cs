using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab_5.Properties
{
    public partial class SettingDialog : Form
{
    public SettingDialog()
    {
        InitializeComponent();
    }

    string numbers;

    public string IntNumber   // Initialized with 'string'
    {
        get
        {
            return (numbers);
        }

        set
        {
            // none
        }
    }

    private void ok_button_Click(object sender, EventArgs e)
    {
        int start, count;

        // Validating the Input:
        //      1. Checking if the input is an Integer.
        //      2. Checking if the input is in Range.

        if (int.TryParse(StartInt.Text, out start) && int.TryParse(CountInt.Text, out count))
        {
            if ((start >= 0 && start <= 1000000000) && (count > 0 && count <= 100))
            {
                ErrorLabel.Visible = false;
                numbers = start.ToString() + "x" + count.ToString();
                DialogResult = DialogResult.OK;
            }

            else
            {
                ErrorLabel.Visible = true;
            }
        }

        else
        {
            ErrorLabel.Visible = true;
        }


    }





    private void SettingDialog_Load(object sender, EventArgs e)
    {

    }

}
}