#include "TMXLayer.h"

TMXLayer::TMXLayer(std::string name, int width,
	               int height)
{
	this->name = name;
	this->width = width;
	this->height = height;
}

TMXLayer::~TMXLayer()
{
}
