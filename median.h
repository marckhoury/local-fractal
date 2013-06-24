#ifndef MEDIAN_H
#define MEDIAN_H

#include <algorithm>
#include "utility.h"
#include "filter.h"
#include "grid.h"
#include "fractalgrid.h"

class MedianFilter : public Filter
{
public:
	virtual void Apply(Grid& g);
};

#endif
