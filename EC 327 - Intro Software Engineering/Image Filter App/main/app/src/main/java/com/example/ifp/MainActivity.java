package com.example.ifp;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Scanner;

/**
 * This class represents the main activity of an Android app that provides various image processing features.
 */


public class MainActivity extends AppCompatActivity{
    // When declaring the components we want to specify the type
    // from xml file and the corresponding string.
    // Declare the instance variables that will be used to reference the UI components
    private ImageView dispimage;
    private ImageView histogramView;
    private TextView angle;
    private TextView edit_height;
    private TextView edit_width;
    private Button histogram;
    private Button help;
    private TextView target_R;
    private TextView target_G;
    private TextView target_B;
    private TextView new_R;
    private TextView new_G;
    private TextView new_B;
    private static final int PICK_FILE_REQUEST_CODE = 1;
    private static final int READ_REQUEST_CODE = 42;
    /**
     * Called when the activity is created. This method initializes the UI components, sets up click listeners for buttons,
     * and defines the logic for image processing operations.
     *
     * @param savedInstanceState a Bundle object containing the activity's previously saved state, or null if no state has been saved yet
     */


    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //Component Tree:
        Button uploadimage = (Button) findViewById(R.id.uploadimage);
        Button uploadtext = (Button) findViewById(R.id.uploadtext);
        Button resize = (Button) findViewById(R.id.resize);
        Button rotate = (Button) findViewById(R.id.rotate);
        dispimage = (ImageView) findViewById(R.id.dispimage);
        Button blur = (Button) findViewById(R.id.imageblur);
        Button zeroGB = (Button) findViewById(R.id.zeroGB);
        angle = (EditText) findViewById(R.id.angle);
        Button grayscale = (Button) findViewById(R.id.grayscale);
        edit_height = (EditText) findViewById(R.id.edit_height);
        edit_width = (EditText) findViewById(R.id.edit_width);
        histogram = (Button) findViewById(R.id.histogram);
        histogramView = (ImageView) findViewById(R.id.histogramView);
        Button recolor = (Button) findViewById(R.id.recolor);
        target_R = (EditText) findViewById(R.id.target_R);
        target_G = (EditText) findViewById(R.id.target_G);
        target_B = (EditText) findViewById(R.id.target_B);
        new_R = (EditText) findViewById(R.id.new_R);
        new_G = (EditText) findViewById(R.id.new_G);
        new_B = (EditText) findViewById(R.id.new_B);
        help = (Button) findViewById(R.id.help);

