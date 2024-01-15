package com.example.ifp;

import android.graphics.Bitmap;
import android.graphics.Color;
/**
 * This Java class provides a method named applyBlur that blurs a Bitmap object by averaging
 * the color values of each pixel with its neighboring pixels in a 3x3 grid.
 */

public class blurclass {
    /**
     * The method first checks if the input Bitmap object is null. If it is, an IllegalArgumentException
     * is thrown. The method then creates a new Bitmap object named blurredBitmap with the same
     * width and height as the originalBitmap.The method then gets the color values of each pixel in the
     * originalBitmap and creates a new integer array named blurredPixels with the same length.
     *
     * For each pixel in the originalBitmap, the method iterates through the neighboring pixels in a
     * 3x3 grid around the current pixel. It calculates the average color components for that pixel
     * and assigns the new color to the corresponding pixel in the blurredPixels array.
     * The method skips any neighboring pixels that are out of the image bounds.
     *
     * The method sets the blurredPixels array as the pixels of the blurredBitmap and returns it as bitmap
     * that has the blur effect applied to it
     *
     * @param originalBitmap The Bitmap object to apply the blur effect to.
     * @return A new Bitmap object with the blur effect applied to it.
     * @throws IllegalArgumentException If the input Bitmap object is null.
     * @throws Exception If any other error occurs during the blur process.
     */

    // Apply blur effect to the given bitmap
    public static Bitmap applyBlur(Bitmap originalBitmap) throws Exception {

        if (originalBitmap == null) {
            throw new IllegalArgumentException("Please upload an image first.");
        }

        int width = originalBitmap.getWidth();
        int height = originalBitmap.getHeight();
        Bitmap blurredBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);

        int[] pixels = new int[width * height];
        int[] blurredPixels = new int[width * height];
        originalBitmap.getPixels(pixels, 0, width, 0, 0, width, height);

        // Iterate through each pixel in the image
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                int redTotal = 0, greenTotal = 0, blueTotal = 0, count = 0;

                // Iterate through the neighboring pixels (3x3 grid around the current pixel)
                for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
                    int neighborRow = row + rowOffset;

                    // Skip the row if it is out of the image bounds
                    if (neighborRow < 0 || neighborRow >= height) {
                        continue;
                    }

                    for (int colOffset = -1; colOffset <= 1; colOffset++) {
                        int neighborCol = col + colOffset;

                        // Skip the column if it is out of the image bounds
                        if (neighborCol < 0 || neighborCol >= width) {
                            continue;
                        }

                        // Get the color components of the neighboring pixel
                        int neighborPixelIndex = neighborRow * width + neighborCol;
                        int neighborPixel = pixels[neighborPixelIndex];
                        int red = Color.red(neighborPixel);
                        int green = Color.green(neighborPixel);
                        int blue = Color.blue(neighborPixel);

                        // Add the color components to their respective totals
                        redTotal += red;
                        greenTotal += green;
                        blueTotal += blue;
                        count++;
                    }
                }

                // Calculate the average color components for the current pixel
                int pixelIndex = row * width + col;
                int pixel = pixels[pixelIndex];
                int red = Color.red(pixel);
                int green = Color.green(pixel);
                int blue = Color.blue(pixel);

                int newRed = (redTotal + red) / (count + 1);
                int newGreen = (greenTotal + green) / (count + 1);
                int newBlue = (blueTotal + blue) / (count + 1);

                // Assign the new color to the corresponding pixel in the blurredPixels array
                blurredPixels[pixelIndex] = Color.rgb(newRed, newGreen, newBlue);
            }
        }

        // Set the blurredPixels array as the pixels of the blurredBitmap and return it
        blurredBitmap.setPixels(blurredPixels, 0, width, 0, 0, width, height);
        return blurredBitmap;
    }
}

