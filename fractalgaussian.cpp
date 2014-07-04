#include "fractalgaussian.h"

#include <iostream>
#include <string>
#include <vector>

#include "booleangrid.h"
#include "grid.h"
#include "utility.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;

FractalGaussianFilter::FractalGaussianFilter(int region_size, float isovalue, float dim_limit) :
    region_size(region_size),
    isovalue(isovalue),
    dim_limit(dim_limit) 
{
}

void FractalGaussianFilter::apply(Grid& g)
{
	BooleanGrid bg(g, region_size, isovalue, dim_limit);
	vector<int> filtered;
	vector<int> neighbors;
    int weight[27] = {1,2,1,2,4,2,1,2,1,
                      2,4,2,4,8,4,2,4,2,
                      1,2,1,2,4,2,1,2,1};
	int filtered_vert_count = 0;
	int count = 0;
	filtered.resize(g.vertex_count());
	neighbors.resize(27);
	for (int i = 0; i < filtered.size(); i++) { //Using -1 to mark unfiltered vertices
		filtered[i] = -1;
    }
	for (int z = -1; z <= 1; z++) {
		for (int y = -1; y <= 1; y++) {
			for (int x = -1; x <= 1; x++) {
				neighborhood[count++] = g.index(x,y,z);
            }
        }
    }

	for (int z = 0; z < bg.get_axis(2); z++) {
		for (int y = 0; y < bg.get_axis(1); y++) {
			for (int x = 0; x < bg.get_axis(0); x++) {
				if (bg[bg.index(x,y,z)]) {
					int iv = g.index(x,y,z);
					if (x == 0 || y == 0 || z == 0 ||
					    x == g.get_axis(0)-1 ||
                        y == g.get_axis(1)-1 ||
                        z == g.get_axis(2)-1) {
					    filtered[iv] = g[iv];
				    } else {
						int total = 0;
						for (int i = 0; i < 27; i++) {
							total += weight[i]*g[iv + neighborhood[i]];
                        }
						filtered[iv] = total / 64;
					}
				}
			}
		}
	}

	for (int i = 0; i < filtered.size(); i++) {
		if (filtered[i] != -1) {
			g[i] = filtered[i];
			filtered_vert_count++;
		}
	}
    cout << "Percentage of grid " << g.to_string() << " filtered: " 
         << (static_cast<double>(filtered_vert_count) / filtered.size())*100 << endl;
	g.set_name(g.to_string() + ".rs=" + str(region_size) + ".iso=" + str(isovalue) + ".dim=" + str(dim_limit) + ".gaussian");
}