        uploadimage.setOnClickListener(new View.OnClickListener() {
            /**
             Called when the "Upload Image" button is clicked. This method launches an intent to open a file picker
             so that the user can select an image from the device's storage.

             The onClick method is called when the "Upload Image" button is clicked. This method launches an
             intent to open a file picker so that the user can select an image from the device's storage.
             @param v The view that was clicked

             */


            // Logic that enables the user to click the Upload Image button to access the device storage
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
                intent.setType("*/*"); //set MIME type to filter files
                startActivityForResult(Intent.createChooser(intent, "Select File"), PICK_FILE_REQUEST_CODE);
            }
        });

        resize.setOnClickListener(new View.OnClickListener() {
            /**

             Sets an onClickListener for the resize button, which resizes the displayed image based on user input.

             If the width or height fields are empty, a toast is displayed prompting the user to enter values.

             @param v The View object representing the resize button
             */


            @Override
            public void onClick(View v) {
                // Check if width or height fields are empty
                if (edit_width.getText().toString().trim().isEmpty() || edit_height.getText().toString().trim().isEmpty()) {
                    // Prompt user to enter values
                    Toast.makeText(getApplicationContext(), "Please enter a value for both width and height.", Toast.LENGTH_SHORT).show();
                    return;
                }
                // Get the original bitmap from the displayed image
                Bitmap originalBitmap = ((BitmapDrawable) dispimage.getDrawable()).getBitmap();
                int newWidth = Integer.parseInt(edit_width.getText().toString());
                int newHeight = Integer.parseInt(edit_height.getText().toString());

                // Resize the bitmap to the desired dimensions
                Bitmap resizedBitmap = Resize.resizeImage(originalBitmap, newWidth, newHeight);

                // Update the ImageView with the resized bitmap
                dispimage.setImageBitmap(resizedBitmap);

            }
        });

        uploadtext.setOnClickListener(new View.OnClickListener() {
            /**
             *
             Sets an OnClickListener for the uploadtext button that opens a file picker activity to select a file to upload.

             Called when the "uploadtext" button is clicked.

             @param v the View that was clicked
             */
            @Override
            public void onClick(View v) {
                // Start file picker activity
                Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
                intent.setType("*/*");
                startActivityForResult(intent, READ_REQUEST_CODE);
            }

        });

        rotate.setOnClickListener(new View.OnClickListener() {
            /**

             Sets an OnClickListener for the rotate button that will rotate the displayed image by the angle provided by the user.

             If the angle field is empty, a toast message will be displayed to prompt the user to enter a value.

             @param v the image rotated by the angle
             */

            @Override
            public void onClick(View v) {
                if (angle.getText().toString().isEmpty()) {
                    // Display a toast message
                    Toast.makeText(MainActivity.this, "Field cannot be empty!!", Toast.LENGTH_SHORT).show();
                    return;
                }
                // Get the rotation angle
                float mAngleRotate = Float.parseFloat(angle.getText().toString());

                // Load the original bitmap image
                Bitmap originalBitmap = ((BitmapDrawable) dispimage.getDrawable()).getBitmap();

                // Create a rotated bitmap
                Bitmap rotatedBitmap = Rotate.rotateBitmap(originalBitmap, mAngleRotate);
                // Display the rotated image
                dispimage.setImageBitmap(rotatedBitmap);
            }
        });

        zeroGB.setOnClickListener(new View.OnClickListener() {
            /**

             An OnClickListener to handle the event when the "Zero GB" button is clicked.

             This class is responsible for removing the green and blue color components from the loaded image.

             @param v the View that was clicked
             */

            @Override
            public void onClick(View v) {
                Drawable drawable = dispimage.getDrawable();

                // Check if the original bitmap is not null
                if (drawable != null) {
                    Bitmap originalBitmap = ((BitmapDrawable) dispimage.getDrawable()).getBitmap();
                    try {
                        // Use ZeroGBClass to remove green and blue components
                        Bitmap zeroedBitmap = zeroGBclass.removeGreenBlue(originalBitmap);

                        // Update the ImageView/original image with the recolored bitmap
                        dispimage.setImageBitmap(zeroedBitmap);
                        originalBitmap = zeroedBitmap;

                    } catch (Exception e) {
                        // Display a message if an error occurs while processing the image
                        Toast.makeText(getApplicationContext(), "An error occurred while processing the image.", Toast.LENGTH_SHORT).show();
                    }
                } else {
                    // Display a message if the original bitmap is null
                    Toast.makeText(getApplicationContext(), "Please upload an image first.", Toast.LENGTH_SHORT).show();
                }
            }
        });

        blur.setOnClickListener(new View.OnClickListener() {
            /**

             Sets an onClickListener on the "blur" button to apply a blur effect to an image.
             The onClick method is called when the "blur" button is clicked. It applies a blur effect to
             the image displayed in the ImageView.
             @param v The view that was clicked
             */

            // Image blur algorithm
            @Override
            public void onClick(View v) {
                Drawable drawable = dispimage.getDrawable();

                // Check if the drawable is not null
                if (drawable != null) {
                    Bitmap originalBitmap = ((BitmapDrawable) drawable).getBitmap();

                    try {
                        // Use BlurClass to apply blur
                        Bitmap blurredBitmap = blurclass.applyBlur(originalBitmap);

                        // Update the ImageView with the blurred bitmap
                        dispimage.setImageBitmap(blurredBitmap);
                        originalBitmap = blurredBitmap;
                    } catch (Exception e) {
                        // Display an error message if an exception occurs during the blurring process
                        Toast.makeText(getApplicationContext(), "An error occurred while applying the blur effect.", Toast.LENGTH_SHORT).show();
                    }
                } else {
                    // Display a message if the drawable is null
                    Toast.makeText(getApplicationContext(), "Please upload an image first.", Toast.LENGTH_SHORT).show();
                }
            }
        });

        histogram.setOnClickListener(new View.OnClickListener() {
            /**

             Sets an onClickListener on the "histogram" button to generate and display a histogram of the image.

             The onClick method is called when the "histogram" button is clicked. It generates a histogram
             of the image displayed in the ImageView and displays it in a separate ImageView.

             @param v The view that was clicked
             */

            @Override
            public void onClick(View v)
            {
                // Get bitmap from ImageView
                Bitmap originalBitmap = ((BitmapDrawable) dispimage.getDrawable()).getBitmap();
                // Get image view from layout
                ImageView histView = findViewById(R.id.histogramView);
                //Generate a histogram
                Bitmap canvasBitmap = Histogram.generateHistogram(originalBitmap, histView);
                histogramView.setImageBitmap(canvasBitmap);
            }
        });

        help.setOnClickListener(new View.OnClickListener() {
            /**

             Sets an OnClickListener for the "Help" button that displays an AlertDialog with instructions.

             Instructions include usage of buttons and their functionalities.
             */
            @Override
            public void onClick(View v) {
                AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
                builder.setTitle("Instructions");

                // Create a ScrollView and add a TextView to it
                ScrollView scrollView = new ScrollView(MainActivity.this);
                TextView textView = new TextView(MainActivity.this);
                textView.setText("Usage of Buttons: \n" +
                        "\n" +
                        "1) Upload Image = Make sure to drag an image into the Downloads library before clicking onto the button. \n" +
                        "\n" +
                        "2) Upload Text = Make sure the text file is in the Downloads folder before clicking onto the button. \n" +
                        "\n" +
                        "3) ZeroGB = Turns an image red from zeroing G and B pixels. \n" +
                        "\n" +
                        "4) Histogram = Display a histogram chart of RGB values present in the pixels of the image. \n" +
                        "\n" +
                        "5) Blurring = Increasing blurring intensity by clicking onto the button multiple times. \n" +
                        "\n" +
                        "6) Grayscale = Converts image to Grayscale. \n" +
                        "\n" +
                        "7) Resize = Enter specified resizing factor in \"Edit Width\" & \"Edit Height\" to resize the image.\n" +
                        "\n" +
                        "8) Recolor = Enter target color to be changed at \"targetR\", \"targetG\", and \"targetB\" box. Enter the new color at \"newR\", \"newG\", and \"newB\" box (must be 0 to 100 inclusive).\n" +
                        "\n" +
                        "9) Rotate =  Enter angle in \"Edit Angle\" box.\n" +
                        "\n" +
                        "For more info, please refer to README.md or Group6_IFP slide\n");
                textView.setPadding(30, 30, 30, 30);
                scrollView.addView(textView);

                // Set the custom view to the dialog
                builder.setView(scrollView);

                // Add the OK button and show the dialog
                builder.setPositiveButton("OK", null);
                AlertDialog dialog = builder.create();
                dialog.show();
            }
        });

        recolor.setOnClickListener(new View.OnClickListener() {
            /**
             Sets an OnClickListener on the "recolor" button to perform recoloring on the selected image.

             This method checks if all required input fields are filled, validates the RGB values entered by the user,

             and uses the RecolorClass to apply the recoloring to the image. If a recolored bitmap is returned, the

             ImageView is updated to display the new image. If the original bitmap is null, a message is displayed to

             prompt the user to upload an image. If any errors occur during the recoloring process, an error message is

             displayed.

             @param v the View to which the OnClickListener is being set
             */

            @Override
            public void onClick(View v) {
                // Check for empty input fields
                if (TextUtils.isEmpty(target_R.getText().toString()) || TextUtils.isEmpty(target_G.getText().toString()) || TextUtils.isEmpty(target_B.getText().toString()) ||
                        TextUtils.isEmpty(new_R.getText().toString()) || TextUtils.isEmpty(new_G.getText().toString()) || TextUtils.isEmpty(new_B.getText().toString())) {
                    Toast.makeText(getApplicationContext(), "Please fill in all input fields.", Toast.LENGTH_SHORT).show();
                    return;
                }

                try {
                    int targetR = Math.round(Float.parseFloat(target_R.getText().toString()));
                    int targetG = Math.round(Float.parseFloat(target_G.getText().toString()));
                    int targetB = Math.round(Float.parseFloat(target_B.getText().toString()));
                    int newR = Math.round(Float.parseFloat(new_R.getText().toString()));
                    int newG = Math.round(Float.parseFloat(new_G.getText().toString()));
                    int newB = Math.round(Float.parseFloat(new_B.getText().toString()));

                    if (targetR < 0 || targetR > 100 || targetG < 0 || targetG > 100 || targetB < 0 || targetB > 100 ||
                            newR < 0 || newR > 100 || newG < 0 || newG > 100 || newB < 0 || newB > 100) {
                        Toast.makeText(getApplicationContext(), "RGB values range from 0 to 100, try again.", Toast.LENGTH_SHORT).show();
                        return;
                    }

                    Drawable drawable = dispimage.getDrawable();

                    // Check if the original bitmap is not null
                    if (drawable != null) {
                        Bitmap originalBitmap = ((BitmapDrawable) dispimage.getDrawable()).getBitmap();
                        try {
                            // Use RecolorClass to apply recoloring
                            Bitmap recolorBitmap = recolorclass.applyRecolor(originalBitmap, targetR, targetG, targetB, newR, newG, newB);

                            // If a recolored bitmap is returned, update the ImageView & replace the original image
                            if (recolorBitmap != null) {
                                dispimage.setImageBitmap(recolorBitmap);
                                originalBitmap = recolorBitmap;
                            } else {
                                Toast.makeText(getApplicationContext(), "Target color not found in the image.", Toast.LENGTH_SHORT).show();
                            }
                        } catch (Exception e) {
                            Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_SHORT).show();
                        }
                    } else {
                        // Display a message if the original bitmap is null
                        Toast.makeText(getApplicationContext(), "Please upload an image first.", Toast.LENGTH_SHORT).show();
                    }
                } catch (NumberFormatException e) {
                    Toast.makeText(getApplicationContext(), "Please enter valid numeric values.", Toast.LENGTH_SHORT).show();
                }
            }
        });

        grayscale.setOnClickListener(new View.OnClickListener() {
            /**
             The onClick method is called when the "grayscale" button is clicked. It applies a grayscale effect

             to the image displayed in the ImageView.

             @param v The view that was clicked
             */

            @Override
            public void onClick(View v) {
                Drawable drawable = dispimage.getDrawable();

                // Check if originalBitmap is not null
                if (drawable != null) {
                    Bitmap originalBitmap = ((BitmapDrawable) dispimage.getDrawable()).getBitmap();
                    // Use GrayscaleClass to apply the grayscale effect
                    Bitmap grayscaleBitmap = grayscaleClass.applyGrayscale(originalBitmap);
                    originalBitmap = grayscaleBitmap;

                    // Check if grayscaleBitmap is not null
                    if (grayscaleBitmap != null) {
                        // Update the ImageView with the grayscale bitmap
                        dispimage.setImageBitmap(grayscaleBitmap);
                    } else {
                        Toast.makeText(getApplicationContext(), "An error occurred while applying the grayscale effect.", Toast.LENGTH_SHORT).show();
                    }
                } else {
                    Toast.makeText(getApplicationContext(), "Please upload an image first.", Toast.LENGTH_SHORT).show();
                }
            }

        });
    }

    /**

     This method is called after an activity has completed and returns data.

     It handles displaying the user uploaded image onto the ImageView template, and converts a text file

     to an image if it is in the correct format.

     @param requestCode The integer request code originally supplied to startActivityForResult().

     @param resultCode The integer result code returned by the child activity through its setResult().

     @param data An Intent, which can return result data to the caller.
     */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        // Logic that displays the user uploaded image onto the ImageView template
        if (requestCode == PICK_FILE_REQUEST_CODE && resultCode == RESULT_OK) {
            Uri uri = data.getData();
            try {
                Bitmap bitmap = MediaStore.Images.Media.getBitmap(this.getContentResolver(), uri);
                dispimage.setImageBitmap(bitmap);
            } catch (IOException e) {
                e.printStackTrace();
            }
        } else if (requestCode == READ_REQUEST_CODE && resultCode == RESULT_OK) {
            Uri uri = null;
            int count = 0;
            if (data != null) {
                uri = data.getData();
                Log.i("TAG", "Uri: " + uri.toString());
            }

            if (uri != null) {
                //Check if the uploaded file is in the right format or not
                try {
                    InputStream inputStream = getContentResolver().openInputStream(uri);
                    BufferedReader br = new BufferedReader(new InputStreamReader(inputStream));
                    String line = br.readLine();

                    try {
                        int width = Integer.parseInt(line.trim());
                        int height = Integer.parseInt(br.readLine().trim());

                        br.mark(1000);

                        if (width <= 0 || height <= 0) {
                            count += 1; // one error
                        }

                        int expectedNumLines = width * height;
                        int currentNumLines = 0;
                        while ((line = br.readLine()) != null) {
                            currentNumLines += 1;
                        }
                        if (expectedNumLines != currentNumLines) {
                            count += 1; //one error
                        }

                        br.reset();

                        while ((line = br.readLine()) != null) {
                            String[] tokens = line.split("\\s+");
                            if (tokens.length != 3) {
                                count += 1; //one error
                            }
                            for (String token : tokens) {
                                int value = Integer.parseInt(token);
                                if (value < 0 || value > 100) {
                                    count += 1; // one error
                                }
                            }
                        }
                    } catch (NumberFormatException e) {
                        Toast.makeText(MainActivity.this, "Wrong file format", Toast.LENGTH_SHORT).show();
                        return;
                    }
                    br.close();
                    inputStream.close();
                } catch (IOException e) {
                    Log.e("MainActivity", "Unable to read file", e);
                }
            }
            //If the format is correct, it will convert the text file to image
            if (count == 0) {
                try {
                    // read the image data from the file
                    Scanner scanner = new Scanner(getContentResolver().openInputStream(uri));

                    // read the dimensions of the image
                    int width = scanner.nextInt();
                    int height = scanner.nextInt();

                    // create a Bitmap to store the pixel data
                    Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);

                    // read in the pixel data
                    for (int y = 0; y < height; y++) {
                        for (int x = 0; x < width; x++) {
                            int r = scanner.nextInt() * 255 / 100;
                            int g = scanner.nextInt() * 255 / 100;
                            int b = scanner.nextInt() * 255 / 100;
                            int rgb = Color.rgb(r, g, b);
                            bitmap.setPixel(x, y, rgb);
                        }
                    }

                    ImageView dispimage = findViewById(R.id.dispimage);
                    dispimage.setImageBitmap(bitmap);

                } catch (IOException e) {
                    Log.e("TAG", "Error reading file", e);
                }
            } else {
            }
        }
    }
}
