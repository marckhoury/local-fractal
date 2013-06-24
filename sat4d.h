#ifndef SAT4D_H
#define SAT4D_H

#include <vector>
#include "grid.h"

using std::vector;
using std::ostream;
using std::endl;

class SAT4D
{
private:
	vector<long> table;
	float iso;
	size_t axis[3];
	int min_sv, max_sv;
	int inc;
	
	int Index(int x, int y, int z);
	long Intersected4D(Grid&g, int iv);
	long BoxCount(Grid& g, int i, int j, int k);
public:
	SAT4D(Grid& g, int increment);
	~SAT4D();

	long RegionCount(int x, int y, int z, int n);
	friend ostream& operator<<(ostream& os, SAT4D& sat);
};

#endif
