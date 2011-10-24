#include "TMXMap.h"

TMXMap::TMXMap(std::string filename)
{
	this->filename = filename;
}

TMXMap::~TMXMap()
{
}

void TMXMap::TMXLoad()
{
	TMXLoader load(this->filename, this);
	load.Parse();
}