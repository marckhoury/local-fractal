#ifndef FRACTAL_MEDIAN_H
#define FRACTAL_MEDIAN_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "utility.h"
#include "filter.h"
#include "booleangrid.h"

using std::vector;
using std::cout;
using std::endl;

class FractalMedianFilter : public Filter
{
private:
	int neighborhood[27];
	float iso;
	int region_size;
	float dim;
public:
	FractalMedianFilter(int region, float iso, float dim_limit);
	virtual void Apply(Grid& g);
};

#endif
