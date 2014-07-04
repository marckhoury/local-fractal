#include "booleangrid.h"

#include <cmath>

#include "fractalgrid.h"
#include "grid.h"
#include "ijkNrrd.h"
#include "utility.h"

using std::cout;
using std::endl;
using std::ostream;
using std::string;
using std::vector;

BooleanGrid::BooleanGrid(Grid& g, int region_size, float isovalue, float dim_limit) :
    region_size(region_size),
    isovalue(isovalue),
    dim_limit(dim_limit)
{
	name = g.to_string() + ".rs=" + str(region_size) + ".iso=" + str(isovalue) + ".dim=" + str(dim_limit);
	build_grid(g);
}

BooleanGrid::BooleanGrid(const BooleanGrid& rhs)
{
	region_size = rhs.region_size;
	isovalue = rhs.isovalue;
	dim_limit = rhs.dim_limit;
	axis[0] = rhs.axis[0], axis[1] = rhs.axis[1], axis[2] = rhs.axis[2];
	grid.resize(rhs.grid.size());
	for(int i = 0; i < grid.size(); i++) {
		grid[i] = rhs.grid[i];
    }
}

BooleanGrid::~BooleanGrid()
{
}

void BooleanGrid::opening()
{
}

void BooleanGrid::closing()
{
}

size_t BooleanGrid::cube_count()
{
	return (axis[0] - 1)*(axis[1] - 1)*(axis[2] - 1);
}

size_t BooleanGrid::vertex_count()
{
	return axis[0]*axis[1]*axis[2];
}

size_t BooleanGrid::get_axis(int i)
{
	return axis[i];
}

size_t BooleanGrid::index(size_t x, size_t y, size_t z)
{
	return x + y*axis[0] + z*axis[0]*axis[1];
}

bool BooleanGrid::operator[](int index)
{
	return (grid[index] == 1) ? true : false;
}

string BooleanGrid::to_string()
{
	return name;
}

void BooleanGrid::write_nrrd()
{
	string output_filename = name + ".boolean.nhdr";
	Nrrd *nval;
 	nval = nrrdNew();
  	nrrdWrap_nva(nval, &(grid[0]), nrrdTypeInt, 3, axis);
  	nrrdSave(output_filename.c_str(), nval, NULL);

  	nrrdNix(nval);
	cout << "Wrote file " << output_filename << endl;
}

BooleanGrid& BooleanGrid::operator=(const BooleanGrid& rhs)
{
	if (this != &rhs) {
		region_size = rhs.region_size;
		isovalue = rhs.isovalue;
		dim_limit = rhs.dim_limit;
		axis[0] = rhs.axis[0], axis[1] = rhs.axis[1], axis[2] = rhs.axis[2];
		grid.resize(rhs.grid.size());
		for(int i = 0; i < grid.size(); i++)
			grid[i] = rhs.grid[i];
	}
	return *this;
}

ostream& operator<<(ostream& os, BooleanGrid& g)
{
	for(int z = 0; z < g.get_axis(2); z++) {
		for(int y = 0; y < g.get_axis(1); y++) {
			for(int x = 0; x < g.get_axis(0); x++) {
				os << g[g.index(x,y,z)] << " ";
			}
			os << endl;
		}
		os << endl;
	}
	return os;
}

void BooleanGrid::build_grid(Grid& g)
{
	FractalGrid fg(g, region_size, isovalue);
	int offset[8];
	
    grid.resize(fg.vertex_count());
	for(int i = 0; i < grid.size(); i++) {
		grid[i] = 0;
    }
	
    axis[0] = fg.get_axis(0), axis[1] = fg.get_axis(1), axis[2] = fg.get_axis(2);
	compute_offset(offset);
	
    for(int z = 0; z < g.get_axis(2)-1; z++) {
		for(int y = 0; y < g.get_axis(1)-1; y++) {
			for(int x = 0; x < g.get_axis(0)-1; x++) {
				int iv = g.index(x,y,z);
				if (g.intersected(iv, isovalue)) {
					set_cube(iv,1,offset);
                }
			}
		}
	}
	
    for(int z = 0; z < fg.get_axis(2); z++) {
		for(int y = 0; y < fg.get_axis(1); y++) {
			for(int x = 0; x < fg.get_axis(0); x++) {
				int iv = fg.index(x,y,z);
				if(grid[iv] == 1)
					grid[iv] = (fg[iv] > dim_limit) ? 1 : 0;
			}
		}
	}
}

void BooleanGrid::compute_offset(int offset[8])
{
	offset[0] = index(0,0,0);
	offset[1] = index(1,0,0);
	offset[2] = index(0,1,0);
	offset[3] = index(1,1,0);
	offset[4] = index(0,0,1);
	offset[5] = index(1,0,1);
	offset[6] = index(0,1,1);
	offset[7] = index(1,1,1);
}

void BooleanGrid::set_cube(int iv, int sv, int offset[8])
{
	for(int i = 0; i < 8; i++) {
		grid[iv + offset[i]] = sv;
    }
}

void BooleanGrid::erosion()
{
	vector<int> morphed;
	morphed.resize(grid.size());
	for(int z = 0; z < axis[2]; z++) {
		for(int y = 0; y < axis[1]; y++) {
			for(int x = 0; x < axis[0]; x++){
			}
		}
	}
}

void BooleanGrid::dilation()
{
    vector<int> morphed;
    morphed.resize(grid.size());
    for(int z = 0; z < axis[2]; z++) {
        for(int y = 0; y < axis[1]; y++) {
			for(int x = 0; x < axis[0]; x++){
			}
    	}
    }
}
