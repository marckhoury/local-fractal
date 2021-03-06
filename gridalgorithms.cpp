#include "gridalgorithms.h"

#include <algorithm>
#include <cmath>

#include "fractalgrid.h"
#include "fractalgrid4d.h"
#include "grid.h"
#include "utility.h"

using std::vector;
using std::queue;
using std::min;
using std::max;

float GridAlgorithms::fractal_dimension(int cubes, int sub_cubes)
{
    if (cubes == 0) {
        return 0;
    } else if (sub_cubes == 0) {
        return 3;
    } else if (log(static_cast<float>(cubes) / sub_cubes) / log(2) > 3) {
        return 3;
    } else {
        return log(static_cast<float>(cubes) / sub_cubes) / log(2);
    }
}

double GridAlgorithms::fractal_dimension_4d(Grid& g, float inc, bool noz)
{
    Grid subgrid = g.subsample(2);
    long cubes = intersected_cubes_4d(g,inc, noz);
    long sub_cubes = intersected_cubes_4d(subgrid,2*inc, noz);
    if (cubes == 0) {
        return 0;
    } else if(sub_cubes == 0) {
        return 4;
    } else {
        return log(static_cast<double>(cubes) / sub_cubes) / log(2);
    }
}

long GridAlgorithms::intersected_cubes_4d(Grid& g, int inc, bool noz)
{
    int max_sv = g[0], min_sv = g[0];
    long cubes = 0;
    for (int i = 0; i < g.vertex_count(); i++) {
        if (max_sv < g[i]) {
            max_sv = g[i];
        }
        if (min_sv > g[i]) {
            min_sv = g[i];
        }
    }
    for (int z = 0; z < g.get_axis(2)-1; z++) {
        for (int y = 0; y < g.get_axis(1)-1; y++) {
            for (int x = 0; x < g.get_axis(0)-1; x++) {
                int span_0, span_1;
                int x0, x1;
                g.max_min(g.index(x, y, z), span_1, span_0);
                if (span_1 - span_0 == 0) {
                    if (!(noz && span_1 == 0)) {
                        cubes++;
                    }
                } else {
                    x0 = (span_0 - min_sv) / inc;
                    if ((span_1 - min_sv) % inc == 0) {
                        x1 = (span_1-min_sv)/inc;
                    } else {
                        x1 = static_cast<int>(ceil((span_1 - min_sv) / inc + 0.5) + 0.5);
                    }
                    cubes += x1 - x0;
                }
            }
        }
    }
    return cubes;
}

vector<int> GridAlgorithms::intersected_cubes(Grid& g)
{
    vector<int> count;
    int max_sv = g[0], min_sv;
    for(int i = 1; i < g.vertex_count(); i++) {
        max_sv = max_sv < g[i] ? g[i] : max_sv;
    }
    count.resize(max_sv+1);
    for (int z = 0; z < g.get_axis(2)-1; z++) {
        for (int y = 0; y < g.get_axis(1)-1; y++) {
            for (int x = 0; x < g.get_axis(0)-1; x++) {
                g.max_min(g.index(x,y,z), max_sv, min_sv);
                for (int i = min_sv; i < max_sv; i++) {
                    count[i]++;
                }
            }
        }
    }
    return count;
}

void GridAlgorithms::smooth_surface(Grid& g, int region, int inc, float iso, float dim_limit)
{
    FractalGrid4D fg(g, region, inc);
    queue<Edge> edges = high_fractal_edges(g, fg, iso, dim_limit);
    float weight[27] = {0.125,0.25,0.125,0.25,0.5,0.25,0.125,0.25,0.125,
                        0.25,0.5,0.25,0.5,1.0,0.5,0.25,0.5,0.25,
                        0.125,0.25,0.125,0.25,0.5,0.25,0.125,0.25,0.125};
    int x_inc[] = {1,-1,0,0,0,0};
    int y_inc[] = {0,0,1,-1,0,0};
    int z_inc[] = {0,0,0,0,1,-1};
    int neighborhood[27];
    int count = 0;
    for (int z = -1; z <= 1; z++) { 
        for (int y = -1; y <= 1; y++) {
            for (int x = -1; x <= 1; x++) {
                neighborhood[count++] = g.index(x,y,z);
            }
        }
    }
    while (!edges.empty()) {
        Edge e = edges.front(); edges.pop();
        int iv = g.index(e.x1, e.y1, e.z1);
        if (g[iv] < iso || g[g.index(e.x0, e.y0, e.z0)] >= iso) {
            continue; // The edge is no longer bipolar due to a change in the grid
        }
        int delta = g[iv]-g[g.index(e.x0, e.y0, e.z0)];
        for (int i = 0; i < 27; i++) {
            int in = iv + neighborhood[i];
            g[in] = g[in] - weight[i]*delta;
            if (g[in] < 0) {
                g[in] = 0;
            }
        }
        int sv = g[iv];
        for (int i = 0; i < 6; i++) {
            int x_n = e.x1 + x_inc[i], y_n = e.y1 + y_inc[i], z_n = e.z1 + z_inc[i];
            int iu = g.index(x_n, y_n, z_n);
            if (is_valid(x_n, y_n, z_n, g)) { //Notice that f.v0 will always be the - vert and f.v1 will be +
                int su = g[iu];
                if (iso > sv && iso <= su) {
                    if (fg[iv] >= dim_limit || fg[iu] >= dim_limit) {
                        Edge f;
                        f.x0 = e.x1, f.y0 = e.y1, f.z0 = e.z1;
                        f.x1 = x_n, f.y1 = y_n, f.z1 = z_n;
                        edges.push(f);
                    }
                }
            }
        }
    }
    g.set_name(g.to_string() + ".rs=" + str(region) + ".iso=" + str(iso) + ".dim=" + str(dim_limit) + ".ss.4d");
}

