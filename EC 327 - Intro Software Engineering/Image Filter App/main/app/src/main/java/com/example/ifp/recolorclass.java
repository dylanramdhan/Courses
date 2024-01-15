package com.example.ifp;

import android.graphics.Bitmap;
import android.graphics.Color;
/*
    This class contains a method that take the bitmap (image) in the image view and
    replace a color to a new color, which both values were input by the user.
*/

public class recolorclass { //exception are handled in onClick listener
    /**
     * This method first check if the target RGB values appears in the originalBitmap. If there's
     * none, it will return null, otherwise it will replace the target RGB values to the new RGB
     * values that the user had entered. The entered pixels values should all be in the range of
     * 0 to 100. This method with then convert the entered pixel values to a range of 0 to 255 and
     * replace the targeted RGB values in the bitmap to the new RGB values. In the end, it return a
     * new recolor bitmap.
     *
     * @param originalBitmap
     * @param targetR
     * @param targetG
     * @param targetB
     * @param newR
     * @param newG
     * @param newB
     * @return recolorBitmap
     * @throws Exception
     */

    public static Bitmap applyRecolor(Bitmap originalBitmap, int targetR, int targetG, int targetB, int newR, int newG, int newB) throws Exception {
        try {
            // Takes the width and height of the uploaded image
            int width = originalBitmap.getWidth();
            int height = originalBitmap.getHeight();
            // Create a new empty bitmap with the same width and height as the uploaded image
            Bitmap recolorBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
            // Initialize pixel arrays to store the pixel values
            int[] pixels = new int[width * height];
            // Obtain the pixel values from the uploaded image
            originalBitmap.getPixels(pixels, 0, width, 0, 0, width, height);

            boolean colorFound = false;
            // Obtain the RBG values
            for (int i = 0; i < pixels.length; i++) {
                int pixelColor = pixels[i];
                int R = Color.red(pixelColor);
                int G = Color.green(pixelColor);
                int B = Color.blue(pixelColor);

                // Check if the targeted pixel values exist in the uploaded image or not
                if (R == targetR * 255 / 100 && G == targetG * 255 / 100 && B == targetB * 255 / 100) {
                    colorFound = true;
                    int newPixelColor = Color.rgb(
                            (int) Math.round(newR * 2.55),
                            (int) Math.round(newG * 2.55),
                            (int) Math.round(newB * 2.55)
                    );
                    // If it's true, it will replace the pixel values with the new pixel values
                    pixels[i] = newPixelColor;
                }
            }
            // If the user want to target a color that doesn't exist, it will return null
            if (!colorFound) {
                return null;
            }
            // New empty bitmap is filled with values
            recolorBitmap.setPixels(pixels, 0, width, 0, 0, width, height);
            return recolorBitmap;
        } catch (Exception e) {
            throw new Exception("An error occurred while applying the recolor effect.", e);
        }
    }
}

