#include "grid.h"

Grid::Grid(string& file)
{
	name = file.substr(file.find_last_of("/")+1);
	name = name.substr(0,name.length()-5);
	offset[0] = -1;
	ReadNrrd(file,this->grid,this->axis,this->spacing);
}

Grid::Grid()
{
	name = "default";
	axis[0] = 0, axis[1] = 0, axis[2] = 0;
	offset[0] = -1;
	spacing[0] = 0, spacing[1] = 0, spacing[2] = 0;
}

Grid::Grid(const Grid& rhs)
{
	name = rhs.name;
	grid.resize(rhs.grid.size());
	for(int i = 0; i < grid.size(); i++)
		grid[i] = rhs.grid[i];
	for(int i = 0; i < 8; i++)
		offset[i] = rhs.offset[i];
	axis[0] = rhs.axis[0], axis[1] = rhs.axis[1], axis[2] = rhs.axis[2];
	spacing[0] = rhs.spacing[0], spacing[1] = rhs.spacing[1], spacing[2] = rhs.spacing[2];
	spacing_flag = rhs.spacing_flag;
}

Grid::~Grid()
{
}

Grid Grid::Subsample(int s)
{
	if(s == 1)
		return *this;
	Grid subsampled;
	subsampled.name = name + ".sub=" + str(s);
	subsampled.axis[0] = (axis[0]+1)/s, subsampled.axis[1] = (axis[1]+1)/s, subsampled.axis[2] = (axis[2]+1)/s;
	subsampled.spacing[0] = spacing[1], subsampled.spacing[1] = spacing[1], subsampled.spacing[2] = spacing[2];
	subsampled.grid.resize(subsampled.axis[0]*subsampled.axis[1]*subsampled.axis[2]);
	for(int z = 0; z < subsampled.axis[2]; z++)
		for(int y = 0; y < subsampled.axis[1]; y++)
			for(int x = 0; x < subsampled.axis[0]; x++)
				subsampled.grid[subsampled.Index(x,y,z)] = grid[this->Index(s*x,s*y,s*z)];
	return subsampled;
}

int Grid::NumCubes()
{
	return (axis[0]-1)*(axis[1]-1)*(axis[2]-1);
}

int Grid::NumVertices()
{
	return axis[0]*axis[1]*axis[2];
}

int Grid::Axis(int index)
{
	return axis[index];
}

bool Grid::SpacingFlag()
{
	return spacing_flag;
}

double Grid::Spacing(int index)
{
	if(!spacing_flag)
		return 1;
	return spacing[index];
}

int Grid::Index(int x, int y, int z)
{
	return x + y*axis[0] + z*axis[0]*axis[1];
}

int& Grid::operator[](int index)
{
	return grid[index];
}

bool Grid::Intersected(int iv, float iso)
{
	int min_sv, max_sv;
	if(offset[0] == -1)
		Offset();
	max_sv = grid[iv+offset[0]];
	min_sv = grid[iv+offset[0]];
	for(int i = 1; i < 8; i++)
	{
		max_sv = max_sv > grid[iv+offset[i]] ? max_sv : grid[iv+offset[i]];
		min_sv = min_sv < grid[iv+offset[i]] ? min_sv : grid[iv+offset[i]];
	}
	return iso >= min_sv and iso < max_sv;
}

void Grid::MaxMin(int iv, int& max_sv, int& min_sv)
{
	if(offset[0] == -1)
		Offset();
	max_sv = grid[iv+offset[0]];
	min_sv = grid[iv+offset[0]];
	for(int i = 1; i < 8; i++)
	{
		max_sv = max_sv > grid[iv+offset[i]] ? max_sv : grid[iv+offset[i]];
		min_sv = min_sv < grid[iv+offset[i]] ? min_sv : grid[iv+offset[i]];
	}
}

void Grid::SetName(string new_name)
{
	name = new_name;
}

string Grid::ToString()
{
	return name;
}

void Grid::WriteNrrd()
{
	string output_filename = name + ".nhdr";
	Nrrd *nval;
 	nval = nrrdNew();
  	nrrdWrap_nva(nval, &(grid[0]), nrrdTypeInt, 3, axis);
	if(spacing_flag)
  		nrrdAxisInfoSet_nva(nval, nrrdAxisInfoSpacing, spacing);
  	nrrdSave(output_filename.c_str(), nval, NULL);

  	nrrdNix(nval);
	cout << "Wrote file " << output_filename << endl;
}

Grid& Grid::operator=(const Grid& rhs)
{
	if(this != &rhs)
	{
		name = rhs.name;
		grid.resize(rhs.grid.size());
		for(int i = 0; i < grid.size(); i++)
			grid[i] = rhs.grid[i];
		for(int i = 0; i < 8; i++)
			offset[i] = rhs.offset[i];
		axis[0] = rhs.axis[0], axis[1] = rhs.axis[1], axis[2] = rhs.axis[2];
		spacing[0] = rhs.spacing[0], spacing[1] = rhs.spacing[1], spacing[2] = rhs.spacing[2];
		spacing_flag = rhs.spacing_flag;
	}
	return *this;
}

ostream& operator<<(ostream& os, Grid& g)
{
	for(int z = 0; z < g.axis[2]; z++)
	{
		for(int y = 0; y < g.axis[1]; y++)
		{
			for(int x = 0; x < g.axis[0]; x++)
			{
				os << g[g.Index(x,y,z)] << " ";
			}
			os << endl;
		}
		os << endl;
	}
	return os;
}

void Grid::ReadNrrd(string& infile, vector<int>& g, size_t axis_size[3], double grid_spacing[3])
{
	const int DIM3 = 3;
	Nrrd *nin = nrrdNew();
	int num_vert = 1;

	nrrdLoad(nin,infile.c_str(),NULL);                                //Read in scalar data set
	nrrdAxisInfoGet_nva(nin,nrrdAxisInfoSize,axis_size);       //Read axis size
	nrrdAxisInfoGet_nva(nin,nrrdAxisInfoSpacing,grid_spacing); //Read grid spacing
	spacing_flag = !isnan(grid_spacing[0]);
	if(nin->dim != DIM3)  //Is this a 3D data set?
	{
		cerr << "Illegal dimension for scalar field: " << nin->dim << endl;
		exit(1);
	}

	for(int i = 0; i < nin->dim; i++)
		num_vert *= axis_size[i];
	grid.resize(num_vert);
	nrrd2scalar(nin,&(g[0])); //Convert nrrd file to a 1D array representing our scalar grid.
	nrrdNuke(nin);
}

void Grid::Offset()
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
