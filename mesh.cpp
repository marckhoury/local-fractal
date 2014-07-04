#include "mesh.h"

#include <sstream>

using std::cerr;
using std::endl;
using std::ifstream;
using std::ostream;
using std::string;
using std::stringstream;
using std::vector;

Mesh::Mesh()
{
	name = "default";
}

Mesh::Mesh(string& file)
{
	name = file.substr(file.find_last_of("/")+1);
	name = name.substr(0,name.length()-4);
	read_off(file);
}

Vertex Mesh::get_vertex(int i)
{
	return vertices[i];
}

Triangle Mesh::get_triangle(int i)
{
	return triangles[i];
}

int Mesh::vertex_count()
{
	return vertices.size();
}

int Mesh::triangle_count()
{
	return triangles.size();
}

string Mesh::to_string()
{
	return name;
}

void Mesh::get_next_line(ifstream& in, string& line)
{
	getline(in,line);
	while (line.length() == 0 || line[0] == '#') {
		getline(in,line);
    }
}

void Mesh::read_off(string& file)
{
	ifstream in(file.c_str());
	int vert_count, face_count, edge_count;
	stringstream ss;
	string line;

	get_next_line(in,line);
	if (line.find("OFF") == -1) {
		cerr << "Unrecognized OFF file header " << line << endl;
		exit(0);
	}
	
    get_next_line(in,line);
	ss << line;
	ss >> vert_count >> face_count >> edge_count;
	
    vertices.resize(vert_count);
	triangles.resize(face_count);
	
    for (int i = 0; i < vert_count; i++) {
		Vertex v;
		get_next_line(in,line);
		ss.clear();
		ss << line;
		ss >> v.x >> v.y >> v.z;
		vertices[i] = v;
	}
	
    for (int i = 0; i < face_count; i++) {
		Triangle t;
		int dim;
		get_next_line(in,line);
		ss.clear();
		ss << line;
		ss >> dim;
		if(dim != 3) {
			cerr << "Only triangular mesh elements are supported" << endl;
			exit(0);
		}
		ss >> t.v[0] >> t.v[1] >> t.v[2];
		triangles[i] = t;
	}
}

ostream& operator<<(ostream& os, Mesh& m)
{
	os << m.vertex_count() << " " << m.triangle_count() << endl;
	for(int i = 0; i < m.vertex_count(); i++) {
		os << m.vertices[i].x << " " << m.vertices[i].y << " " << m.vertices[i].z << endl;
    }
	for(int i = 0; i < m.triangle_count(); i++) {
		os << m.triangles[i].v[0] << " " << m.triangles[i].v[1] << " " << m.triangles[i].v[2] << endl;
    }
	return os;
}
