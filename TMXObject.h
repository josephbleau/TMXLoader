#ifndef TMXOBJECT_H
#define TMXOBJECT_H

#include <string>
#include <map>

class TMXObject
{
public:
	std::string name;
	std::string type;
	int x,y,w,h,gid;

	std::map<std::string, std::string> properties;

	TMXObject(std::string name, std::string type, 
		      int x, int y, int w, int h, int gid = -1);
	~TMXObject();
};

#endif 