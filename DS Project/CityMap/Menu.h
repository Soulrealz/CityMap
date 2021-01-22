#pragma once
#include <iostream>
#include <sstream>

#include "GraphAlgorithms.h"
#include "Map.h"

class InteractiveMode;

namespace menu
{
	void menuOptions();

	void caseOne(GraphAlgorithm& algos, Map& map);
	void caseTwo(GraphAlgorithm& algos, Map& map);
	void caseThree(GraphAlgorithm& algos, Map& map);
	void caseFour(GraphAlgorithm& algos, Map& map);
	void caseFive(Graph& graph, GraphAlgorithm& algos, Map& map);
	void caseSix(Graph& graph, Map& map);
	void caseSeven(Graph& graph, GraphAlgorithm& algos, Map& map);
}

class InteractiveMode
{
public:
	InteractiveMode();
	InteractiveMode(std::string& loc) : location(loc) {}

	void loop(Graph& graph, GraphAlgorithm& algos, Map& map);
	bool detectCommand(Graph& graph, GraphAlgorithm& algos, Map& map);
	void close(std::string& closee, Graph& graph, Map& map);

private:
	std::string location;
	static std::vector<std::string> closedList;
	static const int SIZE = 150;
	static int closed[SIZE][SIZE];

	void displayCommands() const;

	void displayNeighbours(Graph& graph, Map& map) const;
	void open(std::string& opener, Graph& graph, Map& map);
	void displayClosed() const;
	void change(std::string& loc, Map& map);
	void move(std::string& loc, GraphAlgorithm& algos, Map& map);
	void tour(GraphAlgorithm& algos, Map& map) const;

	std::vector<int> worldTourPath(int node, Graph graph) const;
	void WTPUtil(int v, bool* visited, Graph graph, std::vector<int>& wtp) const;

};
