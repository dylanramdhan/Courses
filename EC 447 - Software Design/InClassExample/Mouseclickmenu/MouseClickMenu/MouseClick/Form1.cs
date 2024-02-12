using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;

namespace MouseClick
{
    public partial class Form1 : Form
    {
        private ArrayList coordinates = new ArrayList();

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                Point p = new Point(e.X, e.Y);
                coordinates.Add(p);
                Invalidate();
            }
            if (e.Button == MouseButtons.Right)
            {
                coordinates.Clear();
                Invalidate();
            }
        }   

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            const int WIDTH = 20;
            const int HEIGHT = 20;

            Graphics g = e.Graphics;
            foreach (Point p in this.coordinates)
            {
                g.FillEllipse(Brushes.Black, 
                    p.X-WIDTH/2, p.Y-HEIGHT/2, WIDTH, HEIGHT);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            coordinates.Clear();
            Invalidate();
        }


    }
}