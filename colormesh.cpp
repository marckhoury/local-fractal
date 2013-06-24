#include "colormesh.h"

ColorMesh::ColorMesh(Mesh& mesh, FractalGrid& g, float l, float h)
{
	m = mesh;
	fg = g;
	use_4d = false;
	low = l;
	high = h;
	colors.resize(m.VertexCount());
}

ColorMesh::ColorMesh(Mesh& mesh, FractalGrid4D& g, float l, float h)
{
	m = mesh;
	fg4d = g;
	use_4d = true;
	low = l;
	high = h;
	colors.resize(m.VertexCount());
}

void ColorMesh::ApplyColor(Texture &tex)
{
	for(int i = 0; i < m.VertexCount(); i++)
	{
		Vertex v = m.GetVertex(i);
		float coord,loc_dim;
		float f1,f2,t;
		if(use_4d)
		{
			v.x = v.x/fg4d.Spacing(0);
			v.y = v.y/fg4d.Spacing(1);
			v.z = v.z/fg4d.Spacing(2);
		}
		else
		{
			v.x = v.x/fg.Spacing(0);
			v.y = v.y/fg.Spacing(1);
			v.z = v.z/fg.Spacing(2);
		}
		ChooseEdge(v,f1,f2,t);
		loc_dim = Lerp(f1,f2,t);
		if(loc_dim <= low)
			coord = 0;
		else if(loc_dim >= high)
			coord = 1;
		else
			coord = (loc_dim - low)/(high-low);
		colors[i] = tex[coord];
	}
	WriteOFF();
}

float ColorMesh::Lerp(float l, float r, float t)
{
	return (1-t)*l+t*r;
}

void ColorMesh::ChooseEdge(Vertex& v, float& l, float& r, float& t)
{
	if(use_4d)
		l = fg4d[fg4d.Index(v.x,v.y,v.z)];
	else
		l = fg[fg.Index(v.x,v.y,v.z)];
	if(v.x-(int)v.x >= EPSILON)
	{
		if(use_4d)
			r = fg4d[fg4d.Index(v.x+1,v.y,v.z)];
		else
			r = fg[fg.Index(v.x+1,v.y,v.z)];
		t = v.x-(int)v.x;
	}
	else if(v.y - (int)v.y >= EPSILON)
	{
		if(use_4d)
			r = fg4d[fg4d.Index(v.x,v.y+1,v.z)];
		else
			r = fg[fg.Index(v.x,v.y+1,v.z)];
		t = v.y-(int)v.y;
	}
	else
	{
		if(use_4d)
			r = fg4d[fg4d.Index(v.x,v.y,v.z+1)];
		else
			r = fg[fg.Index(v.x,v.y,v.z+1)];
		t = v.z-(int)v.z;
	}
}

void ColorMesh::WriteOFF()
{
	string out_file = m.ToString() + ".l=" + str(low) + ".h=" + str(high) +".fractal_color.off";
	ofstream out(out_file.c_str());
	out << "CCOFF" << endl;
	out << m.VertexCount() << " " << m.TriangleCount() <<  " 0" << endl;
	for(int i = 0; i < m.VertexCount(); i++)
	{
		Vertex v = m.GetVertex(i);
		Color c = colors[i];
		out << v.x << " " << v.y << " " << v.z << " ";
		out << c.r << " " << c.g << " " << c.b << " " << c.a << " ";		
		out << c.r << " " << c.g << " " << c.b << " " << c.a << endl;
	}
	for(int i = 0; i < m.TriangleCount(); i++)
	{
		Triangle t = m.GetTriangle(i);
		out << "3 " << t.v[0] << " " << t.v[1] << " " << t.v[2] << endl;
	}
	cout << "Wrote mesh " << out_file << endl;
	out.close();
}
