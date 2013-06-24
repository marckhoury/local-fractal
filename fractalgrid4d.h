#ifndef FRACTAL_GRID_4D_H
#define FRACTAL_GRID_4D_H

#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include "utility.h"
#include "grid.h"
#include "sat4d.h"

using std::vector;
using std::string;
using std::ostream;
using std::endl;
using std::cout;

class FractalGrid4D
{
private:
	vector<float> grid;
	size_t axis[3];
	bool spacing_flag;
	double spacing[3];
	string name;
	int region_size;
	int inc;

	void BuildGrid(Grid& g);
	void Offset(int offset[8]);
public:	
	FractalGrid4D();
	FractalGrid4D(Grid& g, int region, int increment);
	FractalGrid4D(const FractalGrid4D& rhs);
	~FractalGrid4D();
	
	int NumCubes();
	int NumVertices();
	int Axis(int index);
	bool SpacingFlag();
	double Spacing(int index);
	int Index(int x, int y, int z);
	float& operator[](int index);
	string ToString();
	void WriteNrrd();
	FractalGrid4D& operator=(const FractalGrid4D& rhs);
	friend ostream& operator<<(ostream& os, FractalGrid4D& g); 
};

#endif
