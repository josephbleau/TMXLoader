#include "TMXTileset.h"

TMXTileset::TMXTileset(int firstgid, std::string name,
	                   int tilewidth, int tileheight)
{
	this->firstgid = firstgid;
	this->name = name;
	this->tilewidth = tilewidth;
	this->tileheight = tileheight;
}

TMXTileset::~TMXTileset()
{
}