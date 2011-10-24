#ifndef TMXMAP_H
#define TMXMAP_H

#include <string>
#include <vector>

#include "TMXTileset.h"
#include "TMXLayer.h"
#include "TMXLoader.h"
#include "TMXObjectGroup.h"

enum TMXOrientation { ORTHOGONAL = 0, ISOMETRIC };

class TMXMap
{
	friend class TMXLoader;

protected:
	std::string version;

	TMXOrientation orientation;

	int width, height, tileheight, tilewidth;

	std::vector<TMXTileset*> tilesets;
	std::vector<TMXLayer*> layers;
	std::vector<TMXObjectGroup*> objectgroups;

	std::string filename;

public:
	TMXMap(std::string filename);
	~TMXMap();

	void TMXLoad();
};


#endif