#include "fractalgrid.h"

FractalGrid::FractalGrid()
{
	name = "default";
	axis[0] = 0, axis[1] = 0, axis[2] = 0;
	spacing[0] = 0, spacing[1] = 0, spacing[2] = 0;
	spacing_flag = false;
}

FractalGrid::FractalGrid(Grid& g, int region, float isovalue)
{
	name = g.ToString() + ".rs=" + str(region) + ".iso=" + str(isovalue);
	region_size = region;
	iso = isovalue;
	spacing[0] = g.Spacing(0), spacing[1] = g.Spacing(1), spacing[2] = g.Spacing(2);
	spacing_flag = g.SpacingFlag();
	BuildGrid(g);
}

FractalGrid::FractalGrid(const FractalGrid& rhs)
{
	name = rhs.name;
	grid.resize(rhs.grid.size());
	for(int i = 0; i < grid.size(); i++)
		grid[i] = rhs.grid[i];
	axis[0] = rhs.axis[0], axis[1] = rhs.axis[1], axis[2] = rhs.axis[2];
	spacing[0] = rhs.spacing[0], spacing[1] = rhs.spacing[1], spacing[2] = rhs.spacing[2];
	iso = rhs.iso;
	region_size = rhs.region_size;
	spacing_flag = rhs.spacing_flag;
}

FractalGrid::~FractalGrid()
{
}

int FractalGrid::NumCubes()
{
	return (axis[0]-1)*(axis[1]-1)*(axis[2]-1);
}

int FractalGrid::NumVertices()
{
	return axis[0]*axis[1]*axis[2];
}

int FractalGrid::Axis(int index)
{
	return axis[index];
}

bool FractalGrid::SpacingFlag()
{
	return spacing_flag;
}

double FractalGrid::Spacing(int index)
{
	if(!spacing_flag)
		return 1;
	return spacing[index];
}

int FractalGrid::Index(int x, int y, int z)
{
	return x + y*axis[0] + z*axis[0]*axis[1];
}

float& FractalGrid::operator[](int index)
{
	return grid[index];
}

string FractalGrid::ToString()
{
	return name;
}

void FractalGrid::WriteNrrd()
{
	string output_filename = name + ".fractal.nhdr";
	Nrrd *nval;
 	nval = nrrdNew();
  	nrrdWrap_nva(nval, &(grid[0]), nrrdTypeFloat, 3, axis);
	if(spacing_flag)
  		nrrdAxisInfoSet_nva(nval, nrrdAxisInfoSpacing, spacing);
  	nrrdSave(output_filename.c_str(), nval, NULL);

  	nrrdNix(nval);
	cout << "Wrote file " << output_filename << endl;
}

FractalGrid& FractalGrid::operator=(const FractalGrid& rhs)
{
	if(this != &rhs)
	{
		name = rhs.name;
		grid.resize(rhs.grid.size());
		for(int i = 0; i < grid.size(); i++)
			grid[i] = rhs.grid[i];
		axis[0] = rhs.axis[0], axis[1] = rhs.axis[1], axis[2] = rhs.axis[2];
		spacing[0] = rhs.spacing[0], spacing[1] = rhs.spacing[1], spacing[2] = rhs.spacing[2];
		iso = rhs.iso;
		region_size = rhs.region_size;
		spacing_flag = rhs.spacing_flag;
	}
	return *this;
}

ostream& operator<<(ostream& os, FractalGrid& g)
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

void FractalGrid::BuildGrid(Grid& g)
{
	Grid sub = g.Subsample(2);
	SAT table(g,iso);
	SAT subtable(sub,iso);
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
				int cube_count = table.RegionCount(2*x,2*y,2*z,2*region_size);
				int sub_cube_count = subtable.RegionCount(x,y,z,region_size);
				int iv = g.Index(2*x,2*y,2*z); //Map from subsampled grid to original
				if(cube_count == 0) //Place fractal dimension value in grid
					grid[iv] = 0;
				else if(sub_cube_count == 0)
					grid[iv] = 3;
				else
					grid[iv] = log(((float)cube_count)/sub_cube_count)/log(2);
				
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

void FractalGrid::Offset(int offset[8])
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
