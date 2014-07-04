#ifndef FRACTAL_MEDIAN_H
#define FRACTAL_MEDIAN_H

#include "filter.h"

class FractalMedianFilter : public Filter
{
public:
    FractalMedianFilter(int region_size, float isovalue, float dim_limit);
    virtual void apply(Grid& g);

private:
    int neighborhood[27];
    float isovalue;
    int region_size;
    float dim_limit;
};
#endif
