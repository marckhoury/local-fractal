#ifndef FILTER_H
#define FILTER_H

class Grid;

class Filter
{
public:
    virtual void apply(Grid& g) = 0;
};

#endif
