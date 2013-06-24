#ifndef GRID_ALGORITHMS_H
#define GRID_ALGORITHMS_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include "grid.h"
#include "fractalgrid4d.h"
#include "fractalgrid.h"

using std::vector;
using std::queue;
using std::min;
using std::max;

struct Edge
{
	int x0, y0, z0;
	int x1, y1, z1;
};

class GridAlgorithms
{
private:
	static bool IsValid(int x, int y, int z, Grid& g);
public:
	static float FractalDimension(int cubes, int sub_cubes);
	static double FractalDimension4D(Grid& g, float inc, bool noz);
	static long IntersectedCubes4D(Grid& g, int inc, bool noz);
	static vector<int> IntersectedCubes(Grid& g);
	static vector<int> IntersectedEdges(Grid& g);
	static vector<float> FractalValues(Grid& g);
	static queue<Edge> HighFractalEdges(Grid& g, FractalGrid4D fg, float iso, float dim_limit);
	static void SmoothSurface(Grid& g, int region, int inc, float iso, float dim_limit);
};

#endif
