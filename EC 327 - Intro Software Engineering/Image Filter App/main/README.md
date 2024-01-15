# Group6Project

# Summary
## Category
Group6: Image Filtering

## Brief Description
A user friendly app that enables user to upload and modify images. 

## Longer Description 
A user friendly app that enables user to upload and modify images. This includes: 
1. Uploading an image from library or from a text file.
2. Applying various filters: Blurring and Grayscale.
3. Rotate image at a specified angle from user.
4. Adjusting size of image according to user-specified factor.
5. Zeroing green and blue pixels of an image to give a green image.
6. Recoloring one color to another color of an image.
7. Displaying histogram of RGB values of an image.

# Authors
## Group Members
- WikiName, BU email address
- EllenBurhansjah, eburhan@bu.edu
- DanielliaSumigar, dsumigar@bu.edu
- MinhNguyen, minhtn@bu.edu
- XinglinHe, hxl@bu.edu
- IanLee, ianxlee@bu.edu
- DylanRamdhan, dylram01@bu.edu
- AurojitChakraborty, aurojit@bu.edu


## Roles
Everyone contributes equally for each roles. Thus, everyone shares an equal percentage of 14.29%


# Accomplishments
## Minimum Requirements [50%]
The following are our minimum requirements that are **completed entirely**.

**1) Application should be able to upload an image, reading from a text file with the following format:**
>
[WIDTH]
[HEIGHT]

[PIXEL_0_0]
[PIXEL_0_1]
[PIXEL_0_2]

[PIXEL_0_[HEIGHT]-1]
[PIXEL_1_0]
[PIXEL_1_1]
[PIXEL_1_2]

[PIXEL_1_[HEIGHT]-1]

[PIXEL_[WIDTH]-1_0]
[PIXEL_[WIDTH]-1_1]
[PIXEL_[WIDTH]-1_2]
>
Where:
- [WIDTH] - is the width of the image, in pixels
- [HEIGHT] - is the height of the image, in pixels
- [PIXEL_X_Y] represents the pixel at coordinates (X,Y), expressed as R_X_Y, G_X_Y, B_X_Y where:
- R_X_Y is the red content of the pixel, expressed as an integer between 0 and 100, inclusive.
- G_X_Y is the green content of the pixel, expressed as an integer between 0 and 100, inclusive.
- B_X_Y is the blue content of the pixel, expressed as an integer between 0 and 100, inclusive.

This minimum requirement has been accomplished and can be tested by clicking on the Upload Text button. 

**2) Recolor uploaded image by zero-ing out all green and blue components of pixels (resulting in a red image).**

This minimum requirement has been accomplished and can be tested by clicking on the zeroGB button.



## Possible Features
The following are our implemented possible features that are **completed entirely**.

**1) Resize [5%]**
+ Resize button allows users to grow or shrink an image according to a specified factor.

**2) Recolor [5%]**
+ Recolor button that allows user to recolor any one color in the image with a different color.

**3) Histogram [10%]**
+ Histogram button that allows user to display a histogram chart of RGB values present in the pixels of the image.

**4) Blurring [10%]**
+ Blur button that allows user to blur an image with the option of increasing blurring intensity by clicking onto the blur button multiple times.

**5) Grayscale [10%]**
+ Grayscale button that allows user to convert image to Grayscale by replacing each pixel color component by the average of its Red, Green, and Blue components.
___
The following is our implemented possible features that is **completed partially**.

**1) Rotate [5%]**
+ Rotate button that allows user to rotate an image by a user-specified angle in degree.
+ **Challenges**: Whenever the user clicks onto the rotate button multiple times, the rotated image will decrease in size upon each clicks. We suspect that the problem lies upon how whenever the rotated image goes beyond the range of the imageView, the image gets automatically modified; hence, decreasing in size for each rotation.



# Execution
## Project Source
Link to the repo:
https://agile.bu.edu/gitlab/ec327_projects/group6project.git

## Installation
1) Click onto the repo link to clone required files onto the computer.
2) Open AndroidStudio and open the cloned folder.
3) Allow build.gradle to complete and configure the device to be Pixel 2A API 30 (Create Device > Pixel 2 > R).
4) Click on the run button to see the App in actionn.

## Usage
The app has a scrolling layout for a better user experience. Uploaded image and modifications to that image can be seen directly at the imageView. Features **1) to 6)** can be tested with a click of a Button, labelled with their respective names. Meanwhile, features **6) - 9)** requires user-specified input that can be entered in the text box.

**1) Upload Image**
+ Upload Image button allows user to upload an image that is stored from their library. 
+ When executed in the emulator, the user has to first drag their desired image onto the emulator. To upload this image, the next step would be to click onto the Upload Image button before selecting the previous dragged image, that has been stored in the Downloads folder.

**2) Upload Text**
+ Usage is similar to **1)** except the user chooses to upload an image based on a text file.

**3) ZeroGB**
+ ZeroGB button that when clicked zeros out green and blue pixels, turning an image red.

**4) Histogram**
+ Histogram button that allows user to display a histogram chart of RGB values present in the pixels of the image.

**5) Blurring**
+ Blur button that allows user to blur an image.
    - **Note**: The user has the option of increasing blurring intensity by clicking onto the blur button multiple times.

**6) Grayscale**
+ Grayscale button that allows user to convert image to Grayscale.

**7) Resize**
+ Resize button allows users to grow or shrink an image according to specified factor.
    - **Note**: 
        - Before clicking onto the Resize button, user enters their specified resizing factor in a textbox, labelled,"Edit Width" & "Edit Height".
        - It is important to note that when a user shrinks an image, it will appear blurred. This is because shrunken image has been compressed and stretched to fit the dimensions of the imageView.

**8) Recolor**
+ Recolor button that allows user to recolor any one color in the uploaded image with a different color.
    - **Note**: Before clicking onto the Recolor button, user enters their target color to be changed at "targetR", "targetG", and "targetB" textbox. The new color will be at "newR", "newG", and "newB" text box (numbers has to be 0 to 100 inclusive, according to the format of input text file).

**9) Rotate**
+ Rotate button that allows user to rotate an image by a user-specified angle.
    - **Note**: 
        - Before clicking onto the Rotate button, user enters their specified angle in a text box, labelled "Edit Angle".
        - Whenever the user clicks onto the rotate button multiple times, the rotated image will decrease in size upon each clicks. We suspect that the problem lies upon how whenever the rotated image goes beyond the range of the imageView, the image gets automatically modified; hence, decreasing in size for each rotation. 


# Miscellaneous
## Extra Features
**1) Upload image button**
- button that allows user to upload image from emulator's library.


## Challenges
**1) Rotate button**
    - Whenever the user clicks onto the rotate button multiple times, the rotated image will decrease in size upon each clicks. We suspect that the problem lies upon how whenever the rotated image goes beyond the range of the imageView, the image gets automatically modified; hence, decreasing in size for each rotation.

# Supporting Material
Please refer to slide: Group6_IFP for App instructions with images (also located in Repo).

# Release
All group members agree to make project public.


