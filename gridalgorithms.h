#ifndef GRID_ALGORITHMS_H
#define GRID_ALGORITHMS_H

#include <queue>
#include <vector>

class Grid;
class FractalGrid4D;

struct Edge
{
	int x0, y0, z0;
	int x1, y1, z1;
};

class GridAlgorithms
{
public:
	static float fractal_dimension(int cubes, int sub_cubes);
	static double fractal_dimension_4d(Grid& g, float inc, bool noz);
	static long intersected_cubes_4d(Grid& g, int inc, bool noz);
	static std::vector<int> intersected_cubes(Grid& g);
	static std::vector<int> intersected_edges(Grid& g);
	static std::vector<float> fractal_values(Grid& g);
	static std::queue<Edge> high_fractal_edges(Grid& g, FractalGrid4D fg, float iso, float dim_limit);
	static void smooth_surface(Grid& g, int region, int inc, float iso, float dim_limit);

private:
	static bool is_valid(int x, int y, int z, Grid& g);
};

#endif
