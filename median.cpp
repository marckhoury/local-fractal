#include "median.h"

#include <algorithm>
#include <string>
#include <vector>

#include "grid.h"

using std::string;
using std::vector;

void MedianFilter::apply(Grid& g)
{
	vector<int> filtered, neighbors;
	int neighborhood[27];
	int count = 0;
	
    filtered.resize(g.vertex_count());
	neighbors.resize(27);
	for (int z = -1; z <= 1; z++) {
		for (int y = -1; y <= 1; y++) {
			for (int x = -1; x <= 1; x++) {
				neighborhood[count++] = g.index(x,y,z);
            }
        }
    }
	
	for (int z = 0; z < g.get_axis(2); z++) {
		for (int y = 0; y < g.get_axis(1); y++) {
			for (int x = 0; x < g.get_axis(0); x++) {
				int iv = g.index(x,y,z);
				if (x == 0 || y == 0 || z == 0 ||
                    x == g.get_axis(0)-1 || 
                    y == g.get_axis(1)-1 ||
                    z == g.get_axis(2)-1) {
                    filtered[iv] = g[iv];
				} else {
					for (int i = 0; i < 27; i++) {
						neighbors[i] = g[iv + neighborhood[i]];
                    }
					sort(neighbors.begin(), neighbors.end());
					filtered[iv] = neighbors[13];
				}
			}
		}
	}
	for (int i = 0; i < filtered.size(); i++) {
		g[i] = filtered[i];
    }

	g.set_name(g.to_string() + ".median");
}
