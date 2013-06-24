CC=g++
OBJECTS1 = cmdline.o grid.o fractalgrid.o sat.o gaussian.o median.o fractalgaussian.o fractalmedian.o gridalgorithms.o booleangrid.o fractalgrid4d.o sat4d.o
OBJECTS2 = fractalgrid.o grid.o mesh.o texture.o colormesh.o sat.o colorcmdline.o fractalgrid4d.o sat4d.o
LIBS = -L. libITKNrrdIO.a libz.so
TARGET1 = fractal
TARGET2 = meshcolor
OPT = -O2
all: fractal meshcolor

meshcolor: $(OBJECTS2)
	$(CC) $(OPT) -o $(TARGET2) meshcolor.cpp $(OBJECTS2) $(LIBS)

fractal: $(OBJECTS1)
	$(CC) $(OPT) -o $(TARGET1) main.cpp $(OBJECTS1) $(LIBS)

cmdline.o: cmdline.h cmdline.cpp
	$(CC) $(OPT) -c cmdline.cpp

colorcmdline.o: colorcmdline.h colorcmdline.cpp
	$(CC) $(OPT) -c colorcmdline.cpp

grid.o: grid.h grid.cpp
	$(CC) $(OPT) -c grid.cpp

sat.o: sat.h sat.cpp
	$(CC) $(OPT) -c sat.cpp

sat4d.o: sat4d.h sat4d.cpp
	$(CC) $(OPT) -c sat4d.cpp

fractalgrid.o: fractalgrid.h fractalgrid.cpp
	$(CC) $(OPT) -c fractalgrid.cpp

fractalgrid4d.o: fractalgrid4d.h fractalgrid4d.cpp
	$(CC) $(OPT) -c fractalgrid4d.cpp

booleangrid.o: booleangrid.h booleangrid.cpp
	$(CC) $(OPT) -c booleangrid.cpp

gaussian.o: gaussian.h gaussian.cpp
	$(CC) $(OPT) -c gaussian.cpp

median.o: median.h median.cpp
	$(CC) $(OPT) -c median.cpp

fractalgaussian.o: fractalgaussian.h fractalgaussian.cpp
	$(CC) $(OPT) -c fractalgaussian.cpp

fractalmedian.o: fractalmedian.h fractalmedian.cpp
	$(CC) $(OPT) -c fractalmedian.cpp

gridalgorithms.o: gridalgorithms.h gridalgorithms.cpp
	$(CC) $(OPT) -c gridalgorithms.cpp

mesh.o: mesh.h mesh.cpp
	$(CC) $(OPT) -c mesh.cpp

texture.o: texture.h texture.cpp
	$(CC) $(OPT) -c texture.cpp

colormesh.o: colormesh.h colormesh.cpp
	$(CC) $(OPT) -c colormesh.cpp

clean:
	rm *.o fractal meshcolor *~
