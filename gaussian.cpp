#include "gaussian.h"

void GaussianFilter::Apply(Grid& g)
{
	vector<int> filtered;
	int neighborhood[27];
	int count = 0;
	int weight[27] = {1,2,1,2,4,2,1,2,1,
			  2,4,2,4,8,4,2,4,2,
			  1,2,1,2,4,2,1,2,1};
	filtered.resize(g.NumVertices());
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
					int total = 0;
					for(int i = 0; i < 27; i++)
						total += weight[i]*g[iv+neighborhood[i]];
					filtered[iv] = total/64;
				}
			}
		}
	}
	for(int i = 0; i < filtered.size(); i++)
		g[i] = filtered[i];

	g.SetName(g.ToString() + ".gaussian");
}
