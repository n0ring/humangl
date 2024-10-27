#pragma once

#include <vector> 
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "Animation.hpp"


class AnimationConfigParser
{
public:
	static bool parseConfig(const std::string& filename, std::unordered_map<std::string, std::string>& mp);
private:
	static bool validateConfig(std::unordered_map<std::string, std::string>& mp);
};