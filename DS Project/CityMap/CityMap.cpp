#include <fstream>
#include <sstream>
//#include <string>

#include "GraphAlgorithms.h"
#include "Map.h"

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
void init(std::ifstream& infile, Graph& graph, Map& map)
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
		map.addStreet(strArr[1]);
		map.addStreet(strArr[2]);
		//addToMap(map, strArr[1], uniqueIntersections);
		//addToMap(map, strArr[2], uniqueIntersections);

		// Adding path
		graph.addPath(map.getCorrespondingNode(strArr[1]), map.getCorrespondingNode(strArr[2]), num);

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
			//addToMap(map, strArr[2], uniqueIntersections);
			map.addStreet(strArr[2]);

			std::stringstream sstream(strArr[3]);
			sstream >> num;
			graph.addPath(map.getCorrespondingNode(strArr[1]), map.getCorrespondingNode(strArr[2]), num);
		}
	}
}

void menu(Graph& graph, GraphAlgorithm& algos, Map& map)
{
	int c = 0;
	while (c != 9)
	{
		std::cout << "Options:\n1.Check if you can reach A from B:\n";
		std::cout << "2.Find 3 shortest routes between A and B:\n";
		std::cout << "3.Check for a cycle after being given A:\n";
		std::cout << "4.Check if you can reach ALL intersections from A:\n";
		std::cout << "9.Quit\n";
		std::cout << "\nChoose Your Option:";
		std::cin >> c;

		switch (c)
		{
		case 1:
			std::cout << "Choose two intersections:\n";
			try
			{
				std::string intersectionOne;
				std::cout << "Intersection 1:";
				std::cin >> intersectionOne;
				std::cout << "Intersection 2:";
				std::string intersectionTwo;
				std::cin >> intersectionTwo;
				std::cout << "\n";
				algos.dijkstra(map.getCorrespondingNode(intersectionOne));

				if (algos.checkPath(map.getCorrespondingNode(intersectionTwo)))
					std::cout << "A path exists between those intersections\n";
				else std::cout << "A path does not exist between those intersections\n";
			}
			catch (...) { std::cout << "Invalid intersection\n"; }
			break;
		case 2:
			std::cout << "Choose two intersections:\n";
			try
			{
				std::string intersectionOne;
				std::cout << "Intersection 1:";
				std::cin >> intersectionOne;
				std::cout << "Intersection 2:";
				std::string intersectionTwo;
				std::cin >> intersectionTwo;
				std::cout << "\n";

				auto paths = algos.YenAlgorithmForThreePaths(map.getCorrespondingNode(intersectionOne), map.getCorrespondingNode(intersectionTwo));
				if (paths[0].size() || paths[1].size() || paths[2].size())
				{
					std::cout << "A path exists between those intersections\n";
					for (int i = 0; i < 3; i++)
					{
						int size = paths[i].size();
						for (int j = 0; j < size; j++)
						{
							std::cout << map.getCorrespondingStreet(paths[i][j]) << " ";
						}
						std::cout << "\n";
					}
				}
				else std::cout << "A path does not exist between those intersections\n";
			}
			catch (...) { std::cout << "Invalid intersection\n"; }
			break;
		case 3:
			std::cout << "Choose intersection:\n";
			try
			{
				std::string intersection;
				std::cout << "Intersection 1:";
				std::cin >> intersection;
				std::cout << "\n";

				if (algos.isCyclic(map.getCorrespondingNode(intersection)))
					std::cout << "Yes you can go around the city and come back to THIS node, there is a cycle.\n";
				else std::cout << "No there is no cycle\n";
			}
			catch (...) { std::cout << "Invalid intersection\n"; }
			break;
		case 4:
			std::cout << "Choose intersection:\n";
			try
			{
				std::string intersection;
				std::cout << "Intersection 1:";
				std::cin >> intersection;
				std::cout << "\n";

				int index = map.getCorrespondingNode(intersection);
				algos.dijkstra(index);

				bool canReachAll = true;
				std::size_t size = algos.getSize();
				for (int i = 1; i < size; i++)
				{
					if (i != index && !algos.checkPath(i))
					{
						canReachAll = false;
						break;
					}
				}

				if (canReachAll)
					std::cout << "Yes all intersections can be reached.\n";
				else std::cout << "Unfortunately not all intersections can be reached.\n";
			}
			catch (...) { std::cout << "Invalid intersection\n"; }
			break;
		}

		std::cout << "\n\n";
	}
	
}

std::vector<Path> Graph::graph[];
int main(int argc, char* argv[])
{
	Graph graph;
	GraphAlgorithm algos(graph);
	Map map;
	
	std::ifstream infile(argv[1], std::ios::in);
	
	init(infile, graph, map);
	// Unique node count
	int unique = map.getSize();
	algos.setSize(unique);


	menu(graph, algos, map);
}
