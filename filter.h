#ifndef FILTER_H
#define FILTER_H

#include "grid.h"

class Filter
{
public:
	virtual void Apply(Grid& g) = 0;
};

#endif
