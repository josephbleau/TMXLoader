#ifndef TMXIMAGE_H
#define TMXIMAGE_H

#include <string>

class TMXImage
{
public:
	std::string source;
	int width, height;

	TMXImage(std::string source, int width,
		     int height);
	~TMXImage();
};

#endif
