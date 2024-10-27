#include "AnimationConfigParser.hpp"

inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void trimString(std::string& s)
{
	ltrim(s);
	rtrim(s);
}

void split(std::string &line, std::vector<std::string> &words, char div)
{
	size_t start = 0, end = 0;
	words.clear();
	if (line.empty())
		return;
	while (end < line.size())
	{
		while (start < line.size() && line[start] == div)
			start++;
		end = start;
		while (end < line.size() && line[end] != div)
			end++;
		if (end - start > 0)
			words.push_back(line.substr(start, end - start));
		start = end;
	}
}

bool AnimationConfigParser::parseConfig(const std::string &filename, std::unordered_map<std::string, std::string>& mp)
{	
	std::string line;
	std::vector<std::string> words;
	std::ifstream file(filename);

	if (file.is_open() == false)
	{
		std::cout << "File error: " << filename << std::endl;
		return false;
	}

	while (getline(file, line))
	{
		if (line.empty())
			continue;
		split(line, words, ' ');
		if (words.size() != 3)
		{
			std::cout << "config error in file: " << filename << std::endl;
			return false;
		}
		mp[words[0]] = words[2];
	}
	return validateConfig(mp);
}

bool AnimationConfigParser::validateConfig(std::unordered_map<std::string, std::string> &mp)
{
	if (mp.count("idle") == 0 || mp["idle"].empty())
		return false;
	if (mp.count("jump") == 0 || mp["jump"].empty())
		return false;
	if (mp.count("walk") == 0 || mp["walk"].empty())
		return false;
	if (mp.count("kick") == 0 || mp["kick"].empty())
		return false;
	if (mp.count("model") == 0 || mp["model"].empty())
		return false;
	return true;
}
