using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

/* Utilize either 'Debugging' (F5) or 'Without Debugging' (Ctrl + F5) to run program!!
 *
 *  public partial class Form1 : Form
 *              ^partial: allows breaking a class into multiple files
 *                      - allows generated code to be hidden from the user provided code.
 *                      - default of 'namespace'
 *                      - 'Form1' is derived from 'Form' which in turn is derived from 'Control'
 *  
 */

namespace InClassExample
{
    public partial class Form1 : Form
    {                           /* ^^ 'Form' is the class, and can be named anything! */
        public Form1()
        {
            InitializeComponent();
                          /* ^^ Constructor for Form1  */
        }



        // Text Box //
            /*    Note to Self: Delete all of this to get only 'Paint' section only.   */
        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            /*  (Leave Blank) Allowing you to have a text box on the pop-up  */
        }


        // Paint //
        private void Form1_Paint(object sender, PaintEventArgs e)
        {                                                   /* ^ This arg is important  */
            Graphics g = e.Graphics;
                      /* ^ the arg  */
            g.DrawString("Here is a line of text.", Font, Brushes.Red, 10, 10); 
                                                  /* ^^                  (x, y)
                                                   * ^^
                                                   * ^^ 'Font' here is a property, but it is also
                                                   *            known as a class */
        }
    }
}
