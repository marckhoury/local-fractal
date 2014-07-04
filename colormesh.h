#ifndef COLORMESH_H
#define COLORMESH_H

#define EPSILON 1E-9

#include <fstream>
#include "texture.h"
#include "mesh.h"
#include "grid.h"
#include "fractalgrid4d.h"
#include "fractalgrid.h"
#include "utility.h"

using std::ofstream;
using std::vector;

class ColorMesh
{
public:
	ColorMesh(Mesh& mesh, FractalGrid& fg, float low, float high);
	ColorMesh(Mesh& mesh, FractalGrid4D& fg4d, float low, float high);
	void apply_color(Texture& tex);

private:
	Mesh mesh;
	FractalGrid fg;
	FractalGrid4D fg4d;
	bool use_4d;
	vector<Color> colors;
	float low, high;
	
	float lerp(float l, float r, float t);
	void choose_edge(Vertex& v, float& l, float& h, float& t);
	void write_off();
};
#endif
