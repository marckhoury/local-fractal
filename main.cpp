// /*******************************************\
// | Author: Marc Khoury
// |
// | Desc: Computation of local fractal dimension
// |       grids, fractal noise filters, and other
// |       useful related tasks.
// |
// | Dependencies: libITKNrrdIO, libz
// \********************************************/

#include <iostream>
#include "cmdline.h"
#include "grid.h"
#include "sat.h"
#include "sat4d.h"
#include "fractalgrid.h"
#include "fractalgrid4d.h"
#include "booleangrid.h"
#include "filter.h"
#include "gaussian.h"
#include "median.h"
#include "fractalgaussian.h"
#include "fractalmedian.h"
#include "gridalgorithms.h"

using namespace std;

int main(int argc, char* argv[])
{
	CmdLine cmd_line;
	ParseCommandLine(argc,argv,cmd_line);
	Grid grid(cmd_line.in_file);

	if(cmd_line.box_dim_plot)
	{
		vector<float> fractal = GridAlgorithms::FractalValues(grid);
		string out_file = grid.ToString() + ".box_dim.gplt";
		WritePlot(out_file,fractal);
	}
	if(cmd_line.iso_area_plot)
	{
		vector<int> edge_count = GridAlgorithms::IntersectedEdges(grid);
		string out_file = grid.ToString() + ".isoarea.edge.gplt";
		WritePlot(out_file,edge_count);
	}
	if(cmd_line.region)
	{
		FractalGrid fg(grid,cmd_line.region_size,cmd_line.region_iso);
		fg.WriteNrrd();
	}
	if(cmd_line.region4d)
	{
		FractalGrid4D fg(grid,cmd_line.region4d_size,cmd_line.region4d_inc);
		fg.WriteNrrd();
	}
	if(cmd_line.boolean_grid)
	{
		BooleanGrid bg(grid,cmd_line.boolean_size, cmd_line.boolean_iso, cmd_line.boolean_dim);
		bg.WriteNrrd();
	}
	if(cmd_line.filter_type.length() != 0)
	{
		Filter* filter;
		int region_size = cmd_line.filter_size;
		float isovalue = cmd_line.filter_iso;
		float dim = cmd_line.filter_dim;
		if(cmd_line.filter_type == "gaussian")
		{
			if(cmd_line.filter_opt)
				filter = new FractalGaussianFilter(region_size,isovalue,dim);
			else
				filter = new GaussianFilter();
		}
		else if(cmd_line.filter_type == "median")
		{
			if(cmd_line.filter_opt)
				filter = new FractalMedianFilter(region_size,isovalue,dim);
			else
				filter = new MedianFilter();
		}
		else
		{	cerr << "Unrecognized filter type: " << cmd_line.filter_type << endl;}
		
		filter->Apply(grid);
		grid.WriteNrrd();
		delete filter;
	}
	if(cmd_line.fractal)
	{
		vector<float> fractal = GridAlgorithms::FractalValues(grid);
		cout << "Fractal Dimension: " << endl;
		for(int i = 0; i < cmd_line.fractal_iso.size(); i++)
		{
			float iso = cmd_line.fractal_iso[i];
			if(iso >= fractal.size())
				cerr << "Isovalue " << iso << "exceeds the maximum scalar value in " << grid.ToString() << endl;
			else
				cout << iso << "\t" << fractal[((int)iso)] << endl;
		}
	}
	if(cmd_line.fractal_4d)
	{
		float fractal_4d = GridAlgorithms::FractalDimension4D(grid, cmd_line.fractal_4d_inc, cmd_line.noz);
		cout << cmd_line.in_file << " " << fractal_4d << endl;
	}
	if(cmd_line.ss_grid)
	{
		GridAlgorithms::SmoothSurface(grid, cmd_line.ss_size, cmd_line.ss_inc, cmd_line.ss_iso, cmd_line.ss_dim);
		grid.WriteNrrd();
	}
	return 0;
}
