Whelp, heres my isometric demo for my game I'm working on. It creates an isometric grid, centered in the middle of the screen.

The centering turned out to be harder than I thought! It depends on the number of cols, rows, tile_size, screen height and screen width. Plus it was hard determining if I should translate before or after the isometric reflection. It's not perfect when rows and cols don't match!

With this setup I plan to draw overlapping cubes and it will look like a voxel world. And, I believe I can just store the center of these tiles for pathing and projectiles and treat entity movement the same as 2D.

This should work with the SFML library version we are using! I kept relevant values at the top so you can experiment with different dimensions.
