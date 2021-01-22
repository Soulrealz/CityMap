#pragma once
#include <unordered_map>

class Map
{
public:
	Map() = default;

	void addStreet(const std::string& str);

	int getCorrespondingNode(const std::string& index) { return mapStreets.at(index); }
	std::string getCorrespondingStreet(const int index) { return mapNodes.at(index); }
	unsigned int getSize() { return uniqueIntersections - 1; }
private:
	std::unordered_map<std::string, int> mapStreets;
	std::unordered_map<int, std::string> mapNodes;
	unsigned int uniqueIntersections = 1;
};