#include "sat.h"

#include "grid.h"

using std::endl;
using std::ostream;
using std::vector;

SAT::SAT(Grid& g, float isovalue) :
    isovalue(isovalue)
{
	axis[0] = g.get_axis(0), axis[1] = g.get_axis(1), axis[2] = g.get_axis(2);
	table.resize(g.vertex_count());
	for (int i = 0; i < table.size(); i++) {
		table[i] = -1;
    }
	box_count(g, g.get_axis(0)-1, g.get_axis(1)-1, g.get_axis(2)-1);
}

SAT::~SAT()
{
}

int SAT::region_count(int x, int y, int z, int n)
{
	int upper[3] = {x + n, y + n, z + n};
	int lower[3] = {x - n, y - n, z - n};
	int region[8];
	for (int i = 0; i < 3; i++) {
		lower[i] = lower[i] >= 0 ? lower[i] : 0;
		upper[i] = upper[i] < axis[i] ? upper[i] : axis[i] - 1;
	}
	
	region[0] = index(upper[0], upper[1], upper[2]);
	region[1] = index(lower[0], upper[1], upper[2]);
	region[2] = index(upper[0], lower[1], upper[2]);
	region[3] = index(lower[0], lower[1], upper[2]);
	region[4] = index(upper[0], upper[1], lower[2]);
	region[5] = index(lower[0], upper[1], lower[2]);
    region[6] = index(upper[0], lower[1], lower[2]);
	region[7] = index(lower[0], lower[1], lower[2]);
	
	return table[region[0]] - table[region[1]] - table[region[4]] + table[region[5]] - 
           table[region[2]] + table[region[3]] + table[region[6]] - table[region[7]];
}

ostream& operator<<(ostream& os, SAT& sat)
{
	for(int z = 0; z < sat.axis[2]; z++) {
		for(int y = 0; y < sat.axis[1]; y++) {
			for(int x = 0; x < sat.axis[0]; x++) {
				os << sat.table[sat.index(x, y, z)] << " ";
			}
			os << endl;
		}
		os << endl;
	}
	return os;
}

int SAT::index(int x, int y, int z)
{
	return x + y*axis[0] + z*axis[0]*axis[1];
}

int SAT::box_count(Grid& g, int i, int j, int k)
{
	int iv = index(i,j,k);

	if (table[iv] != -1) {
		return table[iv];
    }
	else if (i == 0 || j == 0 || k == 0) {
		table[iv] = 0;
    } else {
        int ic = g.index(i-1,j-1,k-1);
        int intersected = g.intersected(ic, isovalue) ? 1 : 0;
        table[iv] = box_count(g, i - 1, j, k) + box_count(g, i, j - 1, k) + box_count(g, i, j, k - 1) - 
                    box_count(g, i - 1, j - 1, k) - box_count(g, i, j - 1, k - 1) - box_count(g, i - 1, j, k - 1) + 
                    box_count(g, i - 1, j - 1, k - 1) + intersected;
	}
	return table[iv];
}
