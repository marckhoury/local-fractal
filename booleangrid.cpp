#include "booleangrid.h"

BooleanGrid::BooleanGrid(Grid& g, int region, float isovalue, float dim_limit)
{
	name = g.ToString() + ".rs=" + str(region) + ".iso=" + str(isovalue) + ".dim=" + str(dim_limit);
	region_size = region;
	iso = isovalue;
	dim = dim_limit;
	BuildGrid(g);
}

BooleanGrid::BooleanGrid(const BooleanGrid& rhs)
{
	region_size = rhs.region_size;
	iso = rhs.iso;
	dim = rhs.dim;
	axis[0] = rhs.axis[0], axis[1] = rhs.axis[1], axis[2] = rhs.axis[2];
	grid.resize(rhs.grid.size());
	for(int i = 0; i < grid.size(); i++)
		grid[i] = rhs.grid[i];
}

BooleanGrid::~BooleanGrid()
{
}

void BooleanGrid::Opening()
{
}

void BooleanGrid::Closing()
{
}

int BooleanGrid::NumCubes()
{
	return (axis[0]-1)*(axis[1]-1)*(axis[2]-1);
}

int BooleanGrid::NumVertices()
{
	return axis[0]*axis[1]*axis[2];
}

int BooleanGrid::Axis(int index)
{
	return axis[index];
}

int BooleanGrid::Index(int x, int y, int z)
{
	return x + y*axis[0] + z*axis[0]*axis[1];
}

bool BooleanGrid::operator[](int index)
{
	return (grid[index] == 1) ? true : false;
}

string BooleanGrid::ToString()
{
	return name;
}

void BooleanGrid::WriteNrrd()
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
	if(this != &rhs)
	{
		region_size = rhs.region_size;
		iso = rhs.iso;
		dim = rhs.dim;
		axis[0] = rhs.axis[0], axis[1] = rhs.axis[1], axis[2] = rhs.axis[2];
		grid.resize(rhs.grid.size());
		for(int i = 0; i < grid.size(); i++)
			grid[i] = rhs.grid[i];
	}
	return *this;
}

ostream& operator<<(ostream& os, BooleanGrid& g)
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

void BooleanGrid::BuildGrid(Grid& g)
{
	FractalGrid fg(g,region_size,iso);
	int offset[8];
	grid.resize(fg.NumVertices());
	for(int i = 0; i < grid.size(); i++)
		grid[i] = 0;
	axis[0] = fg.Axis(0), axis[1] = fg.Axis(1), axis[2] = fg.Axis(2);
	Offset(offset);
	for(int z = 0; z < g.Axis(2)-1; z++)
	{
		for(int y = 0; y < g.Axis(1)-1; y++)
		{
			for(int x = 0; x < g.Axis(0)-1; x++)
			{
				int iv = g.Index(x,y,z);
				if(g.Intersected(iv,iso))
					SetCube(iv,1,offset);
			}
		}
	}
	for(int z = 0; z < fg.Axis(2); z++)
	{
		for(int y = 0; y < fg.Axis(1); y++)
		{
			for(int x = 0; x < fg.Axis(0); x++)
			{
				int iv = fg.Index(x,y,z);
				if(grid[iv] == 1)
					grid[iv] = (fg[iv] > dim) ? 1 : 0;
			}
		}
	}
}

void BooleanGrid::Offset(int offset[8])
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

void BooleanGrid::SetCube(int iv, int sv, int offset[8])
{
	for(int i = 0; i < 8; i++)
		grid[iv+offset[i]] = sv;
}

void BooleanGrid::Erosion()
{
	vector<int> morphed;
	morphed.resize(grid.size());
	for(int z = 0; z < axis[2]; z++)
	{
		for(int y = 0; y < axis[1]; y++)
		{
			for(int x = 0; x < axis[0]; x++)
			{
				
			}
		}
	}
}

void BooleanGrid::Dilation()
{
	vector<int> morphed;
	morphed.resize(grid.size());
	for(int z = 0; z < axis[2]; z++)
	{
		for(int y = 0; y < axis[1]; y++)
		{
			for(int x = 0; x < axis[0]; x++)
			{
			}
		}
	}
}
