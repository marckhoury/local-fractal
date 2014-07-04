#ifndef BOOLEAN_GRID_H
#define BOOLEAN_GRID_H

#include <vector>
#include <iostream>
#include <string>

class Grid;

class BooleanGrid
{
public:
    BooleanGrid(Grid& g, int region_size, float isovalue, float dim_limit);
    BooleanGrid(const BooleanGrid& bg);
    ~BooleanGrid();
    
    void opening();
    void closing();
    size_t cube_count();
    size_t vertex_count();
    size_t get_axis(int i);
    size_t index(size_t x, size_t y, size_t z);
    bool operator[](int index);
    std::string to_string();
    void write_nrrd();
    BooleanGrid& operator=(const BooleanGrid& rhs);
    friend std::ostream& operator<<(std::ostream& os, BooleanGrid& g); 

private:
    std::vector<int> grid;
    size_t axis[3];
    std::string name;
    float isovalue;
    int region_size;
    float dim_limit;

    void build_grid(Grid& g);   
    void compute_offset(int offset[8]);
    void set_cube(int iv, int sv, int offset[8]);
    void erosion();
    void dilation();
};

#endif
