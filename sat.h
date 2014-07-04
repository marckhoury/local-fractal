#ifndef SAT_H
#define SAT_H

#include <vector>
#include <iostream>

class Grid;

class SAT
{
public:
    SAT(Grid& g, float isovalue);
    ~SAT();

    int region_count(int x, int y, int z, int n);
    friend std::ostream& operator<<(std::ostream& os, SAT& sat);

private:
    std::vector<int> table;
    float isovalue;
    size_t axis[3];
    
    int index(int x, int y, int z);
    int box_count(Grid& g, int i, int j, int k);
};

#endif
