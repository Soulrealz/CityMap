#include "Menu.h"

void menu::menuOptions()
{
	std::cout << "Options:\n1.Check if you can reach A from B:\n";
	std::cout << "2.Find 3 shortest routes between A and B:\n";
	std::cout << "3.Check for a cycle after being given A:\n";
	std::cout << "4.Check if you can reach ALL intersections from A:\n";
	std::cout << "5.Find all streets that go into a dead end:\n";
	std::cout << "6.Mark an intersection as blocked:\n";
	std::cout << "7.Interactive Mode:\n";
	std::cout << "9.Quit\n";
	std::cout << "\nChoose Your Option:";
}

void menu::caseOne(GraphAlgorithm& algos, Map& map)
{
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
}
void menu::caseTwo(GraphAlgorithm& algos, Map& map)
{
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
}
void menu::caseThree(GraphAlgorithm& algos, Map& map)
{
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
}
void menu::caseFour(GraphAlgorithm& algos, Map& map)
{
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
		for (std::size_t i = 1; i < size; i++)
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
}
void menu::caseFive(Graph& graph, GraphAlgorithm& algos, Map& map)
{
	int size = map.getSize() + 1;
	std::vector<std::string> deadEnds;
	bool isThereDeadEnd = false;
	for (int i = 1; i < size; i++)
	{
		if (!graph.isNotDeadEnd(i, size))
		{
			isThereDeadEnd = true;
			auto leadingStreets = graph.getIncomingStreets(i, size);
			int sizeLeading = leadingStreets.size();
			for (int j = 0; j < sizeLeading; j++)
			{
				std::string street = map.getCorrespondingStreet(leadingStreets[j]);
				street += " and ";
				street += map.getCorrespondingStreet(i);
				deadEnds.emplace_back(street);
			}
		}
	}

	size = deadEnds.size();
	if (isThereDeadEnd)
		for (int i = 0; i < size; i++)
			std::cout << deadEnds[i] << "\n";
	else std::cout << "There are no deadends.\n";
}
void menu::caseSix(Graph& graph, Map& map)
{
	std::cout << "Insert intersections that you would like to close:\n";
	std::cin.ignore();

	std::string closee;
	std::getline(std::cin, closee);

	std::vector<std::string> selected;
	std::stringstream ss(closee);
	std::string buffer;

	while (ss >> buffer)
		selected.emplace_back(buffer);

	InteractiveMode close;

	for (std::size_t i = 0; i < selected.size(); i++)
		close.close(selected[i], graph, map);
}
void menu::caseSeven(Graph& graph, GraphAlgorithm& algos, Map& map)
{
	std::cout << "Where would you like to start?\n";
	std::string loc;
	std::cin >> loc;

	try
	{
		map.getCorrespondingNode(loc);
		InteractiveMode inter(loc);
		inter.loop(graph, algos, map);
	}
	catch (...) { std::cout << "There is no such intersection.\n"; }
}


InteractiveMode::InteractiveMode()
{
	for (int i = 0; i < 100; i++)
	{
		std::fill(closed[i], closed[i] + SIZE, -1);
	}
}

void InteractiveMode::displayCommands() const
{
	std::cout << "\nCommand List:\n";
	std::cout << "location\n";
	std::cout << "change @\n";
	std::cout << "neighbours\n";
	std::cout << "move @\n";
	std::cout << "close @\n";
	std::cout << "open @\n";
	std::cout << "closed\n";
	std::cout << "tour\n";
	std::cout << "quit\n";
	std::cout << std::endl;
}
void InteractiveMode::loop(Graph& graph, GraphAlgorithm& algos, Map& map)
{
	displayCommands();
	bool isQuit = detectCommand(graph, algos, map);
	std::cout << "\n\n";

	if (!isQuit)
		loop(graph, algos, map);
}
bool InteractiveMode::detectCommand(Graph& graph, GraphAlgorithm& algos, Map& map)
{
	std::string command;
	std::getline(std::cin, command);
	std::cout << "\n";

	std::size_t size = command.size();
	std::size_t pos = 0;
	for (std::size_t i = 0; i < size; i++)
	{
		if (command[i] == ' ')
		{
			pos = i + 1;
			break;
		}
	}


	if (pos == 0)
	{
		if (command == "location")
			std::cout << location;
		else if (command == "neighbours")
			displayNeighbours(graph, map);
		else if (command == "closed")
			displayClosed();			
		else if (command == "tour")
			tour(algos, map);
		else if (command == "quit")
			return true;
	}
	else
	{
		std::string loc = command.substr(pos);
		command = command.substr(0, pos - 1);
		if (command == "change")
			change(loc, map);
		else if (command == "move")
			move(loc, algos, map);
		else if (command == "close")
			close(loc, graph, map);
		else if (command == "open")
			open(loc, graph, map);
	}

	std::cout << "\n";
	return false;
}

