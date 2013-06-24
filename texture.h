#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <iostream>
using std::vector;

struct Color
{
	float r,g,b,a;
};

class Texture
{
private:
	vector<Color> colors;
	float Lerp(float l, float r, float t);
public:
	Texture(vector<Color>& ci);
	Color operator[](float coord);
};

#endif
