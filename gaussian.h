#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include "filter.h"

class GaussianFilter : public Filter
{
public:
	virtual void Apply(Grid& g);
};

#endif
