#ifndef FRACTAL_GRID_H
#define FRACTAL_GRID_H

#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include "utility.h"
#include "grid.h"
#include "sat.h"

using std::vector;
using std::string;
using std::ostream;
using std::endl;
using std::cout;

class FractalGrid
{
private:
	vector<float> grid;
	size_t axis[3];
	bool spacing_flag;
	double spacing[3];
	string name;
	int region_size;
	float iso;

	void BuildGrid(Grid& g);
	void Offset(int offset[8]);
public:	
	FractalGrid();
	FractalGrid(Grid& g, int region, float isovalue);
	FractalGrid(const FractalGrid& rhs);
	~FractalGrid();
	
	int NumCubes();
	int NumVertices();
	int Axis(int index);
	bool SpacingFlag();
	double Spacing(int index);
	int Index(int x, int y, int z);
	float& operator[](int index);
	string ToString();
	void WriteNrrd();
	FractalGrid& operator=(const FractalGrid& rhs);
	friend ostream& operator<<(ostream& os, FractalGrid& g); 
};

#endif
