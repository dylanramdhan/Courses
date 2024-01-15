package com.example.ifp;

import android.graphics.Bitmap;
import android.graphics.Color;
/*
    This class contains a method that take the bitmap (image) in the image view and
    replace all green and blue pixel values by zero.
*/

public class zeroGBclass { //exception are handled in onClick listener
    /**
     * This method take the originalbitmap and obtain the width, heigh and pixel values. The pixel
     * values were stored in an array. Then the method empty all the green and blue pixel values of
     * the pixel array and create a new bitmap with the new pixel values.
     *
     * @param originalBitmap
     * @return zeroedBitmap
     */
    public static Bitmap removeGreenBlue(Bitmap originalBitmap) {
        // Takes the width and height of the uploaded image
        int width = originalBitmap.getWidth();
        int height = originalBitmap.getHeight();
        // Create a new empty bitmap with the same width and height as the uploaded image
        Bitmap zeroedBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        // Initialize pixel arrays to store the pixel values
        int[] pixels = new int[width * height];
        // Obtain the pixel values from the uploaded image
        originalBitmap.getPixels(pixels, 0, width, 0, 0, width, height);
        // Looping through the pixel arrays and replacing all the green and blue pixel values by 0
        for (int i = 0; i < pixels.length; i++) {
            int red = Color.red(pixels[i]);
            int recolorPixel = Color.rgb(red, 0, 0);
            pixels[i] = recolorPixel;
        }
        // Fill in the new empty bitmap with the new pixel values
        zeroedBitmap.setPixels(pixels, 0, width, 0, 0, width, height);
        // return the new bitmap
        return zeroedBitmap;
    }
}