queue<Edge> GridAlgorithms::high_fractal_edges(Grid& g, FractalGrid4D fg, float iso, float dim_limit)
{
    queue<Edge> edges;
    vector<bool> visited;
    queue<int> x,y,z;
    int max_sv = g[0];
    int x_inc[] = {1,-1,0,0,0,0};
    int y_inc[] = {0,0,1,-1,0,0};
    int z_inc[] = {0,0,0,0,1,-1};
    visited.resize(g.vertex_count());
    for (int i = 0; i < visited.size(); i++) {
        visited[i] = false;
    }
    x.push(0); y.push(0); z.push(0);
    visited[g.index(0,0,0)] = true;
    while (x.size() > 0) {
        int x_c = x.front(); x.pop();
        int y_c = y.front(); y.pop();
        int z_c = z.front(); z.pop();
        int iv = g.index(x_c,y_c,z_c);
        int sv = g[iv];

        for (int i = 0; i < 6; i++) {
            int x_n = x_c + x_inc[i], y_n = y_c + y_inc[i], z_n = z_c + z_inc[i];
            int iu = g.index(x_n, y_n, z_n);
            if (is_valid(x_n, y_n, z_n, g) && !visited[iu]) {
                int su = g[iu];
                if (iso > min(sv, su) && iso <= max(sv, su)) { //Is the edge bipolar
                    //This might have to be swicthed to match Rafe's definition of bipolar
                    if (fg[iv] >= dim_limit || fg[iu] >= dim_limit) { //Is it high fractal
                        Edge e;
                        if (sv < su) { //e.v0 has the - vert and e.v1 has the + vert
                            e.x0 = x_c, e.y0 = y_c, e.z0 = z_c;
                            e.x1 = x_n, e.y1 = y_n, e.z1 = z_n;
                        } else {
                            e.x0 = x_n, e.y0 = y_n, e.z0 = z_n;
                            e.x1 = x_c, e.y1 = y_c, e.z1 = z_c;
                        }
                        edges.push(e);
                    }
                }
                visited[iu] = true;     
                x.push(x_n);
                y.push(y_n);
                z.push(z_n);
            }
        }
    }
    return edges;
}

vector<int> GridAlgorithms::intersected_edges(Grid& g)
{
    vector<bool> visited;
    vector<int> edge_count;
    queue<int> x,y,z;
    int max_sv = g[0];
    int x_inc[] = {1,-1,0,0,0,0};
    int y_inc[] = {0,0,1,-1,0,0};
    int z_inc[] = {0,0,0,0,1,-1};
    
    visited.resize(g.vertex_count());
    for (int i = 0; i < visited.size(); i++) {
        visited[i] = false;
    }
    
    for (int i = 1; i < g.vertex_count(); i++) {
        max_sv = max_sv < g[i] ? g[i] : max_sv;
    }

    edge_count.resize(max_sv + 1);
    x.push(0); y.push(0); z.push(0);
    while (x.size() > 0) {
        int x_c = x.front(); x.pop();
        int y_c = y.front(); y.pop();
        int z_c = z.front(); z.pop();
        int iv = g.index(x_c, y_c, z_c);
        int sv = g[iv];
        if (visited[iv]) {
            continue;
        }
        visited[iv] = true;
        for (int i = 0; i < 6; i++) {
            int iu = g.index(x_c + x_inc[i], y_c + y_inc[i], z_c + z_inc[i]);
            if (is_valid(x_c + x_inc[i], y_c + y_inc[i], z_c + z_inc[i], g) && !visited[iu]) {
                int su = g[iu];
                int maxv = max(sv, su);
                for (int j = min(sv, su); j < maxv; j++) {
                    edge_count[j]++;
                }

                x.push(x_c + x_inc[i]);
                y.push(y_c + y_inc[i]);
                z.push(z_c + z_inc[i]);
            }
        }
    }
    return edge_count;
}

vector<float> GridAlgorithms::fractal_values(Grid& g)
{
    Grid subgrid = g.subsample(2);
    vector<int> cubes = intersected_cubes(g);
    vector<int> subcubes = intersected_cubes(subgrid);
    vector<float> fractal;
    fractal.resize(cubes.size());
    for (int i = 0; i < subcubes.size(); i++) {
        fractal[i] = fractal_dimension(cubes[i], subcubes[i]);
    }
    for (int i = subcubes.size(); i < cubes.size(); i++) {
        fractal[i] = fractal_dimension(cubes[i], 0);
    }
    return fractal;
}

bool GridAlgorithms::is_valid(int x, int y, int z, Grid& g)
{
    return x >= 0 && y >= 0 && z >= 0 && x < g.get_axis(0) && y < g.get_axis(1) && z < g.get_axis(2);
}
