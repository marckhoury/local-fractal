#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

using std::string;
using std::ifstream;
using std::vector;
using std::stringstream;
using std::cerr;
using std::endl;
using std::ostream;

struct Vertex
{
	float x,y,z;
};

struct Triangle
{
	int v[3];
};

class Mesh
{
private:
	vector<Vertex> vertices;
	vector<Triangle> triangles;
	string name;

	void GetNextLine(ifstream& in, string& line);
	void ReadOFF(string& file);
public:
	Mesh();
	Mesh(string& file);
	Vertex GetVertex(int i);
	Triangle GetTriangle(int i);
	int VertexCount();
	int TriangleCount();
	string ToString();
	friend ostream& operator<<(ostream& os, Mesh& m);
};

#endif
