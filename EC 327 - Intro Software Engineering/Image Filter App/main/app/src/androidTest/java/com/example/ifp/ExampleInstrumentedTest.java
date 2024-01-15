package com.example.ifp;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Color;

import androidx.test.platform.app.InstrumentationRegistry;
import androidx.test.ext.junit.runners.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

import static org.junit.Assert.*;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
public class ExampleInstrumentedTest {
    @Test
    public void useAppContext() {
        // Context of the app under test.
        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();
        assertEquals("com.example.ifp", appContext.getPackageName());
    }

    // Asserts that all pixels of the given colorInt has the given value
    private void assertGreyscale(int expectedValue, int colorInt) {
        assertEquals(expectedValue, Color.red(colorInt));
        assertEquals(expectedValue, Color.green(colorInt));
        assertEquals(expectedValue, Color.blue(colorInt));
    }

    // Asserts that the given colorInt has the given rgb values
    private void assertRGB(int red, int green, int blue, int colorInt) {
        assertEquals(red, Color.red(colorInt));
        assertEquals(green, Color.green(colorInt));
        assertEquals(blue, Color.blue(colorInt));
    }

    @Test
    public void blur_test() {
        final int width = 2;
        final int height = 2;

        Bitmap.Config conf = Bitmap.Config.ARGB_8888;
        Bitmap originalBitmap = Bitmap.createBitmap(width, height, conf);
        Bitmap blurredBitmap;

        // Top left and bottom right pixels are white
        originalBitmap.setPixel(0, 0, Color.rgb(255, 255, 255));
        originalBitmap.setPixel(1, 1, Color.rgb(255, 255, 255));
        // Top Right and bottom left pixels are black
        originalBitmap.setPixel(1, 0, Color.rgb(0, 0, 0));
        originalBitmap.setPixel(0, 1, Color.rgb(0, 0, 0));

        // Blur image
        try {
            blurredBitmap = blurclass.applyBlur(originalBitmap);

            // Check the value of each pixel
            // We expect both the white and black pixels to turn gray
            assertGreyscale(153, blurredBitmap.getPixel(0, 0));
            assertGreyscale(102, blurredBitmap.getPixel(1, 0));
            assertGreyscale(102, blurredBitmap.getPixel(0, 1));
            assertGreyscale(153, blurredBitmap.getPixel(1, 1));
        }
        catch (Exception e) {
            fail("Exception when blurring image:\n" + e.getMessage());
        }
    }

    @Test
    public void greyscale_test() {
        final int width = 2;
        final int height = 2;

        Bitmap.Config conf = Bitmap.Config.ARGB_8888;
        Bitmap originalBitmap = Bitmap.createBitmap(width, height, conf);
        Bitmap greyscaleBitmap;

        // Top left pixel is red
        originalBitmap.setPixel(0, 0, Color.rgb(255, 0, 0));
        // Top right pixel is gray
        originalBitmap.setPixel(1, 0, Color.rgb(80, 80, 80));
        // Bottom left pixel is black
        originalBitmap.setPixel(0, 1, Color.rgb(0, 0, 0));
        // Bottom right pixel is white
        originalBitmap.setPixel(1, 1, Color.rgb(255, 255, 255));

        // Convert to greyscale
        greyscaleBitmap = grayscaleClass.applyGrayscale(originalBitmap);

        // Check the value of each pixel
        assertGreyscale(85, greyscaleBitmap.getPixel(0, 0));
        assertGreyscale(80, greyscaleBitmap.getPixel(1, 0));
        assertGreyscale(0, greyscaleBitmap.getPixel(0, 1));
        assertGreyscale(255, greyscaleBitmap.getPixel(1, 1));
    }

    @Test
    public void histogram_test() {
        final int width = 2;
        final int height = 2;
        int[] histData;

        Bitmap.Config conf = Bitmap.Config.ARGB_8888;
        Bitmap originalBitmap = Bitmap.createBitmap(width, height, conf);

        // Top left pixel is red
        originalBitmap.setPixel(0, 0, Color.rgb(255, 0, 0));
        // Top right pixel is yellow
        originalBitmap.setPixel(1, 0, Color.rgb(255, 255, 0));
        // Bottom left pixel is green
        originalBitmap.setPixel(0, 1, Color.rgb(0, 255, 0));
        // Bottom right pixel is magenta
        originalBitmap.setPixel(1, 1, Color.rgb(255, 0, 255));

        // Get histogram data
        histData = Histogram.getHistData(originalBitmap);

        // Check histogram values

        // Red values
        assertEquals(1, histData[0]);
        assertEquals(3, histData[255]);
        // Green values
        assertEquals(2, histData[0 + 256]);
        assertEquals(2, histData[255 + 256]);
        // Blue values
        assertEquals(3, histData[0 + 512]);
        assertEquals(1, histData[255 + 512]);
    }

