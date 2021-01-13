#pragma once
#include <unordered_map>

class Map
{
public:
	Map() = default;

	void addStreet(std::string str);

	int getCorrespondingNode(const std::string& index) { return mapStreets[index]; }
	std::string getCorrespondingStreet(int index) { return mapNodes[index]; }
	unsigned int getSize() { return uniqueIntersections; }
private:
	std::unordered_map<std::string, int> mapStreets;
	std::unordered_map<int, std::string> mapNodes;
	unsigned int uniqueIntersections = 1;
};