void InteractiveMode::displayNeighbours(Graph& graph, Map& map) const
{
	auto neighboursTo = graph.getIncomingStreets(map.getCorrespondingNode(location), map.getSize() + 1);
	auto neighboursFrom = graph.getOutgoingStreets(map.getCorrespondingNode(location), map.getSize() + 1);

	std::size_t neighbourSize = neighboursTo.size();
	for (std::size_t i = 0; i < neighbourSize; i++)
		std::cout << map.getCorrespondingStreet(neighboursTo[i]) << " ";

	neighbourSize = neighboursFrom.size();
	for (std::size_t i = 0; i < neighbourSize; i++)
		std::cout << map.getCorrespondingStreet(neighboursFrom[i]) << " ";
}
void InteractiveMode::close(std::string& closee, Graph& graph, Map& map)
{
	int index = -1;
	try { index = map.getCorrespondingNode(closee); }
	catch (...) { std::cout << "No such intersection.\n"; }

	if (index != -1)
	{
		auto adjList = graph.getAdjList();
		auto incoming = graph.getIncomingStreets(index, map.getSize() + 1);
		auto outgoing = graph.getOutgoingStreets(index, map.getSize() + 1);

		// Incoming
		std::size_t sizeInc = incoming.size();
		for (std::size_t i = 0; i < sizeInc; i++)
		{			
			// find node to save length
			for (std::size_t j = 0; j < adjList[incoming[i]].size(); j++)
			{
				// Now we know which node connections length to save
				if (adjList[incoming[i]][j].to == index)
				{
					// incoming[i] = from
					// index = to
					closed[incoming[i]][index] = adjList[incoming[i]][j].length;
					break;
				}
			}
			
			// Mark path as untraversable and close it down
			graph.adjustPath(incoming[i], index);
			graph.close(incoming[i], index);
		}
		
		// Outgoing
		std::size_t sizeOut = outgoing.size();
		for (std::size_t i = 0; i < sizeOut; i++)
		{
			closed[index][outgoing[i]] = adjList[index][i].length;
			graph.adjustPath(index, outgoing[i]);
			graph.close(index, outgoing[i]);
		}

		closedList.emplace_back(closee);
	}
}
void InteractiveMode::open(std::string& opener, Graph& graph, Map& map)
{
	int index = -1;
	try { index = map.getCorrespondingNode(opener); }
	catch (...) { std::cout << "No such intersection.\n"; }

	if (index != -1)
	{
		auto a = graph.getAdjList();

		// Outgoing
		std::size_t outSize = a[index].size();
		for (std::size_t i = 0; i < outSize; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				if (closed[index][j] > -1)
				{
					if (j > graph.getSize())
						break;
					graph.adjustPath(index, j, closed[index][j]);
					graph.open(index, j);
					closed[index][j] = -1;					
				}
			}			
		}
		// Incoming
		for (int i = 0; i < SIZE; i++)
		{
			if (closed[i][index] > -1)
			{
				if (i > graph.getSize())
					break;
				graph.adjustPath(i, index, closed[i][index]);
				graph.open(i, index);
				closed[i][index] = -1;
				
			}
		}

		for (std::size_t i = 0; i < closedList.size(); i++)
		{
			if (closedList[i] == opener)
			{
				closedList[i] = closedList[closedList.size() - 1];
				closedList.pop_back();
				break;
			}
		}

	}
}
void InteractiveMode::displayClosed() const
{
	std::size_t size = closedList.size();
	for (std::size_t i = 0; i < size; i++)
		std::cout << closedList[i] << " ";
	std::cout << "\n";
}
void InteractiveMode::change(std::string& loc, Map& map)
{
	try 
	{ 
		map.getCorrespondingNode(loc); 
		location = loc;
	}
	catch (...) { std::cout << "No such intersection.\n"; }
}
void InteractiveMode::move(std::string& loc, GraphAlgorithm& algos, Map& map)
{
	int index = -1;
	try { index = map.getCorrespondingNode(loc); }
	catch (...) { std::cout << "No such intersection.\n"; }

	if (index != -1)
	{
		auto path = algos.dijkstra(map.getCorrespondingNode(location), index, algos.getGraphVector());
		if (algos.checkPath(index))
		{
			location = loc;
			std::size_t size = path.size();
			for (std::size_t i = 0; i < size; i++)
			{
				std::cout << map.getCorrespondingStreet(path[size - i - 1]) << " ";
			}
		}
		else std::cout << loc << " cannot be reached from " << location;
	}
}
void InteractiveMode::tour(GraphAlgorithm& algos, Map& map) const
{
	int node = map.getCorrespondingNode(location);
	algos.dijkstra(node);
	auto graph = algos.getGraphVector();

	bool canReachAll = true;
	std::size_t size = algos.getSize();
	for (std::size_t i = 1; i < size; i++)
	{
		if (i != node && !algos.checkPath(i))
		{
			canReachAll = false;
			break;
		}
	}

	if (canReachAll)
	{
		auto wtp = worldTourPath(node, algos.getGraph());

		size = wtp.size();
		for (std::size_t i = 0; i < size; i++)
		{
			std::cout << map.getCorrespondingStreet(wtp[i]) << " ";
		}
	}
	else std::cout << "Tour of current city layout is impossible.";
}

std::vector<int> InteractiveMode::worldTourPath(int node, Graph graph) const
{
	bool visited[SIZE];
	std::vector<int> wtp;

	std::fill(visited, visited + SIZE, false);

	WTPUtil(node, visited, graph, wtp);
	return wtp;
}
void InteractiveMode::WTPUtil(int v, bool* visited, Graph graph, std::vector<int>& wtp) const
{
	if (visited[v] == false)
	{
		wtp.emplace_back(v);
		visited[v] = true;
		auto grph = graph.getAdjList();

		for (std::size_t i = 0; i < grph[v].size(); i++)
		{
			if (!visited[grph[v][i].to])
				WTPUtil(grph[v][i].to, visited, graph, wtp);
		}

	}

	if (wtp.size() > 1)
	{
		wtp.emplace_back(v);
		if (wtp[wtp.size() - 1] == wtp[wtp.size() - 2])
			wtp.pop_back();
	}
	else wtp.emplace_back(v);
}