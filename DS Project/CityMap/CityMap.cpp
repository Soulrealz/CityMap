#include <fstream>
#include <sstream>
#include <unordered_map>
//#include <string>

#include "Graph.h"

void addToMap(std::unordered_map<std::string, int>& map, std::string& str, unsigned int& index)
{
	try { map.at(str); }
	catch (...) { map[str] = index++; }
}
std::string takeIntersection(std::string& str, std::size_t& beg, std::size_t& num)
{
	std::string substr = str.substr(beg, num);
	num = 0;
	return substr;
}
void readStreet(std::string strArr[], std::size_t& begIndex)
{
	// 0 will always be file input
	// 1 and 2 will be first and second word
	// 3 will be the weight/length
	std::size_t size = strArr[0].size();
	std::size_t wordCount = 0;
	std::size_t characterCount = 0;
	std::size_t i = 0;
	for (; i < size; i++)
	{
		// THIS IS NOT A MATRIX !!!
		// Access string on position 0 (file input)
		// And access it's i-th element
		if (strArr[0][i] != ' ')
		{
			characterCount++;
		}
		else
		{
			if (wordCount == 0)
			{
				strArr[1] = takeIntersection(strArr[0], begIndex, characterCount);
				begIndex = i + 1;
			}
			else if (wordCount == 1)
			{
				strArr[2] = takeIntersection(strArr[0], begIndex, characterCount);
				begIndex = i + 1;
			}
			else break;
			++wordCount;
		}
	}
	strArr[3] = takeIntersection(strArr[0], begIndex, characterCount);
	begIndex = i + 1;
}
int init(std::ifstream& infile, Graph& graph, std::unordered_map<std::string, int>& map)
{
	unsigned int uniqueIntersections = 1;

	std::string str = "";
	while (std::getline(infile, str))
	{
		if (str == "" || str == "\n" || str == " ")
			continue;

		std::string first;
		std::string second;
		std::string length;

		std::size_t size = str.size();
		std::size_t begIndex = 0;

		// Work with string array for ease of passing arguments
		// 0 will always be file input
		// 1 and 2 will be first and second word
		// 3 will be the weight/length
		std::string strArr[] = { str, first, second, length };
		readStreet(strArr, begIndex);


		std::stringstream sstream(strArr[3]);
		int num = 0;
		sstream >> num;

		addToMap(map, strArr[1], uniqueIntersections);
		addToMap(map, strArr[2], uniqueIntersections);

		graph.addPath(map[strArr[1]], map[strArr[2]], num);

		if (begIndex < size)
		{
			std::size_t characterCount = 0;
			num = 0;
			second.clear();
			length.clear();
			for (std::size_t i = begIndex; i < size; i++)
			{
				if (str[i] != ' ')
				{
					characterCount++;
				}
				else
				{
					strArr[2] = takeIntersection(str, begIndex, characterCount);
					begIndex = i + 1;
				}
			}
			strArr[3] = takeIntersection(str, begIndex, characterCount);
			addToMap(map, strArr[2], uniqueIntersections);

			std::stringstream sstream(strArr[3]);
			sstream >> num;
			graph.addPath(map[strArr[1]], map[strArr[2]], num);
		}
	}

	return map.size();
}



std::vector<Path> Graph::graph[];
bool Graph::Algorithms::visited[];
unsigned int Graph::Algorithms::distance[];
int main(int argc, char* argv[])
{
	std::unordered_map<std::string, int> map;
	Graph graph;
	std::ifstream infile(argv[1], std::ios::in);
	
	int unique = init(infile, graph, map) + 1;
	graph.print();
	std::cout << "\n\n";

	std::string intersection;
	std::cout << "Intersection 1:";
	std::cin >> intersection;
	Graph::Algorithms::dijkstra(map[intersection]);
	std::cout << "Intersection 2:";
	std::cin >> intersection;
	std::cout << Graph::Algorithms::checkPath(map[intersection]);
	//Graph::Algorithms::print(unique);
}
