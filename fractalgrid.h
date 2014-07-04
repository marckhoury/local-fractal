#ifndef FRACTAL_GRID_H
#define FRACTAL_GRID_H

#include <iostream>
#include <string>
#include <vector>

class Grid;

class FractalGrid
{
public: 
    FractalGrid();
    FractalGrid(Grid& g, int region_size, float isovalue);
    FractalGrid(const FractalGrid& rhs);
    ~FractalGrid();
    
    size_t cube_count();
    size_t vertex_count();
    size_t get_axis(int i);
    bool has_spacing();
    double get_spacing(int i);
    size_t index(size_t x, size_t y, size_t z);
    float& operator[](int i);
    std::string to_string();
    void write_nrrd();
    FractalGrid& operator=(const FractalGrid& rhs);
    friend std::ostream& operator<<(std::ostream& os, FractalGrid& g); 

private:
    std::vector<float> grid;
    size_t axis[3];
    bool spacing_flag;
    double spacing[3];
    std::string name;
    int region_size;
    float isovalue;

    void build_grid(Grid& g);
    void compute_offset(int offset[8]);
};

#endif
