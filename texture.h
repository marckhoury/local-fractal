#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>

struct Color
{
    float r,g,b,a;
};

class Texture
{
public:
    Texture(std::vector<Color>& colors);
    Color operator[](float coord);

private:
    std::vector<Color> colors;
    float lerp(float l, float r, float t);
};

#endif
