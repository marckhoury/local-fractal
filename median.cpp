#include "median.h"

void MedianFilter::Apply(Grid& g)
{
	vector<int> filtered,neighbors;
	int neighborhood[27];
	int count = 0;
	filtered.resize(g.NumVertices());
	neighbors.resize(27);
	for(int z = -1; z <= 1; z++)
		for(int y = -1; y <= 1; y++)
			for(int x = -1; x <= 1; x++)
				neighborhood[count++] = g.Index(x,y,z);
	
	for(int z = 0; z < g.Axis(2); z++)
	{
		for(int y = 0; y < g.Axis(1); y++)
		{
			for(int x = 0; x < g.Axis(0); x++)
			{
				int iv = g.Index(x,y,z);
				if(x == 0 or y == 0 or z == 0
					or x == g.Axis(0)-1 or y == g.Axis(1)-1 or z == g.Axis(2)-1)
					filtered[iv] = g[iv];
				else
				{
					for(int i = 0; i < 27; i++)
						neighbors[i] = g[iv+neighborhood[i]];
					sort(neighbors.begin(),neighbors.end());
					filtered[iv] = neighbors[13];
				}
			}
		}
	}
	for(int i = 0; i < filtered.size(); i++)
		g[i] = filtered[i];

	g.SetName(g.ToString()+".median");
}
