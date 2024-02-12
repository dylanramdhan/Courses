using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TrackBar;

namespace Lab2
{
    public partial class Form1 : Form
    {
        // Size of Circle
        int size = 10;

        // Storing the Points of Circles on the Form
        private ArrayList CircleCoordinates = new ArrayList(); 

        public Form1()
        {
            InitializeComponent();
            this.MouseClick += Form1_MouseClick;
        }

        // Connect Check Box
        private void Connect_CheckedChanged(object sender, EventArgs e)
        {
            // Double Checking the Error 
            if (Connect.Checked && ConnectAll.Checked)
            {
                MessageBox.Show("ERROR: Both boxes checked. Only check ONE box at a time.");

                Connect.Checked = false;
                ConnectAll.Checked = false;
            }
            
            else
            {
                this.Invalidate();
            }
            
        }


        // Connect All Check Box
        private void ConnectAll_CheckedChanged(object sender, EventArgs e)
        {
            // Double Cheking Errors
            if (Connect.Checked && ConnectAll.Checked)
            {
                MessageBox.Show("ERROR: Both boxes checked. Only check ONE box at a time.");

                Connect.Checked = false;
                ConnectAll.Checked = false;
            }
            
            else
            {
                this.Invalidate();
            }

            
        }


        private void Form1_MouseClick(object sender, MouseEventArgs e)
        {
            // Points containts X and Y Coordinants
            PointF pt = new PointF(e.X, e.Y);
            this.CircleCoordinates.Add(pt);

            this.Invalidate();


            // Clearing ALL of the Dots
            if (e.Button == MouseButtons.Right)
            {
                this.CircleCoordinates.Clear();
                this.Invalidate();
            }
        }


        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            Graphics g = e.Graphics;

            //! Draw circles
            foreach (PointF p in this.CircleCoordinates)
            {
                g.FillEllipse(Brushes.Black, p.X - size / 2, p.Y - size / 2, size, size);
            }

            //! Pen used to draw lines
            Pen myPen = new Pen(Color.Black, 1);


            // If 'Connected' is selected:
            if (CircleCoordinates.Count > 0)        // Error Checking is Done in the CheckChanged Method ^^^
            {
                // Connect Box is Chcked
                if (Connect.Checked)
                {
                    for (int CountCircle = 0; CountCircle < CircleCoordinates.Count; CountCircle++)
                    {
                        // Drawing the Line Between the Circles, and then Leading Back to 0
                        if (CountCircle < CircleCoordinates.Count - 1)
                        {
                            g.DrawLine(myPen, (PointF)CircleCoordinates[CountCircle], (PointF)CircleCoordinates[CountCircle + 1]);
                        }
                    }
                    // Drawing the Closing Line ONLY if Less than 2 Circles.
                    // Count - 1, is the for the Start-At Zero Counting
                    if (CircleCoordinates.Count > 2) g.DrawLine(myPen, (PointF)CircleCoordinates[CircleCoordinates.Count - 1], (PointF)CircleCoordinates[0]);
                }

                // Connecting ALL Feature: Connecting All of the Dots //
                if (ConnectAll.Checked)
                {
                    // Creating the Connecting Line 
                    if (CircleCoordinates.Count > 1)
                    {
                        // CurrentCircle = the 'CURRENT' Circle that is being Drawn //
                        for (int CurrentCircle = 0; CurrentCircle < CircleCoordinates.Count; CurrentCircle++)
                        {
                            // NextCircle = is all the Next Circles We Draw to
                            for (int NextCircle = CurrentCircle; NextCircle < CircleCoordinates.Count; NextCircle++)
                            
                                g.DrawLine(myPen, (PointF)CircleCoordinates[CurrentCircle], (PointF)CircleCoordinates[NextCircle]);
                            }
                        }
                    }
                }
            }

        }
}