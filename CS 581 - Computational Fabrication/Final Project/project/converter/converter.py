## cs581 phone case
# converting user's PNG to STL file


# import numpy as np
# from stl import mesh

# # Define the 8 vertices of the cube
# vertices = np.array([\
#     [-1, -1, -1],
#     [+1, -1, -1],
#     [+1, +1, -1],
#     [-1, +1, -1],
#     [-1, -1, +1],
#     [+1, -1, +1],
#     [+1, +1, +1],
#     [-1, +1, +1]])
# # Define the 12 triangles composing the cube
# faces = np.array([\
#     [0,3,1],
#     [1,3,2],
#     [0,4,7],
#     [0,7,3],
#     [4,5,6],
#     [4,6,7],
#     [5,1,2],
#     [5,2,6],
#     [2,3,6],
#     [3,7,6],
#     [0,1,5],
#     [0,5,4]])

# # Create the mesh
# cube = mesh.Mesh(np.zeros(faces.shape[0], dtype=mesh.Mesh.dtype))
# for i, f in enumerate(faces):
#     for j in range(3):
#         print(vertices[f[j],:])
#         cube.vectors[i][j] = vertices[f[j]]

# # Write the mesh to file "cube.stl"
# cube.save('cube.stl')










from PIL import Image
import numpy as np
from stl import mesh


def 




def image_to_stl(image_path, stl_path, scale=1, height=10):
    # Load image file
    image = Image.open(image_path).convert('L')  # convert to grayscale
    data = np.array(image)

    # Normalize data to 0-1 range
    data = data / 255
    
    # Create a grid of x, y coordinates
    nx, ny = data.shape
    x = np.linspace(0, scale * nx, num=nx)
    y = np.linspace(0, scale * ny, num=ny)
    x, y = np.meshgrid(x, y)

    # Create vertices for the mesh by stacking coordinates
    vertices = np.dstack([x, y, data * height])

    # Create the faces of the mesh
    faces = []
    for i in range(ny - 1):
        for j in range(nx - 1):
            v0 = i * nx + j
            v1 = i * nx + j + 1
            v2 = (i + 1) * nx + j + 1
            v3 = (i + 1) * nx + j
            faces.append([v0, v1, v2])
            faces.append([v0, v2, v3])

    # Create the mesh
    cube = mesh.Mesh(np.zeros(len(faces), dtype=mesh.Mesh.dtype))
    for i, f in enumerate(faces):
        for j in range(3):
            cube.vectors[i][j] = vertices[f[j] // nx, f[j] % nx]

    # Save to file
    cube.save(stl_path)

# Example usage
image_to_stl('path_to_image.png', 'output.stl', scale=0.1, height=20)
