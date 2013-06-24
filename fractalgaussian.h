#ifndef FRACTAL_GAUSSIAN_H
#define FRACTAL_GAUSSIAN_H

#include <string>
#include <iostream>
#include <vector>
#include "filter.h"
#include "booleangrid.h"

using std::vector;
using std::cout;
using std::endl;

class FractalGaussianFilter : public Filter
{
private:
	int neighborhood[27];
	float iso;
	int region_size;
	float dim;
public:
	FractalGaussianFilter(int region, float iso, float dim_limit);
	virtual void Apply(Grid& g);
};

#endif
