#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Vertex
{
    float x, y, z;
};

struct Triangle
{
    int v[3];
};

class Mesh
{
public:
    Mesh();
    Mesh(std::string& file);
    Vertex get_vertex(int i);
    Triangle get_triangle(int i);
    int vertex_count();
    int triangle_count();
    std::string to_string();
    friend std::ostream& operator<<(std::ostream& os, Mesh& m);

private:
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
    std::string name;

    void get_next_line(std::ifstream& in, std::string& line);
    void read_off(std::string& file);
};

#endif
