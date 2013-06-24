#include "fractalgrid4d.h"

FractalGrid4D::FractalGrid4D()
{
	name = "default";
	axis[0] = 0, axis[1] = 0, axis[2] = 0;
	spacing[0] = 0, spacing[1] = 0, spacing[2] = 0;
	spacing_flag = false;
}

FractalGrid4D::FractalGrid4D(Grid& g, int region, int increment)
{
	name = g.ToString() + ".rs=" + str(region);
	region_size = region;
	inc = increment;
	spacing[0] = g.Spacing(0), spacing[1] = g.Spacing(1), spacing[2] = g.Spacing(2);
	spacing_flag = g.SpacingFlag();
	BuildGrid(g);
}

FractalGrid4D::FractalGrid4D(const FractalGrid4D& rhs)
{
	name = rhs.name;
	grid.resize(rhs.grid.size());
	for(int i = 0; i < grid.size(); i++)
		grid[i] = rhs.grid[i];
	axis[0] = rhs.axis[0], axis[1] = rhs.axis[1], axis[2] = rhs.axis[2];
	spacing[0] = rhs.spacing[0], spacing[1] = rhs.spacing[1], spacing[2] = rhs.spacing[2];
	region_size = rhs.region_size;
	inc = rhs.inc;
	spacing_flag = rhs.spacing_flag;
}

FractalGrid4D::~FractalGrid4D()
{
}

int FractalGrid4D::NumCubes()
{
	return (axis[0]-1)*(axis[1]-1)*(axis[2]-1);
}

int FractalGrid4D::NumVertices()
{
	return axis[0]*axis[1]*axis[2];
}

int FractalGrid4D::Axis(int index)
{
	return axis[index];
}

bool FractalGrid4D::SpacingFlag()
{
	return spacing_flag;
}

double FractalGrid4D::Spacing(int index)
{
	if(!spacing_flag)
		return 1;
	return spacing[index];
}

int FractalGrid4D::Index(int x, int y, int z)
{
	return x + y*axis[0] + z*axis[0]*axis[1];
}

float& FractalGrid4D::operator[](int index)
{
	return grid[index];
}

string FractalGrid4D::ToString()
{
	return name;
}

void FractalGrid4D::WriteNrrd()
{
	string output_filename = name + ".fractal4d.nhdr";
	Nrrd *nval;
 	nval = nrrdNew();
  	nrrdWrap_nva(nval, &(grid[0]), nrrdTypeFloat, 3, axis);
	if(spacing_flag)
  		nrrdAxisInfoSet_nva(nval, nrrdAxisInfoSpacing, spacing);
  	nrrdSave(output_filename.c_str(), nval, NULL);

  	nrrdNix(nval);
	cout << "Wrote file " << output_filename << endl;
}

FractalGrid4D& FractalGrid4D::operator=(const FractalGrid4D& rhs)
{
	if(this != &rhs)
	{
		name = rhs.name;
		grid.resize(rhs.grid.size());
		for(int i = 0; i < grid.size(); i++)
			grid[i] = rhs.grid[i];
		axis[0] = rhs.axis[0], axis[1] = rhs.axis[1], axis[2] = rhs.axis[2];
		spacing[0] = rhs.spacing[0], spacing[1] = rhs.spacing[1], spacing[2] = rhs.spacing[2];
		region_size = rhs.region_size;
		spacing_flag = rhs.spacing_flag;
	}
	return *this;
}

ostream& operator<<(ostream& os, FractalGrid4D& g)
{
	for(int z = 0; z < g.Axis(2); z++)
	{
		for(int y = 0; y < g.Axis(1); y++)
		{
			for(int x = 0; x < g.Axis(0); x++)
			{
				os << g[g.Index(x,y,z)] << " ";
			}
			os << endl;
		}
		os << endl;
	}
	return os;
}

void FractalGrid4D::BuildGrid(Grid& g)
{
	Grid sub = g.Subsample(2);
	SAT4D table(g, inc);
	SAT4D subtable(sub, inc);
	int offset[8];
	axis[0] = g.Axis(0), axis[1] = g.Axis(1), axis[2] = g.Axis(2);
	grid.resize(axis[0]*axis[1]*axis[2]);
	Offset(offset);
	for(int z = 0; z < sub.Axis(2); z++)
	{
		for(int y = 0; y < sub.Axis(1); y++)
		{
			for(int x = 0; x < sub.Axis(0); x++)
			{
				long cube_count = table.RegionCount(2*x,2*y,2*z,2*region_size);
				long sub_cube_count = subtable.RegionCount(x,y,z,region_size);
				int iv = g.Index(2*x,2*y,2*z); //Map from subsampled grid to original
				if(cube_count == 0) //Place fractal dimension value in grid
					grid[iv] = 0;
				else if(sub_cube_count == 0)
					grid[iv] = 4;
				else
					grid[iv] = log(((double)cube_count)/sub_cube_count)/log(2);
				
				if(x < sub.Axis(0)-1 and y < sub.Axis(1)-1 and z < sub.Axis(2)-1)
				{
					for(int i = 0; i < 8; i++) //Fill in fractal value for ever cube vertex defined by iv
						grid[iv+offset[i]] = grid[iv];
				}
				else  //We'll have to handle the boundary separetely
				{
					int index[8][3] = {{0,0,0},{1,0,0},{0,1,0},{1,1,0},{0,0,1},{1,0,1},{0,1,1},{1,1,1}};
					for(int i = 0; i < 8; i++)
					{
						if(2*x + index[i][0] < g.Axis(0) and 2*y+index[i][1] < g.Axis(1) and 2*z+index[i][2] < g.Axis(2))
							grid[iv+offset[i]] = grid[iv];
					}
				}
			}
		}
	}
}

void FractalGrid4D::Offset(int offset[8])
{
	offset[0] = Index(0,0,0);
	offset[1] = Index(1,0,0);
	offset[2] = Index(0,1,0);
	offset[3] = Index(1,1,0);
	offset[4] = Index(0,0,1);
	offset[5] = Index(1,0,1);
	offset[6] = Index(0,1,1);
	offset[7] = Index(1,1,1);
}
