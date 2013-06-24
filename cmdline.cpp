#include "cmdline.h"

string cmd_params[] = {"-box_dim","-iso_area", "-region", "-boolean" ,"-filter", "-fractal", "-4D", "-noz", "-ss", "-region4d", "-help"};

void Usage()
{
	cerr << "Usage: fractal [OPTIONS] <input_file>" << endl;
	cerr << "OPTIONS: " << endl;
	cerr << "[-box_dim] [-iso_area]" << endl;
	cerr << "[-region region_size isovalue]" << endl;
	cerr << "[-region4d region_size inc]" << endl;
	cerr << "[-boolean region_size isovalue dim]" << endl;
	cerr << "[-filter {median|gaussian} [region_size isovalue dim] ]" << endl;
	cerr << "[-fractal {isovalue0 isovalue1 ...}] [-4D inc] [-noz]" << endl;
	cerr << "[-ss region_size inc isovalue dim] [-help]" << endl;
	exit(0);
}

void Help()
{
	cerr << "Usage: fractal [OPTIONS] <input_file>" << endl;
	cerr << "OPTIONS: " << endl;
	cerr << "-box_dim : Produces a fractal box dimension plot for the input data set." << endl << endl;
	cerr << "-iso_area : Produces a isosurfaces area plot for the input data set." << endl << endl;
	cerr << "-region region_size isovalue : Computes a grid where each scalar value is the local fractal dimension of the (4*region_size)^3 region centered at the vertex. Thus region_size is the number of cubes from the center of the region to the boundary in the subsampled grid. For example, given a region_size=1, the fractal dimension is computed using 5x5x5 regions in the grid, which when subsampled are 3x3x3 grids. Additionally the resuling grid has size equal to the subsampled grid, since the local fractal dimension is computed for each vertex in the subsampled grid." << endl << endl;
	cerr << "-region4d region_size inc: Does the same thing as -region only compute the 4d local fractal dimension." << endl;
	cerr << "-boolean region_size isovalue dim : Computes a boolean grid where a vertex will have scalar value 1 if it would be filtered using a fractal filter technique and 0 if it would not be filtered." << endl << endl;
	cerr << "-filter {median|gaussian} [region_size isovalue dim] : Applies a median or gaussian filter to the entire data set. If the optional parameters are included the specified filter will only be applied to regions with local fractal dimension greater than dim." << endl << endl;
	cerr << "-fractal {isovalue0 isovalue1 ...} : Computes the fractal dimension of the input data set for the given isovalues." << endl << endl;
	cerr << "-4D inc: Computes the 4D fractal dimension of the scalar grid using inc as the increment through the scalar values." << endl << endl;
	cerr << "-noz: Do not count zero span cubes with value zero in the 4D fractal dimension." << endl << endl;
	cerr << "-ss region_size inc isovalue dim: Move a isosurface to a region with local fractal dimension less than dim." << endl << endl;
	cerr << "-help : Brings up this help message." << endl;
	exit(0);
}

Parameter GetParameter(string& s)
{
	for(int i = 0; i < (int)UNKNOWN_PARAM; i++)
		if(cmd_params[i] == s)
			return Parameter(i);
	return UNKNOWN_PARAM;
}

void ParseCommandLine(int argc, char* argv[], CmdLine& cmd_line)
{
	if(argc == 1)
		Usage();
	
	int argi = 1;
	while(argi < argc and argv[argi][0] == '-')
	{
		string str = argv[argi];
		Parameter param = GetParameter(str);
		switch(param)
		{
			case BOX_DIM_PARAM:
			{
				cmd_line.box_dim_plot = true;
			}
			break;
			case ISO_AREA_PARAM:
			{
				cmd_line.iso_area_plot = true;
			}
			break;
			case REGION_PARAM:
			{
				argi += 2;
				if(argi >= argc)
					Usage();
				cmd_line.region = true;
				cmd_line.region_size = atoi(argv[argi-1]);
				cmd_line.region_iso = atof(argv[argi]);
			}
			break;
			case REGION_4D_PARAM:
			{
				argi += 2;
				if(argi >= argc)
					Usage();
				cmd_line.region4d = true;
				cmd_line.region4d_size = atoi(argv[argi-1]);
				cmd_line.region4d_inc = atoi(argv[argi]);
			}
			break;
			case BOOLEAN_PARAM:
			{
				argi += 3;
				if(argi >= argc)
					Usage();
				cmd_line.boolean_grid = true;
				cmd_line.boolean_size = atoi(argv[argi-2]);
				cmd_line.boolean_iso = atof(argv[argi-1]);
				cmd_line.boolean_dim = atof(argv[argi]);
			}
			break;
			case FILTER_PARAM:
			{
				argi++;
				if(argi >= argc)
					Usage();
				cmd_line.filter_type = argv[argi];
				if(argi+1 < argc-1 and argv[argi+1][0] != '-')
				{
					if(argi+3 >= argc)
						Usage();
					cmd_line.filter_size = atoi(argv[argi+1]);
					cmd_line.filter_iso = atof(argv[argi+2]);
					cmd_line.filter_dim = atof(argv[argi+3]);
					cmd_line.filter_opt = true;
					argi += 3;
				}
			}
			break;
			case FRACTAL_PARAM:
			{
				argi++;
				if(argi >= argc)
					Usage();
				cmd_line.fractal = true;
				for(; argi < argc-1 && argv[argi][0] != '-'; argi++)
				{
					string iso = argv[argi];
					for(int i = 0; i < iso.length(); i++)
						if(!(isdigit(iso[i]) or iso[i] == '.'))
							Usage();
					cmd_line.fractal_iso.push_back(atof(argv[argi]));
				}
				argi--;
			}
			break;
			case FRACTAL_4D_PARAM:
			{
				argi++;
				cmd_line.fractal_4d = true;
				cmd_line.fractal_4d_inc = atof(argv[argi]);
			}
			break;
			case SMOOTH_SURFACE_PARAM:
			{
				argi += 4;
				if(argi >= argc)
					Usage();
				cmd_line.ss_grid = true;
				cmd_line.ss_size = atoi(argv[argi-3]);
				cmd_line.ss_inc = atoi(argv[argi-2]);
				cmd_line.ss_iso = atof(argv[argi-1]);
				cmd_line.ss_dim = atof(argv[argi]);
			}
			case NOZ_PARAM:
			{
				cmd_line.noz = true;
			}
			break;
			case HELP_PARAM:
			{
				Help();
			}
			break;
			case UNKNOWN_PARAM:
			{
				Usage();
			}
			default:
			break;
		}
		argi++;
	}
	if(argi == argc-1)
		cmd_line.in_file = argv[argi];
	else
		Usage();
}
