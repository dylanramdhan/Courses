package com.example.ifp;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.Paint;

/**
 * This class handles the grayscale feature within our app. It includes one method which handles the logic for
 * the grayscale algorithm.
 * **/


public class grayscaleClass {
    /**
     * This method first checks if there is anything mapped onto the originalBitmap parameter and returns null. Given that
     * there is an image mapped onto the bitmap, the algorithm works by taking the average value of the RGB channel of the
     * current pixel and passes it into the Color.rgb method which handles the recoloring of the bitmap image.
     *
     * @param originalBitmap
     * @return grayscaleBitmap
     */
    public static Bitmap applyGrayscale(Bitmap originalBitmap) {

        Bitmap grayscaleBitmap = Bitmap.createBitmap(originalBitmap.getWidth(), originalBitmap.getHeight(), Bitmap.Config.ARGB_8888);

        for (int x = 0; x < originalBitmap.getWidth(); x++) {
            for (int y = 0; y < originalBitmap.getHeight(); y++) {
                int pixel = originalBitmap.getPixel(x, y);
                int red = Color.red(pixel);
                int green = Color.green(pixel);
                int blue = Color.blue(pixel);

                // Calculate the average of the RGB values to get the grayscale value}
                int gray = (red + green + blue) / 3;

                grayscaleBitmap.setPixel(x, y, Color.rgb(gray, gray, gray));
            }
        }

        return grayscaleBitmap;
    }
}

