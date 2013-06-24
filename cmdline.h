#ifndef CMDLINE_H
#define CMDLINE_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <cctype>
#include <vector>

using std::string;
using std::cerr;
using std::endl;
using std::vector;

typedef enum {BOX_DIM_PARAM, ISO_AREA_PARAM, REGION_PARAM, BOOLEAN_PARAM, FILTER_PARAM,
		FRACTAL_PARAM, FRACTAL_4D_PARAM, NOZ_PARAM, SMOOTH_SURFACE_PARAM, REGION_4D_PARAM, HELP_PARAM, UNKNOWN_PARAM} Parameter;

class CmdLine
{
public:
	string in_file;
	bool box_dim_plot;
	bool iso_area_plot;
	bool region;
	int region_size;
	float region_iso;
	bool region4d;
	int region4d_size;
	int region4d_inc;
	bool boolean_grid;
	int boolean_size;
	float boolean_iso;
	float boolean_dim;
	string filter_type;
	bool filter_opt;
	int filter_size;
	float filter_iso;
	float filter_dim;
	bool fractal;
	bool fractal_4d;
	float fractal_4d_inc;
	bool noz;
	bool ss_grid;
	int ss_size;
	int ss_inc;
	float ss_iso;
	float ss_dim;
	vector<float> fractal_iso;
	
	CmdLine()
	{
		box_dim_plot = false, iso_area_plot = false, region = false;
		filter_opt = false, fractal = false, boolean_grid = false;
		fractal_4d = false, noz = false, ss_grid = false;
		region4d = false;
	}
};

void Usage();
void Help();
Parameter GetParameter(string& s);
void ParseCommandLine(int argc, char* argv[], CmdLine& cmd_line);
#endif
