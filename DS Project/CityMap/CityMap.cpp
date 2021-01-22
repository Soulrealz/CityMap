#include <fstream>

#include "Menu.h"

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
			map.addStreet(strArr[2]);

			std::stringstream sstream(strArr[3]);
			sstream >> num;
			graph.addPath(map.getCorrespondingNode(strArr[1]), map.getCorrespondingNode(strArr[2]), num);
		}
	}

	graph.createMatrix(map.getSize() + 1);
}

void console(Graph& graph, GraphAlgorithm& algos, Map& map)
{
	int c = 0;
	while (c != 9)
	{
		menu::menuOptions();
		std::cin >> c;

		switch (c)
		{
		case 1:
			menu::caseOne(algos, map);
			break;
		case 2:
			menu::caseTwo(algos, map);
			break;
		case 3:
			menu::caseThree(algos, map);
			break;
		case 4:
			menu::caseFour(algos, map);
			break;
		case 5:
			menu::caseFive(graph, algos, map);
			break;
		case 6:
			menu::caseSix(graph, map);
			break;
		case 7:
			menu::caseSeven(graph, algos, map);
			break;
		}

		std::cout << "\n\n";
	}	
}

std::vector<std::string> InteractiveMode::closedList;
int InteractiveMode::closed[150][150];
std::vector<Path> Graph::graph[];
int main()
{
	// Declare objects
	Graph graph;
	GraphAlgorithm algos(graph);
	Map map;
	
	// Open file
	std::ifstream infile("Input.txt", std::ios::in);
	
	// Initialize map of the city
	init(infile, graph, map);

	for (std::size_t i = 1; i < map.getSize() + 1; i++)
	{
		std::cout << i << " - " << map.getCorrespondingStreet(i) << "\n";
	}

	// Unique node count
	algos.setSize(map.getSize());

	// Console output
	console(graph, algos, map);
}
