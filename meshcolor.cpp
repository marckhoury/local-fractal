// /*******************************************\
// | Author: Marc Khoury
// |
// | Desc: Colors a triangular mesh to highlight
// |       areas of high fractal dimension.
// |       Outputs the new mesh in .off format.
// |
// | Dependencies: libITKNrrdIO, libz
// \********************************************/
#include <iostream>
#include "mesh.h"
#include "grid.h"
#include "fractalgrid4d.h"
#include "fractalgrid.h"
#include "texture.h"
#include "colormesh.h"
#include "colorcmdline.h"

using namespace std;

int main(int argc, char* argv[])
{
	CmdLine cmd_line;
	ParseCommandLine(argc,argv,cmd_line);

	Mesh mesh(cmd_line.in_mesh);
	Grid grid(cmd_line.in_grid);

	if(cmd_line.use_4d)
	{
		FractalGrid4D fg(grid,cmd_line.region_size,cmd_line.inc);
		Texture tex(cmd_line.colors);
		ColorMesh cm(mesh,fg,cmd_line.low_dim,cmd_line.high_dim);
		cm.ApplyColor(tex);
	}
	else
	{
		FractalGrid fg(grid,cmd_line.region_size,cmd_line.isovalue);
		Texture tex(cmd_line.colors);
		ColorMesh cm(mesh,fg,cmd_line.low_dim,cmd_line.high_dim);
		cm.ApplyColor(tex);
	}
	return 0;
}
