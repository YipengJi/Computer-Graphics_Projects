# camera: eye, center, up, fovy, width, height
camera  280 66 -700  430 50 -670  0 1 0  45  500 500

# recursion depth
depth  1

# background color
background 0 0 0

# global ambient light
ambience   0.2 0.2 0.2

# light: position and color
light 400 120 -660 1.0 1.0 1.0

# meshes: filename, shading, material
mesh glas.off          FLAT   0.8 0.8 0.9  0.8 0.8 0.9  0.9 0.9 0.9  100.0  0.5
mesh fenster.off       FLAT   0.1 0.1 0.1  0.3 0.3 0.3  0.0 0.0 0.0  1.0    0.0
mesh stuhl_beine.off   PHONG  0.4 0.4 0.4  0.4 0.4 0.4  0.0 0.0 0.0  1.0    0.0
mesh stuhl_polster.off PHONG  0.2 0.2 0.7  0.2 0.2 0.7  0.0 0.0 0.0  1.0    0.0
mesh heizung.off       FLAT   0.1 0.3 0.1  0.1 0.3 0.1  0.2 0.4 0.2  1.0    0.0
mesh saeulen.off       FLAT   0.2 0.2 0.2  0.2 0.2 0.2  0.0 0.0 0.0  1.0    0.0
mesh boden.off         FLAT   0.2 0.1 0.1  0.2 0.1 0.1  0.0 0.0 0.0  1.0    0.0
mesh tisch_beine.off   FLAT   0.1 0.1 0.1  0.5 0.5 0.5  1.0 1.0 1.0  100.0  0.0
mesh tisch.off         FLAT   0.8 0.8 0.5  0.8 0.8 0.5  0.9 0.9 0.9  10.0   0.0
mesh schraenke.off     FLAT   0.8 0.8 0.5  0.8 0.8 0.5  0.9 0.9 0.9  10.0   0.0
mesh griffe.off        FLAT   0.1 0.1 0.1  0.3 0.3 0.3  0.0 0.0 0.0  1.0    0.0
mesh wand.off          FLAT   0.8 0.8 0.8  0.9 0.9 0.9  0.0 0.0 0.0  1.0    0.0
