#ifndef TMXLOADER_H
#define TMXLOADER_H
#define _DEBUG_ 0

#include <string>
#include <iostream>
#include <vector>

#include <string.h>

#include <libxml/xmlreader.h>

#include "TMXTileset.h"
#include "TMXLayer.h"
#include "TMXImage.h"
#include "TMXObjectGroup.h"
#include "TMXObject.h"

class TMXMap;
class TMXLoader
{
private:
	xmlDocPtr doc_ptr;
	xmlNodePtr node_ptr;
	bool parsed;

	TMXTileset *last_tileset;
	TMXLayer *last_layer;
	TMXObject *last_object;
	TMXObjectGroup *last_object_group;
	TMXMap *map;

	void ParseMapTag(xmlNodePtr p);
	void ParseTilesetTag(xmlNodePtr p);
	void ParseImageTag(xmlNodePtr p);
	void ParseLayerTag(xmlNodePtr p);
	void ParseObjectGroupTag(xmlNodePtr p);
	void ParseObjectTag(xmlNodePtr p);
	void ParsePropertyTag(xmlNodePtr p);

	std::vector<int> ParseDataText(std::string data);

public:
	TMXLoader(std::string filename, TMXMap *map);
	~TMXLoader();

	void Parse();
};

#endif