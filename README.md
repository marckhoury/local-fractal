local-fractal
=============
A software package that computes a variety of isosurface fractal dimension measurements. Compiling the package creates two executables, fractal and meshcolor. The executable fractal, reads in a scalar grid from an nhdr file and, most notably, computes the local fractal dimension in a specified region around each grid vertex. Based on the local fractal dimension, selective noise filters can be applied to smooth the scalar grid, resulting in much smoother isosurfaces. There is also an implementation of a smooth surface construction algorithm (still experimental).

To visulize the local fractal dimension for a specific isovalue, meshcolor reads in an isosurface mesh and a scalar grid and colors the vertices of the mesh beased on the local fractal dimension values.

![ScreenShot](https://raw.github.com/marckhoury/local-fractal/master/screenshot1.png)

![ScreenShot](https://raw.github.com/marckhoury/local-fractal/master/screenshot2.png)

To construct an isosurface mesh to be used as input for meshcolor, use ijkmcube which can be downloaded from http://www.cse.ohio-state.edu/research/graphics/isotable/.

Dependencies:
libITKNrrdIO (library file included), zlib (shared library file included)

Copyright 2011 Marc Khoury
Please send any bug reports to marc.khry@gmail.com
