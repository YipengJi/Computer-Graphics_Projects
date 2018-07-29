# camera: eye, center, up, fovy, width, height
camera 1 2 3  0 0.5 0  0 1 0  45  500 500

# recursion depth
depth  0

# background color
background 0 0 0

# global ambient light
ambience   0.2 0.2 0.2

# light: position and color
light  40  0 -5   0.5 0.5 0.5
light  50 50 50  0.35 0.35 0.35
light -10 0 -5  0.35 0.35 0.35

# mesh: filename, FLAT/SMOOTH, material
mesh mask.off PHONG  1 0.6 0.45  1 0.6 0.45  0.5 0.5 0.5  11.0  0.0

