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
private:
	Mesh m;
	FractalGrid fg;
	FractalGrid4D fg4d;
	bool use_4d;
	vector<Color> colors;
	float low,high;
	
	float Lerp(float l, float r, float t);
	void ChooseEdge(Vertex& v, float& l, float& h, float& t);
	void WriteOFF();
public:
	ColorMesh(Mesh& mesh, FractalGrid& g, float l, float h);
	ColorMesh(Mesh& mesh, FractalGrid4D& g, float l, float h);
	void ApplyColor(Texture& tex);
};
#endif
