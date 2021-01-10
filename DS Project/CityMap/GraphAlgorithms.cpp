#include <queue>
#include <limits.h>

#include "GraphAlgorithms.h"
#define pair std::pair<int, int>

void GraphAlgorithm::dijkstra(std::size_t startingPoint)
{
	// getting array of vectors
	auto graph = this->graph.getGraph();
	// priority queue to work with closest nodes according to length
	// defined pair to not write std::pair<int, int> 3 times
	std::priority_queue<pair, std::vector<pair>, std::greater<pair>> toVisit;

	// Reseting visited/distance arrays
	fillArrays(startingPoint);

	// Distance from starting node is always 0
	toVisit.push({ 0, startingPoint });

	while (!toVisit.empty())
	{
		// visit node
		int node = toVisit.top().second;
		toVisit.pop();

		// if visited skip
		if (visited[node])
			continue;
		
		// mark as visited
		visited[node] = true;
		for (Path neighbor : graph[node])
		{
			// if current node doesnt present shorter path, skip
			if (distance[node] + neighbor.length >= distance[neighbor.to])
				continue;

			// updating with new shortest path
			distance[neighbor.to] = distance[node] + neighbor.length;
			// adding this node for exploration
			toVisit.push({ distance[neighbor.to], neighbor.to });
		}
	}
}

std::vector<int> GraphAlgorithm::dijkstra(std::size_t startingPoint, std::size_t endingPoint, std::vector<Path>* graph)
{
	// parent array to know where we came from
	int parent[MAXSIZE];
	// if value is -1 at the end that means there is no path (or we are at starting node)
	std::fill(parent, parent + MAXSIZE, -1);

	std::priority_queue<pair, std::vector<pair>, std::greater<pair>> toVisit;

	fillArrays(startingPoint);
	toVisit.push({ 0, startingPoint });

	// starting node has no parent
	parent[startingPoint] = -1;

	while (!toVisit.empty())
	{
		int node = toVisit.top().second;
		toVisit.pop();

		if (visited[node])
			continue;

		visited[node] = true;
		for (Path neighbor : graph[node])
		{
			if (distance[node] + neighbor.length >= distance[neighbor.to])
				continue;

			distance[neighbor.to] = distance[node] + neighbor.length;
			toVisit.push({ distance[neighbor.to], neighbor.to });

			// set parent of new node to be current node
			parent[neighbor.to] = node;
		}
	}

	// place the actual path in a vector
	// WARNING PATH IS REVERSED
	// if path is 1-2-3-4
	// then vector will have it as 4-3-2-1
	std::vector<int> routes;
	routes.emplace_back(endingPoint);

	// take parent of target node
	int par = parent[endingPoint];

	// if -1 there is no path
	if (par != -1)
	{
		// repeat until starting node is found
		while (par != -1)
		{
			routes.emplace_back(par);
			par = parent[par];
		}
	}
	else routes.clear();


	return routes;
}

std::vector<int>* GraphAlgorithm::YenAlgorithmForThreePaths(std::size_t startingPoint, std::size_t endingPoint)
{
	//Space Complexity - KN where K is amount of desired paths - 1
	//Time Complexity - O(KN ^ 3) 
	std::cout << "Printing shortest paths between " << startingPoint << " and " << endingPoint << "\n";
	
	// normal dijkstra to find shortest path
	auto first = dijkstra(startingPoint, endingPoint, graph.getGraph());
	if (first.size() == 0)
	{
		std::cout << "There is no path\n";
		return;
	}
	//print();
	printPath(first);
	//std::cout << "\n\n\n";


	
	// first.size() - 1 would give starting node so we do - 2
	// to remove the second node in the path (as we cant remove the starting point)
	// if 1-2-3-4 remove 2 from path
	int edgeToRemove = first.size() - 2;
	auto second = first;
	second.clear();
	while (edgeToRemove > 1)
	{
		// We want to edit the graph so we make a copy to avoid changing the original
		auto originalGraph = graph;

		// remove a different node from first shortest path until a second shortest one is found (if there is any)
		originalGraph.removePath(startingPoint, first[edgeToRemove]);
		second = dijkstra(startingPoint, endingPoint, originalGraph.getGraph());
		
		// If a path has been found already
		if (second.size() != 0)
			break;
		edgeToRemove--;
	}

	if (second.size() == 0)
	{
		std::cout << "There is no second path\n";
		return;
	}
	//print();
	printPath(second);
	//std::cout << "\n\n\n";

	
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

	edgeToRemove = second.size() - 2;
	auto third = second;
	third.clear();
	bool firstIter = true;
	while (edgeToRemove > 1)
	{
		auto originalGraph = graph;
		originalGraph.removePath(startingPoint, second[edgeToRemove]);
		if (firstIter)
		{
			originalGraph.removePath(startingPoint, first[first.size() - 2]);
			firstIter = false;
		}

		third = dijkstra(startingPoint, endingPoint, originalGraph.getGraph());

		if (third.size() != 0)
			break;
		edgeToRemove--;
	}

	if (third.size() == 0)
	{
		std::cout << "There is no third path\n";
		return;
	}
	//print();
	printPath(third);
	//std::cout << "\n\n\n";

	// Returning array of 3 vectors with the paths
	std::vector<int> paths[3];
	std::size_t size = first.size();
	for (int i = 0; i < size; i++)
	{
		paths[0].emplace_back(first[i]);
	}
	size = second.size();
	for (int i = 0; i < size; i++)
	{
		paths[2].emplace_back(second[i]);
	}
	size = third.size();
	for (int i = 0; i < size; i++)
	{
		paths[2].emplace_back(third[i]);
	}

	return paths;
}

void GraphAlgorithm::fillArrays(std::size_t startingPoint)
{
	std::fill(visited, visited + MAXSIZE, false);
	std::fill(distance, distance + MAXSIZE, INT_MAX);
	distance[startingPoint] = 0;
}