#ifndef TMXOBJECTGROUP_H
#define TMXOBJECTGROUP_H

#include <string>
#include <vector>

#include "TMXObject.h"

class TMXObjectGroup
{
public:
	std::string name;
	int width;
	int height;

	std::vector<TMXObject*> objects;

	TMXObjectGroup(std::string name, int width, int height);
	~TMXObjectGroup();
};

#endif