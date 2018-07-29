# camera: eye, center, up, fovy, width, height
camera 1 3 8  1 1 0  0 1 0  45  500 500

# recursion depth
depth  5

# background color
background 0 0 0

# global ambient light
ambience   0.2 0.2 0.2

# light: position and color
light  0  50 0   0.35 0.35 0.35
light  50 50 50  0.35 0.35 0.35
light -50 50 50  0.35 0.35 0.35

# spheres: center, radius, material
sphere  0.0 1.0 0.0  1.0  1.0 0.0 0.0  1.0 0.0 0.0  1.0 1.0 1.0  100.0  0.4 
sphere -1.0 0.5 2.0  0.5  0.0 1.0 0.0  0.0 1.0 0.0  1.0 1.0 1.0  200.0  0.2
sphere  3.0 2.0 1.5  2.0  0.0 0.0 1.0  0.0 0.0 1.0  1.0 1.0 1.0   50.0  0.2

# planes: center, normal, material
plane  0 0 0  0 1 0  0.2 0.2 0.2  0.2 0.2 0.2  0.0 0.0 0.0  100.0  0.1
