#include "Map.h"

void Map::addStreet(std::string str)
{
	// If a unique intersection is added
	// .at(str) will return invalid index exception
	// by catching it we create new object which map holds
	// second parameter is which index its node corresponds to
	try { mapStreets.at(str); }
	catch (...) 
	{ 
		mapStreets[str] = uniqueIntersections;
		mapNodes[uniqueIntersections++] = str;
	}
}
