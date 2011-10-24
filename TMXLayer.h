#ifndef TMXLAYER_H
#define TMXLAYER_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

class TMXLayer
{
public:
	int width;
	int height;

	std::string name;
	std::vector<int> data;       

	TMXLayer(std::string name, int width,
		     int height);
	~TMXLayer();
};

#endif