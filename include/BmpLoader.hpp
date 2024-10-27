#pragma once

#include <iostream>
#include <fstream>
#include <vector>

class BmpLoader
{
public:
	static bool loadBMPTexture(const char *filename, 
		std::vector<unsigned char>& data, int32_t& height, int32_t& width);
};