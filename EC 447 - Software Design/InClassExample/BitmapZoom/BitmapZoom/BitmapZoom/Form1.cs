using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace BitmapZoom
{

    public partial class Form1 : Form
    {
        private Bitmap bm;
        private RectangleF dragRect = new Rectangle();
        private bool dragging = false;
        private float zoom = 1.0f;   //zoom factor
        private float width, height;
        public Form1()
        {

            InitializeComponent();
            zoomToolStripMenuItem.Visible = false; //hide initially
            closeToolStripMenuItem.Enabled = false;
            ResizeRedraw = true;

        }
        private void client_Paint(object sender, PaintEventArgs e)
        {
            if (bm == null) return;
            Graphics g = e.Graphics;
            g.TranslateTransform(client.AutoScrollPosition.X,
                client.AutoScrollPosition.Y);
            g.DrawImage(bm, 0, 0, width, height);
            if (dragging)
            {
                RectangleF r = dragRect;
                r.X -= client.AutoScrollPosition.X;
                r.Y -= client.AutoScrollPosition.Y;
                g.DrawRectangle(Pens.Black, r.X, r.Y, r.Width, r.Height);
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (openFile.ShowDialog(this) == DialogResult.OK)
            {
                try
                {
                    bm = new Bitmap(openFile.FileName);
                    width = bm.Width;
                    height = bm.Height;
                    zoom = 1.0f;
                    client.AutoScrollMinSize = new Size((int)width,
                        (int)height);
                    zoomToolStripMenuItem.Visible = true;
                    closeToolStripMenuItem.Enabled = true;
                    client.Invalidate();
                }
                catch
                {
                    MessageBox.Show("Not an image file!");
                }
            }
        }
        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (bm != null) bm.Dispose();
            bm = null;
            zoomToolStripMenuItem.Visible = false;
            closeToolStripMenuItem.Enabled = false;
            client.Invalidate();
        }

        private void client_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                dragRect.X = e.X;
                dragRect.Y = e.Y;
                dragRect.Width = 0;
                dragRect.Height = 0;
                dragging = true;
            }
        }

        private void client_MouseMove(object sender, MouseEventArgs e)
        {
            if (dragging)
            {
                dragRect.Width = e.X - dragRect.X;
                dragRect.Height = e.Y - dragRect.Y;
                client.Invalidate();
            }
        }
        private void client_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                Size cs = client.ClientSize;
                dragging = false;
                float zf, dx, dy;
                PointF p = client.AutoScrollPosition;
                //ignore if rectangle is too small
                if (dragRect.Width < 10 || dragRect.Height < 10)
                {
                    client.Invalidate();
                    return;
                }
                //calculate zoom factor
                zf = Math.Min(cs.Width / dragRect.Width, cs.Height / dragRect.Height);
                //find center of zoom box
                dx = (-p.X + dragRect.X + dragRect.Width / 2);
                dy = (-p.Y + dragRect.Y + dragRect.Width / 2);
                //scale to new position
                dx *= zf;
                dy *= zf;
                //subtract client offset
                dx -= cs.Width / 2;
                dy -= cs.Height / 2;
                //zoom in/out
                if (SetZoom(zf))
                    //scroll to center point
                    client.AutoScrollPosition = new Point((int)dx, (int)dy);
                client.Invalidate();

            }
        }
        //        }
        private void actualSizeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SetZoom(1 / zoom);
        }

        private void inToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SetZoom(1.5f);

        }

        private void outToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SetZoom(1 / 1.5f);

        }
        private bool SetZoom(float zf)
        {
            if (bm == null) return false;
            float lastzoom = zoom;
            float lastwidth = width;
            float lastheight = height;
            zoom *= zf;
            width = bm.Width * zoom;
            height = bm.Height * zoom;
            if (width < 10 || height < 10 || width > Int16.MaxValue || height > Int16.MaxValue)
            {
                MessageBox.Show("Zoom range exceeded!");
                width = lastwidth;
                height = lastheight;
                zoom = lastzoom;
                return false;
            }
            //adjust scroll position
            Size cs = client.ClientSize;
            PointF p = client.AutoScrollPosition;
            //get current center point
            float dx = -p.X + cs.Width / 2;
            float dy = -p.Y + cs.Height / 2;
            //scale to new position
            dx *= zf;
            dy *= zf;
            //subtract client offset
            dx -= cs.Width / 2;
            dy -= cs.Height / 2;
            //set new bounds
            client.AutoScrollMinSize = new Size((int)width,
              (int)height);
            //scroll to new center point
            client.AutoScrollPosition = new Point((int)dx, (int)dy);
            client.Invalidate();
            return true;

        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }
    }

    //derive from panel to allow double buffering to be set
    public class Client : Panel
    {
        public Client()
        {
            DoubleBuffered = true;
        }
    }
}