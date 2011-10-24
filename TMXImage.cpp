#include "TMXImage.h"

TMXImage::TMXImage(std::string source, int width,
	               int height)
{
	this->source = source;
	this->width = width;
	this->height = height;
}

TMXImage::~TMXImage()
{
}