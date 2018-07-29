# camera: eye, center, up, fovy, width, height
camera 5 2 5  0 0 0  0 1 0  45  500 500

# recursion depth
depth  1

# background color
background 1 1 1

# global ambient light
ambience   0.2 0.2 0.2

# light: position and color
light -1 10 -1  1.0 0.2 0.2
light 10 0 5    0.2 0.2 1.0

# mesh: filename, FLAT/SMOOTH, material
mesh cube.off FLAT  0.5 0.5 0.5  0.5 0.5 0.5  0.0 0.0 0.0  0.0  0.0
#mesh cube.off PHONG  0.5 0.5 0.5  0.5 0.5 0.5  0.0 0.0 0.0  0.0  0.0