    @Test
    public void recolor_test() {
        final int width = 2;
        final int height = 2;

        Bitmap.Config conf = Bitmap.Config.ARGB_8888;
        Bitmap originalBitmap = Bitmap.createBitmap(width, height, conf);
        Bitmap recoloredBitmap;

        // Top left pixel is red
        originalBitmap.setPixel(0, 0, Color.rgb(255, 0, 0));
        // Top right pixel is green
        originalBitmap.setPixel(1, 0, Color.rgb(0, 255, 0));
        // Bottom left pixel is blue
        originalBitmap.setPixel(0, 1, Color.rgb(0, 0, 255));
        // Bottom right pixel is white
        originalBitmap.setPixel(1, 1, Color.rgb(255, 255, 255));

        try {
            // Note: applyRecolor takes values between 0-100
            // Recolor red to yellow
            recoloredBitmap = recolorclass.applyRecolor(originalBitmap, 100, 0, 0, 100, 100, 0);
            // Recolor green to red
            recoloredBitmap = recolorclass.applyRecolor(recoloredBitmap, 0, 100, 0, 100, 0, 0);
            // Recolor blue to magenta
            recoloredBitmap = recolorclass.applyRecolor(recoloredBitmap, 0, 0, 100, 100, 0, 100);
            // Recolor white to black
            recoloredBitmap = recolorclass.applyRecolor(recoloredBitmap, 100, 100, 100, 0, 0, 0);

            // Check the value of each pixel
            assertRGB(255, 255, 0, recoloredBitmap.getPixel(0, 0));
            assertRGB(255, 0, 0, recoloredBitmap.getPixel(1, 0));
            assertRGB(255, 0, 255, recoloredBitmap.getPixel(0, 1));
            assertRGB(0, 0, 0, recoloredBitmap.getPixel(1, 1));

            // Check for value not in the image
            recoloredBitmap = recolorclass.applyRecolor(originalBitmap, 50, 50, 50, 0, 0, 0);
            assertEquals(null, recoloredBitmap);
        }
        catch (Exception e) {
            fail("Exception when recoloring image:\n" + e.getMessage());
        }
    }

    @Test
    public void resize_test() {
        final int width = 8;
        final int height = 8;
        final int newWidth = 2;
        final int newHeight = 2;

        Bitmap.Config conf = Bitmap.Config.ARGB_8888;
        Bitmap originalBitmap = Bitmap.createBitmap(width, height, conf);
        Bitmap resizedBitmap;

        // Create gradient from black to white going from top to bottom
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                float value;

                // Color top left and bottom right corners white
                if (i < width/2 && j < height/2 || i > width/2 && j > height/2) {
                    value = 255;
                }
                // Color bottom left and top right corners black
                else {
                    value = 0;
                }

                originalBitmap.setPixel(i, j, Color.rgb(value, value, value));
            }
        }

        // Resize image to 2x2 pixels
        resizedBitmap = Resize.resizeImage(originalBitmap, newWidth, newHeight);

        // Check size
        assertEquals(newWidth, resizedBitmap.getWidth());
        assertEquals(newHeight, resizedBitmap.getHeight());

        // Check the value of each pixel
        assertRGB(255, 255, 1, resizedBitmap.getPixel(0, 0));
        assertRGB(0, 0, 0, resizedBitmap.getPixel(1, 0));
        assertRGB(0, 0, 0, resizedBitmap.getPixel(0, 1));
        assertRGB(255, 255, 1, resizedBitmap.getPixel(1, 1));
    }

    @Test
    public void rotate_test() {
        final int width = 2;
        final int height = 2;

        Bitmap.Config conf = Bitmap.Config.ARGB_8888;
        Bitmap originalBitmap = Bitmap.createBitmap(width, height, conf);
        Bitmap rotatedBitmap;

        // Top left pixel is red
        originalBitmap.setPixel(0, 0, Color.rgb(255, 0, 0));
        // Top right pixel is green
        originalBitmap.setPixel(1, 0, Color.rgb(0, 255, 0));
        // Bottom left pixel is blue
        originalBitmap.setPixel(0, 1, Color.rgb(0, 0, 255));
        // Bottom right pixel is white
        originalBitmap.setPixel(1, 1, Color.rgb(255, 255, 255));

        // Rotate by 90 degrees
        rotatedBitmap = Rotate.rotateBitmap(originalBitmap, 90);

        // Check the value of each pixel
        assertRGB(0, 0, 255, rotatedBitmap.getPixel(0, 0));
        assertRGB(255, 0, 0, rotatedBitmap.getPixel(1, 0));
        assertRGB(255, 255, 255, rotatedBitmap.getPixel(0, 1));
        assertRGB(0, 255, 0, rotatedBitmap.getPixel(1, 1));

        // Rotate by another 180 degrees
        rotatedBitmap = Rotate.rotateBitmap(rotatedBitmap, 180);

        // Check the value of each pixel
        assertRGB(0, 255, 0, rotatedBitmap.getPixel(0, 0));
        assertRGB(255, 255, 255, rotatedBitmap.getPixel(1, 0));
        assertRGB(255, 0, 0, rotatedBitmap.getPixel(0, 1));
        assertRGB(0, 0, 255, rotatedBitmap.getPixel(1, 1));
    }

    @Test
    public void zeroGB_test() {
        final int width = 2;
        final int height = 2;

        Bitmap.Config conf = Bitmap.Config.ARGB_8888;
        Bitmap originalBitmap = Bitmap.createBitmap(width, height, conf);
        Bitmap zeroGBBitmap;

        // Top left pixel is red
        originalBitmap.setPixel(0, 0, Color.rgb(255, 0, 0));
        // Top right pixel is green
        originalBitmap.setPixel(1, 0, Color.rgb(0, 255, 0));
        // Bottom left pixel is blue
        originalBitmap.setPixel(0, 1, Color.rgb(0, 0, 255));
        // Bottom right pixel is white
        originalBitmap.setPixel(1, 1, Color.rgb(255, 255, 255));

        // Remove green and blue
        zeroGBBitmap = zeroGBclass.removeGreenBlue(originalBitmap);

        // Check the value of each pixel
        assertRGB(255, 0, 0, zeroGBBitmap.getPixel(0, 0));      // Red stays the same
        assertRGB(0, 0, 0, zeroGBBitmap.getPixel(1, 0));        // Green turns into black
        assertRGB(0, 0, 0, zeroGBBitmap.getPixel(0, 1));        // Blue turns into black
        assertRGB(255, 0, 0, zeroGBBitmap.getPixel(1, 1));      // White turns to red
    }
}