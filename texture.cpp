#include "texture.h"

using std::vector;

Texture::Texture(vector<Color>& colors) :
    colors(colors)
{}

Color Texture::operator[](float coord)
{
    Color c,c0,c1;
    int i = coord / (1.0 / (colors.size() - 1));
    if (i == colors.size()-1) {
        i--;
    }
    c0 = colors[i]; 
    c1 = colors[i+1];
    coord = (coord - static_cast<float>(i) / (colors.size() - 1)) / (1.0 / (colors.size() - 1)); //Normalize to [0,1] for target interval
    c.r = lerp(c0.r,c1.r,coord);
    c.g = lerp(c0.g,c1.g,coord);
    c.b = lerp(c0.b,c1.b,coord);
    c.a = lerp(c0.a,c1.a,coord);
    return c;
}

float Texture::lerp(float l, float r, float t)
{
    return (1-t)*l + t*r;
}
