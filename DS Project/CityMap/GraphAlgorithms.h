#pragma once
#include "Graph.h"

class GraphAlgorithm
{
public:
	GraphAlgorithm(Graph& _graph) : graph(_graph) {}
	void setSize(std::size_t _size) { size = _size; }
	std::size_t getSize() const { return size; }

	// Normal dijkstra
	void dijkstra(std::size_t startingPoint);

	// dijkstra for more than one path
	std::vector<int> dijkstra(std::size_t startingPoint, std::size_t endingPoint, std::vector<Path>* graph);

	// algorithm that finds the shortest 3 paths
	// explanation in definition
	std::vector<std::vector<int>> YenAlgorithmForThreePaths(std::size_t startingPoint, std::size_t endingPoint);


	
	bool isCyclic(int node);

	// Checks if a path exists to node from lastly used node for dijkstra
	bool checkPath(std::size_t to) { return !(distance[to] == INT_MAX); }

	// Function to fill visited and distance arrays with default values
	void fillArrays(std::size_t startingPoint);


	std::vector<Path>* getGraphVector() const { return graph.getGraph(); }
	Graph getGraph() const { return graph; }


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
protected:
	Graph graph;
	std::size_t size;
	bool visited[MAXSIZE];
	unsigned int distance[MAXSIZE];


	bool isCyclicUtil(int v, bool *recStack);
};






class Paths
{
public:
	Paths() = default;
	void setStartEnd(int start, int end)
	{
		startingPoint = start;
		endingPoint = end;
	}
	int firstPath(std::vector<std::vector<int>>& paths, GraphAlgorithm graph);
	int secondPath(std::vector<std::vector<int>>& paths, GraphAlgorithm graph, int edgeToRemove);
	void thirdPath(std::vector<std::vector<int>>& paths, GraphAlgorithm graph, int edgeToRemove);


	std::vector<int> getFirst() { return first; }
	std::vector<int> getSecond() { return second; }
	std::vector<int> getThird() { return third; }

	// PROBLEM WITH THIRD PATH
	/*
	if we have 1-2-3-4-5 as shortest path
	remove 2 and maybe find 1-3-4-5
	if found then keep removing from first until u find third
	aka remove 3 and have 1-2-4-5 if possible or 1-4-5
	Once all nodes have been removed once(excluding starting and ending)
	from first shortest path AND no third path has been found
	THEN we start removing nodes from second shortest path.
	repeat same process that was used with first-second paths but
	remove from second until u find third (if any)

	source:
	https://en.wikipedia.org/wiki/Yen%27s_algorithm
	https://stackoverflow.com/a/53529719/14309138

	PROBLEM WITH THIRD PATH:
	D -> E, E -> F,
	D -> A, A -> E,
	E -> B, B -> F
	If we have
	A  B
	 \  \
	^ \^ \
	D> E >F
	Assuming D is start and F is finish
	first path would be
	D E F
	second path would be
	D A E F
	now if we remove D -> A (node from second path)
	algoritm will go D E F aka going through the first shortest
	so we need to remove D -> E as well
	which would turn this into an impossible path
	then we remove A -> E and as we are already on A its also impossible
	then we remove E -> F and path would be D A E B F

	In short when removing the connection between first and second node in second path
	remove connection between first and second node in first path
	to force dijkstra to go into another node
	*/
private:
	std::size_t startingPoint;
	std::size_t endingPoint;
	std::vector<int> first;
	std::vector<int> second;
	std::vector<int> third;
};

struct Subset 
{
public:
	int parent;
	int rank;
};