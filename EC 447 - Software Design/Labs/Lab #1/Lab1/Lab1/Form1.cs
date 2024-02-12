using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Security;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        
        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            // Instantiating Graphics Class Reference
            Graphics g = e.Graphics;

            // Initiating Values
            int value = 2;
            int h = 0;  // used for height

            for (int i = 1; i <= 8; ++i)
            {
                string s = string.Format("{0} {1}", i, value);
                value *= 2;

                // 'Painting' on Windows
                g.DrawString(s, Font, Brushes.Black, 20, 20 + h);

                // Setting New Height of Line:
                h += Font.Height;
            }
            
            
        }
    }
}
