# camera: eye, center, up, fovy, width, height
camera 0 3 8  0 1 0  0 1 0  45  1024 1024

# recursion depth
depth  5

# background color
background 0 0 0

# global ambient light
ambience   0.2 0.2 0.2

# light: position and color
light  20 50 0   0.5 0.5 0.5
light  50 50 50  0.5 0.5 0.5
light -50 50 50  0.5 0.5 0.5

# cylinders: center, radius, axis, height, material
cylinder  -1.5 1.0 0.0  0.5  -1.0 1.0 1.0  1.50      0.8 0.8 0.0  0.8 0.8 0.8  1.0 1.0 1.0   50.0  0.2
cylinder  0.0 1.0 0.0  0.5    0.0 1.0 1.0  1.50      0.8 0.8 0.8  0.8 0.8 0.8  1.0 1.0 1.0   50.0  0.2
cylinder  1.5 1.0 0.0  0.5    1.0 1.0 1.0  1.50      0.8 0.0 0.8  0.8 0.8 0.8  1.0 1.0 1.0   50.0  0.2

# planes: center, normal, material
plane  0 0 0  0 1 0  0.2 0.2 0.2  0.2 0.2 0.2  0.0 0.0 0.0  100.0  0.1
