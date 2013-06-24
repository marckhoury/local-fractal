#include "colorcmdline.h"

string cmd_params[] = {"-fractal_range", "-color_range", "-fractal4d", "-help"};

void Usage()
{
	cerr << "Usage: meshcolor [OPTIONS] <region_size> <isovalue> <mesh_file> <grid_file>" << endl;
	cerr << "OPTIONS:" << endl;
	cerr << "[-fractal_range low_dim high_dim]" << endl;
	cerr << "[-fractal4d inc]" << endl;
	cerr << "[-color_range n r0 g0 b0 ... rn gn bn]" << endl;
	cerr << "[-help]" << endl;
	exit(0);
}

void Help()
{
	cerr << "Usage: meshcolor [OPTIONS] <region_size> <isovalue> <mesh_file> <grid_file>" << endl;
	cerr << "OPTIONS:" << endl;
	cerr << "-fractal_range low_dim high_dim: Sets the range for high and low local fractal dimension. All vertices in the mesh with local dimension larger than high_dim are set to the high color, while those with less than low_dim are set to the low color. All other values are lerped between the two ranges. Default values are 2 and 2.5" << endl;
	cerr << "-fractal4d inc: Use the 4D fractal dimension grid with increment inc to color the surface. The isovalue provided will be ignored." << endl;
	cerr << "-color_range n r0 g0 b0 ... rn gn bn: Sets the range for the low and high color values. The values r0,g0,b0 are the low color values and have default (0,0,1). The values rn,gn,bn are the high color values and have default (1,0,0). If this setting is not included the default is only two colors, from blue to red." << endl;
	cerr << "-help: Brings up this help message." << endl;
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
			case FRACTAL_RANGE_PARAM:
			{
				argi += 2;
				if(argi >= argc)
					Usage();
				cmd_line.low_dim = atof(argv[argi-1]);
				cmd_line.high_dim = atof(argv[argi]);
			}
			break;
			case COLOR_RANGE_PARAM:
			{
				argi++;
				if(argi >= argc)
					Usage();
				int n = atoi(argv[argi]);
				cmd_line.colors.clear();
				argi++;
				for(int i = 0; i < n; i++)
				{
					Color ci;
					if(argi + 2 >= argc)
						Usage();
					ci.r = atof(argv[argi]);
					ci.g = atof(argv[argi+1]);
					ci.b = atof(argv[argi+2]);
					ci.a = 1;
					cmd_line.colors.push_back(ci);
					argi += 3;
				}
				argi--;
			}
			break;
			case FRACTAL_4D_PARAM:
			{
				argi++;
				if(argi >= argc)
					Usage();
				cmd_line.use_4d = true;
				cmd_line.inc = atoi(argv[argi]);
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
			break;
			default:
			break;
		}
		argi++;
	}
	if(argi+3 == argc-1)
	{
		cmd_line.region_size = atoi(argv[argi]);
		cmd_line.isovalue = atof(argv[argi+1]);
		cmd_line.in_mesh = argv[argi+2];
		cmd_line.in_grid = argv[argi+3];
	}
	else
		Usage();
}
