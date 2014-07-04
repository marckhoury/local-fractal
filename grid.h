#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <string>
#include <vector>

class Grid
{
public:
    Grid(std::string& file);
    Grid();
    Grid(const Grid& rhs);
    ~Grid();

    Grid subsample(int s);
    size_t cube_count();
    size_t vertex_count();
    size_t get_axis(int index);
    bool has_spacing();
    double get_spacing(int index);
    size_t index(size_t x, size_t y, size_t z);
    int& operator[](int index);
    bool intersected(int iv, float iso);
    void max_min(int iv, int& max_sv, int& min_sv);
    void set_name(std::string new_name);
    std::string to_string();
    void write_nrrd();
    Grid& operator=(const Grid& rhs);
    friend std::ostream& operator<<(std::ostream& os, Grid& g);

private:
    std::vector<int> grid;
    size_t axis[3];
    bool spacing_flag;
    double spacing[3];
    std::string name;
    int offset[8];

    void read_nrrd(std::string& infile, 
                   std::vector<int>& g, 
                   size_t axis_size[3], 
                   double grid_spacing[3]);
    void compute_offset();
};

#endif
