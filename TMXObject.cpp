#include "TMXObject.h"

TMXObject::TMXObject(std::string name, std::string type,
	                 int x, int y, int w, int h, int gid)
{
	this->name = name;
	this->type = type;
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->gid = gid;
}

TMXObject::~TMXObject()
{
}