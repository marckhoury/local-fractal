#include "fractalgaussian.h"

FractalGaussianFilter::FractalGaussianFilter(int region, float isovalue, float dim_limit)
{
	region_size = region;
	iso = isovalue;
	dim = dim_limit;
}

void FractalGaussianFilter::Apply(Grid& g)
{
	BooleanGrid bg(g,region_size,iso,dim);
	vector<int> filtered;
	vector<int> neighbors;
	int weight[27] = {1,2,1,2,4,2,1,2,1,
			  2,4,2,4,8,4,2,4,2,
			  1,2,1,2,4,2,1,2,1};
	int filtered_vert_count = 0;
	int count = 0;
	filtered.resize(g.NumVertices());
	neighbors.resize(27);
	for(int i = 0; i < filtered.size(); i++) //Using -1 to mark unfiltered vertices
		filtered[i] = -1;
	for(int z = -1; z <= 1; z++)
		for(int y = -1; y <= 1; y++)
			for(int x = -1; x <= 1; x++)
				neighborhood[count++] = g.Index(x,y,z);

	for(int z = 0; z < bg.Axis(2); z++)
	{
		for(int y = 0; y < bg.Axis(1); y++)
		{
			for(int x = 0; x < bg.Axis(0); x++)
			{
				if(bg[bg.Index(x,y,z)])
				{
					int iv = g.Index(x,y,z);
					if(x == 0 or y == 0 or z == 0
						or x == g.Axis(0)-1 or y == g.Axis(1)-1 or z == g.Axis(2)-1)
						filtered[iv] = g[iv];
					else
					{
						int total = 0;
						for(int i = 0; i < 27; i++)
							total += weight[i]*g[iv+neighborhood[i]];
						filtered[iv] = total/64;
					}
				}
			}
		}
	}

	for(int i = 0; i < filtered.size(); i++)
	{
		if(filtered[i] != -1)
		{
			g[i] = filtered[i];
			filtered_vert_count++;
		}
	}
	cout << "Percentage of grid " << g.ToString() << " filtered: " << (double)filtered_vert_count/filtered.size()*100 << endl;
	g.SetName(g.ToString() + ".rs=" + str(region_size) + ".iso=" + str(iso) + ".dim=" + str(dim) + ".gaussian");
}
