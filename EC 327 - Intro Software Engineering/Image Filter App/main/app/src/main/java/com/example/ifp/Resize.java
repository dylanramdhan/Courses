package com.example.ifp;

import android.graphics.Bitmap;
/*
    This class contains a method that take the bitmap (image) in the image view and
    resize it to a new dimensions that the user entered.
*/


public class Resize {
    /**
     * This method take the new image width and height value that the user entered and create a new
     * bitmap with the originalBitmap pixel values and the new width and height values.
     *
     * @param originalBitmap
     * @param newWidth
     * @param newHeight
     * @return resizedBitmap
     */


    public static Bitmap resizeImage(Bitmap originalBitmap, int newWidth, int newHeight) {

        // Create a new bitmap with the desired dimensions
        Bitmap resizedBitmap = Bitmap.createScaledBitmap(originalBitmap, newWidth, newHeight, true);

        // Update the ImageView with the resized bitmap
        return resizedBitmap;
    }
}

