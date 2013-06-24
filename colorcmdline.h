#ifndef COLORCMDLINE_H
#define COLORCMDLINE_H

#include <iostream>
#include <vector>
#include <cctype>
#include <string>
#include <cstdlib>
#include "texture.h"

using std::cerr;
using std::endl;
using std::vector;
using std::string;

typedef enum{FRACTAL_RANGE_PARAM, COLOR_RANGE_PARAM, FRACTAL_4D_PARAM, HELP_PARAM, UNKNOWN_PARAM} Parameter;

class CmdLine
{
public:
	string in_mesh;
	string in_grid;
	bool use_4d; 
	float isovalue;
	int region_size;
	int inc;
	float low_dim;
	float high_dim;
	vector<Color> colors;
	Color c0;
	Color c1;
	
	CmdLine()
	{
		use_4d = false;
		low_dim = 2, high_dim = 2.5;
		c0.r = 0, c0.g = 0, c0.b = 1, c0.a = 1;
		c1.r = 1, c1.g = 0, c1.b = 0, c1.a = 1;
		colors.push_back(c0);
		colors.push_back(c1);
	}
};

void Usage();
void Help();
Parameter GetParameter(string& s);
void ParseCommandLine(int argc, char* argv[], CmdLine& cmd_line);
#endif
