#ifndef MEDIAN_H
#define MEDIAN_H

#include "filter.h"

class MedianFilter : public Filter
{
public:
	virtual void apply(Grid& g);
};

#endif
