# camera: eye, center, up, fovy, width, height
camera 2 2 -10  -0.3 0.5 0  0 1 0  45  1024 1024

# recursion depth
depth  5

# background color
background 0 0 0

# global ambient light
ambience   0.0 0.0 0.0

# light: position and color
light   10 10 -5   0.9 0.9 0.9
light   -5  3 -5   0.4 0.4 0.4


# spheres: center, radius, material
sphere   0.67 0.5 0   0.5   0.0 0.0 0.0  1.0 0.0 0.0  0.3 0.0 0.0 100.0 0.3
sphere  -0.67 0.5 0   0.5   0.0 0.0 0.0  0.0 1.0 0.0  0.0 0.3 0.0 100.0 0.3
sphere  -2.00 0.5 0   0.5   0.0 0.0 0.0  0.0 0.0 1.0  0.0 0.0 0.3 100.0 0.3

# cylinders: center, radius, axis, height, material
cylinder  2.0 0.5 0.0  0.33   0.5 1.0 -0.5  1.0      0.0 0.0 0.0  0.8 0.8 0.8  0.8 0.8 0.8   100.0  0.7

# planes: center, normal, material
# Purple ground plane
plane  0 0 0  0 1 0      0.0 0.0 0.0  0.5 0.0 0.5  0.0 0.0 0.0  100.0  0.1
