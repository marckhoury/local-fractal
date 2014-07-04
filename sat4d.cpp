#include "sat4d.h"

#include <cmath>

#include "grid.h"

using std::endl;
using std::ostream;
using std::vector;

SAT4D::SAT4D(Grid& g, int inc) :
    inc(inc)
{
	axis[0] = g.get_axis(0), axis[1] = g.get_axis(1), axis[2] = g.get_axis(2);
    
	table.resize(g.vertex_count());
	for (int i = 0; i < table.size(); i++) {
		table[i] = -1;
    }
	
    max_sv = g[0], min_sv = g[0];
	for (int i = 0; i < g.vertex_count(); i++) {
		if (max_sv < g[i]) {
			max_sv = g[i];
        }
		if (min_sv > g[i]) {
			min_sv = g[i];
        }
	}
	
    box_count(g, g.get_axis(0)-1, g.get_axis(1)-1, g.get_axis(2)-1);
}

SAT4D::~SAT4D()
{}

long SAT4D::region_count(int x, int y, int z, int n)
{
	int upper[3] = {x + n, y + n, z + n};
	int lower[3] = {x - n, y - n, z - n};
	int region[8];
	for(int i = 0; i < 3; i++) {
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

ostream& operator<<(ostream& os, SAT4D& sat)
{
	for (int z = 0; z < sat.axis[2]; z++) {
		for (int y = 0; y < sat.axis[1]; y++) {
			for (int x = 0; x < sat.axis[0]; x++) {
				os << sat.table[sat.index(x, y, z)] << " ";
			}
			os << endl;
		}
		os << endl;
	}
	return os;
}

int SAT4D::index(int x, int y, int z)
{
	return x + y*axis[0] + z*axis[0]*axis[1];
}

long SAT4D::intersected_4d(Grid& g, int iv)
{
	int span_0, span_1;
	int x0, x1;
	
    g.max_min(iv,span_1,span_0);

	if (span_1-span_0 == 0) {
		return 1;
	} else {
		x0 = (span_0 - min_sv) / inc;
		if ((span_1 - min_sv) % inc == 0) {
			x1 = (span_1 - min_sv) / inc;
		} else {
			x1 = static_cast<int>(ceil((span_1 - min_sv) / inc + 0.5) + 0.5);
		}
        return x1 - x0;
	}
}

long SAT4D::box_count(Grid& g, int i, int j, int k)
{
	int iv = index(i,j,k);

	if(table[iv] != -1) {
		return table[iv];
	} else if(i == 0 || j == 0 || k == 0) {
		table[iv] = 0;
	} else {
		int ic = g.index(i - 1, j  -1, k - 1);
		long intersected = intersected_4d(g, ic);
		table[iv] = box_count(g, i - 1, j, k) + box_count(g, i, j - 1, k) + box_count(g, i, j, k - 1) - 
                    box_count(g, i - 1, j - 1, k) - box_count(g, i, j - 1, k - 1) - box_count(g, i - 1, j, k - 1) + 
                    box_count(g, i - 1, j - 1, k - 1) + intersected;
	}
	return table[iv];
}
