#ifndef FRACTAL_GAUSSIAN_H
#define FRACTAL_GAUSSIAN_H

#include "filter.h"

class FractalGaussianFilter : public Filter
{
public:
	FractalGaussianFilter(int region_size, float isovalue, float dim_limit);

	virtual void apply(Grid& g);

private:
	int neighborhood[27];
	float isovalue;
	int region_size;
	float dim_limit;
};

#endif
