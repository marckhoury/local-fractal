#include "mesh.h"

Mesh::Mesh()
{
	name = "default";
}

Mesh::Mesh(string& file)
{
	name = file.substr(file.find_last_of("/")+1);
	name = name.substr(0,name.length()-4);
	ReadOFF(file);
}

Vertex Mesh::GetVertex(int i)
{
	return vertices[i];
}

Triangle Mesh::GetTriangle(int i)
{
	return triangles[i];
}

int Mesh::VertexCount()
{
	return vertices.size();
}

int Mesh::TriangleCount()
{
	return triangles.size();
}

string Mesh::ToString()
{
	return name;
}

void Mesh::GetNextLine(ifstream& in, string& line)
{
	getline(in,line);
	while(line.length() == 0 or line[0] == '#')
		getline(in,line);
}

void Mesh::ReadOFF(string& file)
{
	ifstream in(file.c_str());
	int vert_count, face_count, edge_count;
	stringstream ss;
	string line;
	GetNextLine(in,line);
	if(line.find("OFF") == -1)
	{
		cerr << "Unrecognized OFF file header " << line << endl;
		exit(0);
	}
	GetNextLine(in,line);
	ss << line;
	ss >> vert_count >> face_count >> edge_count;
	vertices.resize(vert_count);
	triangles.resize(face_count);
	for(int i = 0; i < vert_count; i++)
	{
		Vertex v;
		GetNextLine(in,line);
		ss.clear();
		ss << line;
		ss >> v.x >> v.y >> v.z;
		vertices[i] = v;
	}
	for(int i = 0; i < face_count; i++)
	{
		Triangle t;
		int dim;
		GetNextLine(in,line);
		ss.clear();
		ss << line;
		ss >> dim;
		if(dim != 3)
		{
			cerr << "Only triangular mesh elements are supported" << endl;
			exit(0);
		}
		ss >> t.v[0] >> t.v[1] >> t.v[2];
		triangles[i] = t;
	}
}

ostream& operator<<(ostream& os, Mesh& m)
{
	os << m.VertexCount() << " " << m.TriangleCount() << endl;
	for(int i = 0; i < m.VertexCount(); i++)
		os << m.vertices[i].x << " " << m.vertices[i].y << " " << m.vertices[i].z << endl;
	for(int i = 0; i < m.TriangleCount(); i++)
		os << m.triangles[i].v[0] << " " << m.triangles[i].v[1] << " " << m.triangles[i].v[2] << endl;
	return os;
}
