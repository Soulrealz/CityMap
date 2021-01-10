#include <fstream>
#include <sstream>
#include <unordered_map>
//#include <string>

#include "GraphAlgorithms.h"

void addToMap(std::unordered_map<std::string, int>& map, std::string& str, unsigned int& index)
{
	// If a unique intersection is added
	// .at(str) will return invalid index exception
	// by catching it we create new object which map holds
	// second parameter is which index its node corresponds to
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
		// Access string on position 0 (file input)
		// And access it's i-th element
		if (strArr[0][i] != ' ')
		{
			characterCount++;
		}
		else
		{
			// [1] and [2] are first and second word

			// Using wordCount to check which word from the line we are on currently
			if (wordCount == 0)
			{
				// Extracting intersection name from line input
				strArr[1] = takeIntersection(strArr[0], begIndex, characterCount);
				// Remembering from where next word starts
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

	// [3] is length/weight
	strArr[3] = takeIntersection(strArr[0], begIndex, characterCount);
	begIndex = i + 1;
}
int init(std::ifstream& infile, Graph& graph, std::unordered_map<std::string, int>& map)
{
	// How many different intersecitons there are
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
		//
		// Example:
		// Megas XLR 150
		// str    = 0 = Megas XLR 150
		// first  = 1 = Megas
		// second = 2 = XLR
		// length = 3 = 150
		std::string strArr[] = { str, first, second, length };

		// Reading singular line of input
		readStreet(strArr, begIndex);

		// Making the "150" string into the integer 150
		std::stringstream sstream(strArr[3]);
		int num = 0;
		sstream >> num;

		// Adding both intersections
		addToMap(map, strArr[1], uniqueIntersections);
		addToMap(map, strArr[2], uniqueIntersections);

		// Adding path
		graph.addPath(map[strArr[1]], map[strArr[2]], num);

		// if input on this line is : Megas XLR 150
		// begIndex will be > size and below code does not matter
		// HOWEVER
		// if input on this line is : Coop Jamie 200 Kiva 100
		// readStreet would have read only Coop Jamie 200 so we need to read Kiva 100
		// below if checks for that
		if (begIndex < size)
		{
			std::size_t characterCount = 0;
			num = 0;
			second.clear();
			length.clear();
			
			// Thanks to saving begIndex earlier on we know from where to pickup
			for (std::size_t i = begIndex; i < size; i++)
			{
				if (str[i] != ' ')
				{
					characterCount++;
				}
				else
				{
					// First word would already have been read (Coop in example)
					// so we need to read second word (Kiva in example)
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
int main(int argc, char* argv[])
{
	Graph graph;
	GraphAlgorithm algos(graph);
	
	// Map to store node value of given intersection
	std::unordered_map<std::string, int> map;
	std::ifstream infile(argv[1], std::ios::in);
	
	// Unique node count
	// It's size + 1 because nodes start from 1 not 0 so we need to compensate
	int unique = init(infile, graph, map) + 1;
	algos.setSize(unique);


	graph.print();
	std::cout << "\n\n";

	algos.YenAlgorithmForThreePaths(1, 4);


	std::cin.get();
	try
	{
		std::string intersection;
		std::cout << "Intersection 1:";
		std::cin >> intersection;
		algos.dijkstra(map[intersection]);
		std::cout << "Intersection 2:";
		std::cin >> intersection;
		if (algos.checkPath(map[intersection]))
		{
			std::cout << "A path exists between those intersections\n";
		}
		else std::cout << "A path does not exist between those intersections\n";
	}
	catch (...)
	{
		std::cout << "Invalid intersection\n";
	}
}
