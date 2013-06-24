#ifndef SAT_H
#define SAT_H

#include <vector>
#include "grid.h"

using std::vector;
using std::ostream;
using std::endl;

class SAT
{
private:
	vector<int> table;
	float iso;
	size_t axis[3];
	
	int Index(int x, int y, int z);
	int BoxCount(Grid& g, int i, int j, int k);
public:
	SAT(Grid& g, float isovalue);
	~SAT();

	int RegionCount(int x, int y, int z, int n);
	friend ostream& operator<<(ostream& os, SAT& sat);
};

#endif
