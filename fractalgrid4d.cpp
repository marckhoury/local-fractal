#include "fractalgrid4d.h"

#include <cmath>

#include "grid.h"
#include "ijkNrrd.h"
#include "sat4d.h"
#include "utility.h"

using std::cout;
using std::endl;
using std::ostream;
using std::string;
using std::vector;

FractalGrid4D::FractalGrid4D()
{
	name = "default";
	axis[0] = 0, axis[1] = 0, axis[2] = 0;
	spacing[0] = 0, spacing[1] = 0, spacing[2] = 0;
	spacing_flag = false;
}

FractalGrid4D::FractalGrid4D(Grid& g, int region_size, int inc) :
    region_size(region_size),
    inc(inc)
{
	name = g.to_string() + ".rs=" + str(region_size);
	spacing[0] = g.get_spacing(0), spacing[1] = g.get_spacing(1), spacing[2] = g.get_spacing(2);
	spacing_flag = g.has_spacing();
	build_grid(g);
}

FractalGrid4D::FractalGrid4D(const FractalGrid4D& rhs)
{
	name = rhs.name;
	grid.resize(rhs.grid.size());
	for (int i = 0; i < grid.size(); i++) {
		grid[i] = rhs.grid[i];
    }
	axis[0] = rhs.axis[0], axis[1] = rhs.axis[1], axis[2] = rhs.axis[2];
	spacing[0] = rhs.spacing[0], spacing[1] = rhs.spacing[1], spacing[2] = rhs.spacing[2];
	region_size = rhs.region_size;
	inc = rhs.inc;
	spacing_flag = rhs.spacing_flag;
}

FractalGrid4D::~FractalGrid4D()
{
}

size_t FractalGrid4D::cube_count()
{
	return (axis[0] - 1)*(axis[1] - 1)*(axis[2] - 1);
}

size_t FractalGrid4D::vertex_count()
{
	return axis[0]*axis[1]*axis[2];
}

size_t FractalGrid4D::get_axis(int i)
{
	return axis[i];
}

bool FractalGrid4D::has_spacing()
{
	return spacing_flag;
}

double FractalGrid4D::get_spacing(int i)
{
    return spacing_flag ? spacing[i] : 1;
}

size_t FractalGrid4D::index(size_t x, size_t y, size_t z)
{
	return x + y*axis[0] + z*axis[0]*axis[1];
}

float& FractalGrid4D::operator[](int index)
{
	return grid[index];
}

string FractalGrid4D::to_string()
{
	return name;
}

void FractalGrid4D::write_nrrd()
{
	string output_filename = name + ".fractal4d.nhdr";
	Nrrd *nval;
 	nval = nrrdNew();
  	nrrdWrap_nva(nval, &(grid[0]), nrrdTypeFloat, 3, axis);
	if (spacing_flag) {
  		nrrdAxisInfoSet_nva(nval, nrrdAxisInfoSpacing, spacing);
    }
  	nrrdSave(output_filename.c_str(), nval, NULL);

  	nrrdNix(nval);
	cout << "Wrote file " << output_filename << endl;
}

FractalGrid4D& FractalGrid4D::operator=(const FractalGrid4D& rhs)
{
	if(this != &rhs) {
		name = rhs.name;
		grid.resize(rhs.grid.size());
		for(int i = 0; i < grid.size(); i++) {
			grid[i] = rhs.grid[i];
        }
		axis[0] = rhs.axis[0], axis[1] = rhs.axis[1], axis[2] = rhs.axis[2];
		spacing[0] = rhs.spacing[0], spacing[1] = rhs.spacing[1], spacing[2] = rhs.spacing[2];
		region_size = rhs.region_size;
		spacing_flag = rhs.spacing_flag;
	}
	return *this;
}

ostream& operator<<(ostream& os, FractalGrid4D& g)
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

void FractalGrid4D::build_grid(Grid& g)
{
	Grid sub = g.subsample(2);
	SAT4D table(g, inc);
	SAT4D subtable(sub, inc);
	int offset[8];

	axis[0] = g.get_axis(0), axis[1] = g.get_axis(1), axis[2] = g.get_axis(2);
	grid.resize(axis[0]*axis[1]*axis[2]);
	
    compute_offset(offset);
   	for(int z = 0; z < sub.get_axis(2); z++) {
		for(int y = 0; y < sub.get_axis(1); y++) {
			for(int x = 0; x < sub.get_axis(0); x++) {
				long cube_count = table.region_count(2*x, 2*y, 2*z, 2*region_size);
				long sub_cube_count = subtable.region_count(x, y, z, region_size);
				int iv = g.index(2 * x, 2 * y, 2 * z); //Map from subsampled grid to original
				if (cube_count == 0) { //Place fractal dimension value in grid
					grid[iv] = 0;
				} else if (sub_cube_count == 0) {
					grid[iv] = 4;
				} else {
					grid[iv] = log(((double)cube_count)/sub_cube_count)/log(2);
				}

				if (x < sub.get_axis(0)-1 && y < sub.get_axis(1)-1 && z < sub.get_axis(2)-1)	{
					for(int i = 0; i < 8; i++) { //Fill in fractal value for ever cube vertex defined by iv
						grid[iv+offset[i]] = grid[iv];
                    }
				} else {  //We'll have to handle the boundary separetely
					int index[8][3] = {{0,0,0},{1,0,0},{0,1,0},{1,1,0},{0,0,1},{1,0,1},{0,1,1},{1,1,1}};
					for(int i = 0; i < 8; i++) {
						if (2*x + index[i][0] < g.get_axis(0) && 
                            2*y + index[i][1] < g.get_axis(1) && 
                            2*z + index[i][2] < g.get_axis(2)) {
							grid[iv+offset[i]] = grid[iv];
                        }
					}
				}
			}
		}
	}
}

void FractalGrid4D::compute_offset(int offset[8])
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
