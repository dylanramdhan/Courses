from PIL import Image
import os

def split_image_into_tiles(image_path):
    # Open the image file
    img = Image.open(image_path)
    img_width, img_height = img.size

    # Calculate the size of the square side (using the smaller dimension of the image)
    square_side = min(img_width, img_height)

    # Crop the image to a square
    img = img.crop((0, 0, square_side, square_side))

    # Calculate the size of each tile
    tile_size = square_side // 3

    # Create a directory for the results if it doesn't exist
    if not os.path.exists('results'):
        os.makedirs('results')

    # Loop to create tiles and save them, except the bottom-right tile
    tile_number = 1
    for i in range(3):
        for j in range(3):
            if i == 2 and j == 2:  # Skip the bottom-right tile
                continue
            # Define the bounding box for each tile
            left = j * tile_size
            upper = i * tile_size
            right = left + tile_size
            lower = upper + tile_size
            # Crop the tile
            tile = img.crop((left, upper, right, lower))
            # Save the tile
            tile.save(f'results/tile_{tile_number}.png')
            tile_number += 1

# Use the function with the path to your image
split_image_into_tiles('star.png')
