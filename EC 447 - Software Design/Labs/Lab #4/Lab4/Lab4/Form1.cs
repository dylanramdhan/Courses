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
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Tab;

namespace Lab4
{
    public partial class Form1 : Form
    {
        // Standardized Variables
        const int Queens = 8;
        int SizeOfCheckerBoxes = 50, SizeOfBorder = 100, QueensOnBoard = 0;


        //  Bool Statments //
        bool GameHints = false;
        bool PlayingGame = true; // Set Automatically to FALSE if all 8 Queens are Placed Correctly
       


        /* Safety Array:  0 = SAFE
                          1 = BLOCKED

            - 0 is the default -      */

        bool[,] ArrayForBoard = new bool[Queens, Queens]; 
        


        // Coordinates of Queen Moves //
        private ArrayList QueenCoordinates = new ArrayList();
        

        public Form1()
        {
            InitializeComponent();
            ResizeRedraw = true;
        }



        // Hint Check Box //
        private void hintsCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (GameHintsCheckBox.Checked) // If 'Hints' is CHECKED
            {
                GameHints = true;
            }
            
            else // If 'Hints' is NOT CHECKED
            {
                GameHints = false;
            }
            
            Invalidate();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;

            Pen BoxDrawingPen = new Pen(Brushes.Black, 1);

            // Drawing the Boxes
            for (int xx = 0; xx < Queens; xx++)
            {
                for (int uu = 0; uu < Queens; uu++)
                {

                    // The Boards Color //
                    if ((xx + uu) % 2 != 0)   // Every Other Square //
                    {
                        g.FillRectangle(Brushes.Black, xx * SizeOfCheckerBoxes + SizeOfBorder,
                            uu * SizeOfCheckerBoxes + SizeOfBorder, SizeOfCheckerBoxes, SizeOfCheckerBoxes);
                    }
                        
                    else // Other Square is White //
                    {
                        g.FillRectangle(Brushes.White, xx * SizeOfCheckerBoxes + SizeOfBorder,
                            uu * SizeOfCheckerBoxes + SizeOfBorder, SizeOfCheckerBoxes, SizeOfCheckerBoxes);
                    }
                       

                    if (GameHints)
                    {
                        if (ArrayForBoard[xx, uu] == true)
                        {
                            g.FillRectangle(Brushes.Red, xx * SizeOfCheckerBoxes + SizeOfBorder,
                                uu * SizeOfCheckerBoxes + SizeOfBorder, SizeOfCheckerBoxes, SizeOfCheckerBoxes);
                        }

                    }

                    // Draw Outlines of the Boxes
                    g.DrawRectangle(BoxDrawingPen, xx * SizeOfCheckerBoxes + SizeOfBorder,
                        uu * SizeOfCheckerBoxes + SizeOfBorder, SizeOfCheckerBoxes, SizeOfCheckerBoxes);
                }
            }





            // Checking for Queens vs Queens //
            if (QueensOnBoard == 1)
            {
                QueensLabel.Text = "You have " + QueensOnBoard + " Queen on the board.";
            }

            else
            {
                QueensLabel.Text = "You have " + QueensOnBoard + " Queens on the board.";
            }

            Font FontsForQueen = new Font("Arial", 30);




            foreach (PointF p in this.QueenCoordinates)
            {
                if ((p.X + p.Y) % 2 == 0 || (GameHints && ArrayForBoard[(int)p.X, (int)p.Y] == true))
                {
                    g.DrawString("Q", FontsForQueen, Brushes.Black,
                        p.X * SizeOfCheckerBoxes + SizeOfBorder, p.Y * SizeOfCheckerBoxes + SizeOfBorder);
                }
                
                    // Inverse of 'Q' //
                else
                {
                    g.DrawString("Q", FontsForQueen, Brushes.White,
                        p.X * SizeOfCheckerBoxes + SizeOfBorder, p.Y * SizeOfCheckerBoxes + SizeOfBorder);
                }
            }

            if (QueensOnBoard == Queens && PlayingGame)
            {
                MessageBox.Show("You Did It!");
                PlayingGame = false;
            }
        }



        private void Form1_MouseClick(object sender, MouseEventArgs e)
        {
            // Getting Mouses's Coordinates //
            float MouseX_Axis, MouseY_Axis;
            
            MouseX_Axis = e.X; 
            MouseY_Axis = e.Y;

            float tmpX_Axis = (e.X - SizeOfBorder) / SizeOfCheckerBoxes;
            float tmpY_Axis = (e.Y - SizeOfBorder) / SizeOfCheckerBoxes;

            if ((e.X - SizeOfBorder) < 0 || (e.Y - SizeOfBorder) < 0)
            {
                return;
            }


            int BoardX_Coord = (int)tmpX_Axis;
            int BoardY_Coord = (int)tmpY_Axis;

            MessageBox.Show(MouseX_Axis + " " + MouseY_Axis + "\nX: " + BoardX_Coord + "\tY: " + BoardY_Coord);



            // Returning if Right Mouse Click is Summoned //
            if (e.Button == MouseButtons.Right)
            {
                return;
            }


            //  Checking for Invalid Spaces (Location & Queen's Coverages) with Beep Feature //
            if ((BoardX_Coord < 0) || (BoardX_Coord > 7) || (BoardY_Coord < 0) || (BoardY_Coord > 7))
            {
                System.Media.SystemSounds.Beep.Play();  // Playing Beep
                return;
            }

            else if (ArrayForBoard[BoardX_Coord, BoardY_Coord] == true)
            {
                System.Media.SystemSounds.Beep.Play();  // Playing Beep
                return;
            }



            // Adding in a New Queen //
            QueensOnBoard++;

            PointF p = new PointF(BoardX_Coord, BoardY_Coord);
            this.QueenCoordinates.Add(p);
            ArrayForBoard[BoardX_Coord, BoardY_Coord] = true;


            // Running Safety Algo After Placement
            for (int ll = 0; ll < Queens; ll++)
            {
                for (int oo = 0; oo < Queens; oo++)
                {
                    // Check rows and cols
                    if (ll == BoardX_Coord || oo == BoardY_Coord)
                    {
                        ArrayForBoard[ll, oo] = true;
                    }
                    else if (Math.Abs(ll - BoardX_Coord) == Math.Abs(oo - BoardY_Coord))
                    {
                        ArrayForBoard[ll, oo] = true;
                    }
                }
            }

            Invalidate();
        }



        // Functions for Clear Button //
        private void ClearButton_Click(object sender, EventArgs e)
        {
            QueensOnBoard = 0;  // Resets Board to None
            QueenCoordinates.Clear();   // Clears Boards Coordinates
            
            Array.Clear(ArrayForBoard, 0, ArrayForBoard.Length);
            PlayingGame = true;
            
            Invalidate();
        }
    }
}