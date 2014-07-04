#ifndef FRACTAL_GRID_4D_H
#define FRACTAL_GRID_4D_H

#include <iostream>
#include <string>
#include <vector>

class Grid;

class FractalGrid4D
{
public:	
	FractalGrid4D();
	FractalGrid4D(Grid& g, int region_size, int inc);
	FractalGrid4D(const FractalGrid4D& rhs);
	~FractalGrid4D();
	
	size_t cube_count();
	size_t vertex_count();
	size_t get_axis(int i);
	bool has_spacing();
	double get_spacing(int i);
	size_t index(size_t x, size_t y, size_t z);
	float& operator[](int index);
	std::string to_string();
	void write_nrrd();
	FractalGrid4D& operator=(const FractalGrid4D& rhs);
	friend std::ostream& operator<<(std::ostream& os, FractalGrid4D& g); 

private:
	std::vector<float> grid;
	size_t axis[3];
	bool spacing_flag;
	double spacing[3];
	std::string name;
	int region_size;
	int inc;

	void build_grid(Grid& g);
	void compute_offset(int offset[8]);
};

#endif
