#ifndef TMXTILESET_H
#define TMXTILESET_H

#include <string>
#include "TMXImage.h"

class TMXTileset
{
public:
	int firstgid;
	unsigned tilewidth;
	unsigned tileheight;

	std::string name;

	TMXImage *image;

	TMXTileset(int firstgid, std::string name,
		       int tilewidth, int tileheight);
	~TMXTileset();
};

#endif