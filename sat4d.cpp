#include "sat4d.h"

SAT4D::SAT4D(Grid& g, int increment)
{
	axis[0] = g.Axis(0), axis[1] = g.Axis(1), axis[2] = g.Axis(2);
	inc = increment;
	table.resize(g.NumVertices());
	for(int i = 0; i < table.size(); i++)
		table[i] = -1;
	max_sv = g[0], min_sv = g[0];
	for(int i = 0; i < g.NumVertices(); i++)
	{
		if(max_sv < g[i])
			max_sv = g[i];
		if(min_sv > g[i])
			min_sv = g[i];
	}
	BoxCount(g,g.Axis(0)-1,g.Axis(1)-1,g.Axis(2)-1);
}

SAT4D::~SAT4D()
{
}

long SAT4D::RegionCount(int x, int y, int z, int n)
{
	int upper[3] = {x+n,y+n,z+n};
	int lower[3] = {x-n,y-n,z-n};
	int region[8];
	for(int i = 0; i < 3; i++)
	{
		lower[i] = lower[i] >= 0 ? lower[i] : 0;
		upper[i] = upper[i] < axis[i] ? upper[i] : axis[i]-1;
	}
	
	region[0] = Index(upper[0],upper[1],upper[2]);
	region[1] = Index(lower[0],upper[1],upper[2]);
	region[2] = Index(upper[0],lower[1],upper[2]);
	region[3] = Index(lower[0],lower[1],upper[2]);
	region[4] = Index(upper[0],upper[1],lower[2]);
	region[5] = Index(lower[0],upper[1],lower[2]);
	region[6] = Index(upper[0],lower[1],lower[2]);
	region[7] = Index(lower[0],lower[1],lower[2]);
	
	return table[region[0]] - table[region[1]] - table[region[4]] + table[region[5]] 
		- table[region[2]] + table[region[3]] + table[region[6]] - table[region[7]];
	
}

ostream& operator<<(ostream& os, SAT4D& sat)
{
	for(int z = 0; z < sat.axis[2]; z++)
	{
		for(int y = 0; y < sat.axis[1]; y++)
		{
			for(int x = 0; x < sat.axis[0]; x++)
			{
				os << sat.table[sat.Index(x,y,z)] << " ";
			}
			os << endl;
		}
		os << endl;
	}
}

int SAT4D::Index(int x, int y, int z)
{
	return x + y*axis[0] + z*axis[0]*axis[1];
}

long SAT4D::Intersected4D(Grid& g, int iv)
{
	int span_0, span_1;
	int x0, x1;
	g.MaxMin(iv,span_1,span_0);
	if(span_1-span_0 == 0)
	{
		return 1;
	}
	else 
	{
		x0 = (span_0-min_sv)/inc;
		if((span_1-min_sv)%inc == 0)
			x1 = (span_1-min_sv)/inc;
		else
			x1 = (int)(ceil((span_1-min_sv)/inc+0.5) + 0.5);
		return (x1-x0);
	}
}

long SAT4D::BoxCount(Grid& g, int i, int j, int k)
{
	int iv = Index(i,j,k);
	if(table[iv] != -1)
		return table[iv];
	else if(i == 0 or j == 0 or k == 0)
		table[iv] = 0;
	else
	{
		int ic = g.Index(i-1,j-1,k-1);
		long intersected = Intersected4D(g, ic);
		table[iv] = BoxCount(g,i-1,j,k) + BoxCount(g,i,j-1,k) + BoxCount(g,i,j,k-1)
				- BoxCount(g,i-1,j-1,k) - BoxCount(g,i,j-1,k-1) - BoxCount(g,i-1,j,k-1) 
				+ BoxCount(g,i-1,j-1,k-1) + intersected;
	}
	return table[iv];
}
