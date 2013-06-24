#ifndef BOOLEAN_GRID_H
#define BOOLEAN_GRID_H

#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include "utility.h"
#include "grid.h"
#include "fractalgrid.h"

using std::vector;
using std::string;
using std::ostream;
using std::endl;
using std::cout;

class BooleanGrid
{
private:
	vector<int> grid;
	size_t axis[3];
	string name;
	float iso;
	int region_size;
	float dim;

	void BuildGrid(Grid& g);	
	void Offset(int offset[8]);
	void SetCube(int iv, int sv, int offset[8]);
	void Erosion();
	void Dilation();
public:
	BooleanGrid(Grid& g, int region, float isovalue, float dim_limit);
	BooleanGrid(const BooleanGrid& bg);
	~BooleanGrid();
	
	void Opening();
	void Closing();
	int NumCubes();
	int NumVertices();
	int Axis(int index);
	int Index(int x, int y, int z);
	bool operator[](int index);
	string ToString();
	void WriteNrrd();
	BooleanGrid& operator=(const BooleanGrid& rhs);
	friend ostream& operator<<(ostream& os, BooleanGrid& g); 
};

#endif
