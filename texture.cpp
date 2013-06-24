#include "texture.h"

Texture::Texture(vector<Color>& ci)
{
	colors = ci;
}

Color Texture::operator[](float coord)
{
	Color c,c0,c1;
	int i = coord/(1.0/(colors.size()-1));
	if(i == colors.size()-1)
		i--;
	c0 = colors[i]; 
	c1 = colors[i+1];
	coord = (coord - (float)i/(colors.size()-1))/(1.0/(colors.size()-1)); //Normalize to [0,1] for target interval
	c.r = Lerp(c0.r,c1.r,coord);
	c.g = Lerp(c0.g,c1.g,coord);
	c.b = Lerp(c0.b,c1.b,coord);
	c.a = Lerp(c0.a,c1.a,coord);
	return c;
}

float Texture::Lerp(float l, float r, float t)
{
	return (1-t)*l + t*r;
}
