#!/bin/bash
t=$1

nframes=90
PI=3.141592653

for frame in $(seq $nframes); do
	# Animate the camera's (x, z) position
	cameraX=$(bc -l <<< "scale=4; 8 * s(($frame - 1) * 2 * $PI / $nframes)");
	cameraZ=$(bc -l <<< "scale=4; 8 * c(($frame - 1) * 2 * $PI / $nframes)");

	../../build/raytrace /dev/stdin out.tga <<-EOF
		# camera: eye, center, up, fovy, width, height
		camera $cameraX 3 $cameraZ  0 1 0  0 1 0  45  1080 1080

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
		EOF
		# You'll need to install Imagemagick to convert .tga to .png...
		convert out.tga $(printf "frame_%02i.png" $frame)
done

# ... and ffmpeg to stitch the frames together into a movie
ffmpeg -framerate 30 -i frame_%02d.png -vcodec libx264 -pix_fmt yuv420p -crf 18 movie.mp4

rm frame_*.png out.tga
