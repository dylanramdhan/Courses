package com.example.ifp;

import android.graphics.Bitmap;
import android.graphics.Matrix;
/** In this java class, rotateclass, the class utilizes the 'Bitmap' class to assist
 *   in the rotation of the image file. In the implementation of the class, the rotation
 *   feature utilizes the 'Bitmap' to allow the use to specify the particular angle that
 *   the user choice.
 */
public class Rotate {
    /**
     *  In this class, the method takes the uploaded image and rotates the image the
     *  user's inputted angle. The file first creates a matrix, and then rotates the
     *  newly created matrix to the user's inputted angle. After this is then applied
     *  the interface of the ImageView.
     *
     * @param originalBitmap
     * @return rotatedBitmap
     */
    public static Bitmap rotateBitmap(Bitmap originalBitmap, float mAngleRotate) {

        // Create a matrix and apply the rotation
        Matrix matrix = new Matrix();
        matrix.postRotate(mAngleRotate);

        // Create a rotated bitmap
        Bitmap rotatedBitmap = Bitmap.createBitmap(originalBitmap, 0, 0, originalBitmap.getWidth(), originalBitmap.getHeight(), matrix, true);
        // Set the rotated bitmap to the image view
        return rotatedBitmap;
        }
}

