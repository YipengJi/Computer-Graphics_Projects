# camera: eye, center, up, fovy, width, height
camera 0 0 10  7 0 0  0 1 0  45  1000 400

# recursion depth
depth  20

# background color
background 0 0 0

# global ambient light
ambience   0.2 0.2 0.2

# light: position and color
light  0  0 10   0.5 0.5 0.5
#light  50 50 50  0.35 0.35 0.35
#light -50 50 50  0.35 0.35 0.35

# spheres: center, radius, material
sphere  0.0 0.0 -15.0  1.0  1.0 0.0 0.0  1.0 0.0 0.0  1.0 1.0 1.0  100.0  0.0 

# planes: center, normal, material
plane  -3 0 0  1 0 0  0.2 0.2 0.2  0.2 0.2 0.2  0.0 0.0 0.0  100.0  0.9
plane  3 0 0  -1 0 0  0.2 0.2 0.2  0.2 0.2 0.2  0.0 0.0 0.0  100.0  0.9
plane  0 -4 0  0 1 0  0.9 0.9 0.9  0.9 0.9 0.9  0.0 0.0 0.0  100.0  0.0
