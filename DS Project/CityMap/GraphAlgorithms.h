#pragma once
#include "Graph.h"

class GraphAlgorithm
{
public:
	GraphAlgorithm(Graph& _graph) : graph(_graph) {}
	void setSize(std::size_t _size) { size = _size; }

	// Normal dijkstra
	void dijkstra(std::size_t startingPoint);

	// dijkstra for more than one path
	std::vector<int> dijkstra(std::size_t startingPoint, std::size_t endingPoint, std::vector<Path>* graph);

	// algorithm that finds the shortest 3 paths
	// explanation in definition
	std::vector<int>* YenAlgorithmForThreePaths(std::size_t startingPoint, std::size_t endingPoint);

	// Checks if a path exists to node from lastly used node for dijkstra
	bool checkPath(std::size_t to) { return !(distance[to] == INT_MAX); }

	// Function to fill visited and distance arrays with default values
	void fillArrays(std::size_t startingPoint);

	// Printing shortest distance to every node according to most recently executed dijkstra
	void print() const
	{
		for (std::size_t i = 1; i < size; i++)
		{
			std::cout << i << " " << distance[i] << "\n";
		}
	}
	// Priting the actual shortest path from last used start/end nodes
	void printPath(std::vector<int> path) const
	{
		std::size_t size = path.size();
		for (std::size_t i = 0; i < size; i++)
		{
			std::cout << path[i] << " ";
		}
		std::cout << "\n";
	}
private:
	Graph graph;
	std::size_t size;
	bool visited[MAXSIZE];
	unsigned int distance[MAXSIZE];
};