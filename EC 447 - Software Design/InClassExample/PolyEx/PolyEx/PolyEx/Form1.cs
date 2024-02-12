using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PolyEx
{
    public partial class Form1 : Form
    {
        private ArrayList pets = new ArrayList();

        public Form1()
        {
            InitializeComponent();
        }

        private void dogb_Click(object sender, EventArgs e)
        {
            pets.Add(new Dog());
            panel1.Invalidate();
        }

        private void catb_Click(object sender, EventArgs e)
        {
            pets.Add(new Cat());
            panel1.Invalidate();

        }

        private void birdb_Click(object sender, EventArgs e)
        {
            pets.Add(new Bird());
            panel1.Invalidate();

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            int cy = Font.Height;
            if (pets.Count > 0)
                panel1.Height = pets.Count * cy;
            int ypos = 0;

            foreach (Pet myPet in pets)
            {
                g.DrawString(myPet.Draw(), Font, Brushes.Black, 10, ypos);
                ypos += cy;
            }
        }

    }
}
