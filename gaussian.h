#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include "filter.h"

class GaussianFilter : public Filter
{
public:
    virtual void apply(Grid& g);
};

#endif
