#ifndef GRID_H
#define GRID_H

#include <vector>
#include <string>
#include "utility.h"
#include "ijkNrrd.h"

using std::string;
using std::vector;
using std::ostream;
using std::cerr;
using std::endl;

class Grid
{
private:
	vector<int> grid;
	size_t axis[3];
	bool spacing_flag;
	double spacing[3];
	string name;
	int offset[8];
	
	void ReadNrrd(string& infile, vector<int>& g, size_t axis_size[3], double grid_spacing[3]);
	void Offset();
public:
	Grid(string& file);
	Grid();
	Grid(const Grid& rhs);
	~Grid();
	
	Grid Subsample(int s);
	int NumCubes();
	int NumVertices();
	int Axis(int index);
	bool SpacingFlag();
	double Spacing(int index);
	int Index(int x, int y, int z);
	int& operator[](int index);
	bool Intersected(int iv, float iso);
	void MaxMin(int iv, int& max_sv, int& min_sv);
	void SetName(string new_name);
	string ToString();
	void WriteNrrd();
	Grid& operator=(const Grid& rhs);
	friend ostream& operator<<(ostream& os, Grid& g);
};

#endif
