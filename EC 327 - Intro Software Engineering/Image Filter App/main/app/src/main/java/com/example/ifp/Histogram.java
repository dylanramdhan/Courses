package com.example.ifp;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.Log;
import android.widget.ImageView;

public class Histogram {
    private static final String TAG = "histogram";

    public static int[] getHistData(Bitmap originalBitmap) {
        /**
         * histogram class extracts image data in the form of bitmaps from the imageView. The function
         * then loops through the pixels of the image and increment to an array to the corresponding
         * color value. After Normalizing this value, histogram bar charts are plotted with their
         * respective colors onto a canvas that can be viewed at another imageView that is
         * designated for histograms.
         *
         */

        // Get bitmap from ImageView
        int width = originalBitmap.getWidth();
        int height = originalBitmap.getHeight();

        // Create an array to store histogram data
        int[] histData = new int[256 * 3];

        // Looping through each pixel and increment to the corresponding color value
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                int pixel = originalBitmap.getPixel(x, y);

                int redValue = Color.red(pixel);
                int greenValue = Color.green(pixel);
                int blueValue = Color.blue(pixel);

                // Increment the histogram data for each color
                histData[redValue]++;
                histData[greenValue + 256]++;
                histData[blueValue + 512]++;
            }
        }

        return histData;
    }

    public static Bitmap generateHistogram(Bitmap originalBitmap, ImageView histView) {
        // Create an array to store histogram data
        int[] histData = getHistData(originalBitmap);

        // Determine the max value in histData for normalization
        int maxVal = 0;
        for (int i = 0; i < 256 * 3; i++){
            if (histData[i] > maxVal){
                maxVal = histData[i];

            }
        }

        Log.d(TAG,"foo 2");
        // Create a canvas to draw histogram on
        Bitmap canvasBitmap = Bitmap.createBitmap(histView.getWidth(), histView.getHeight(), Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(canvasBitmap);
        Paint paint = new Paint();
        paint.setStyle(Paint.Style.FILL);


        // Set colors for the histogram bars
        int[] barColors = new int[] {Color.RED, Color.GREEN, Color.BLUE};

        // Draw histogram bars
        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < 3; j++) {
                int count = histData[i + j * 256];
                float barHeight = (float) count / maxVal * canvas.getHeight();
                float barLeft = i * canvas.getWidth() / 256f + j * canvas.getWidth() / 3f;
                float barTop = canvas.getHeight() - barHeight;
                float barRight = barLeft + canvas.getWidth() / 3f / 256f;
                float barBottom = canvas.getHeight();
                paint.setColor(barColors[j]);
                canvas.drawRect(barLeft, barTop, barRight, barBottom, paint);

            }
        }
        Log.d(TAG,"foo 3");

        return canvasBitmap;
    }
}

