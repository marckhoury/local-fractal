#ifndef SAT4D_H
#define SAT4D_H

#include <vector>

class Grid;

class SAT4D
{
public:
    SAT4D(Grid& g, int increment);
    ~SAT4D();

    long region_count(int x, int y, int z, int n);
    friend std::ostream& operator<<(std::ostream& os, SAT4D& sat);

private:
    std::vector<long> table;
    size_t axis[3];
    int min_sv, max_sv;
    int inc;
    
    int index(int x, int y, int z);
    long intersected_4d(Grid&g, int iv);
    long box_count(Grid& g, int i, int j, int k);
};

#endif
