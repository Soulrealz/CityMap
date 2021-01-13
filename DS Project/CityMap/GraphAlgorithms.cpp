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

std::vector<std::vector<int>> GraphAlgorithm::YenAlgorithmForThreePaths(std::size_t startingPoint, std::size_t endingPoint)
{
	//Space Complexity - KN where K is amount of desired paths - 1
	//Time Complexity - O(KN ^ 3) 

	std::vector<std::vector<int>> paths;
	//Reserving space for 3 paths
	paths.resize(3);

	Paths threePaths;
	threePaths.setStartEnd(startingPoint, endingPoint);
	// first.size() - 1 would give starting node so we do - 2
	// required to see if there are any nodes that can be removed or if there is a path at all
	int edgeToRemove = threePaths.firstPath(paths, graph) - 2;
	
	if (edgeToRemove >= 0)
	{
		edgeToRemove = threePaths.secondPath(paths, graph, edgeToRemove) - 2;

		if (edgeToRemove > 0)
			threePaths.thirdPath(paths, graph, edgeToRemove);
	}
	


	// First path (if found) is already filled in array
	// Now fill second and third path (if any)
	auto path = threePaths.getSecond();
	std::size_t size = path.size();
	for (int i = size - 1; i >= 0; i--)
	{
		paths[1].emplace_back(path[i]);
	}

	path = threePaths.getThird();
	size = path.size();
	for (int i = size - 1; i >= 0; i--)
	{
		paths[2].emplace_back(path[i]);
	}

	// Returning array of 3 vectors with the paths
	return paths;
}


void GraphAlgorithm::fillArrays(std::size_t startingPoint)
{
	std::fill(visited, visited + MAXSIZE, false);
	std::fill(distance, distance + MAXSIZE, INT_MAX);
	distance[startingPoint] = 0;
}

bool GraphAlgorithm::isCyclic(int node)
{
	bool* recStack = new bool[size];

	std::fill(visited, visited + size, false);
	std::fill(recStack, recStack + size, false);

	return isCyclicUtil(node, recStack);
}
bool GraphAlgorithm::isCyclicUtil(int v, bool * recStack)
{
	if (visited[v] == false)
	{
		// Mark the current node as visited and part of recursion stack 
		visited[v] = true;
		recStack[v] = true;
		auto grph = graph.getGraph();
		// Recur for all the vertices adjacent to this vertex 
		for (int i = 0; i < grph[v].size(); i++)
		{
			if (!visited[grph[v][i].to] && isCyclicUtil(grph[v][i].to, recStack))
				return true;
			else if (recStack[grph[v][i].to])
				return true;
		}

	}
	recStack[v] = false;
	return false;
}

int Paths::firstPath(std::vector<std::vector<int>>& paths, GraphAlgorithm graph)
{
	std::size_t size;

	// normal dijkstra to find shortest path
	first = graph.dijkstra(startingPoint, endingPoint, graph.getGraphVector());
	if (first.size() != 0)
	{
		size = first.size();
		for (int i = size - 1; i >= 0; i--)
		{
			paths[0].emplace_back(first[i]);
		}
	}

	// print();
	// printPath(first);
	// std::cout << "\n\n\n";

	return first.size();
}
int Paths::secondPath(std::vector<std::vector<int>>& paths, GraphAlgorithm graph, int edgeToRemove)
{
	// to remove the second node in the path (as we cant remove the starting point)
	// if 1-2-3-4 remove 2 from path
	while (edgeToRemove >= 1)
	{
		// We want to edit the graph so we make a copy to avoid changing the original
		auto originalGraph = graph.getGraph();

		// remove a different node from first shortest path until a second shortest one is found (if there is any)
		originalGraph.removePath(startingPoint, first[edgeToRemove]);
		second = graph.dijkstra(startingPoint, endingPoint, originalGraph.getGraph());


		// If a path has been found already
		if (second.size() != 0)
			break;
		edgeToRemove--;
	}

	//print();
	//printPath(second);
	//std::cout << "\n\n\n";

	return second.size();
}
void Paths::thirdPath(std::vector<std::vector<int>>& paths, GraphAlgorithm graph, int edgeToRemove)
{
	bool firstIter = true;
	while (edgeToRemove >= 1)
	{
		auto originalGraph = graph.getGraph();
		originalGraph.removePath(startingPoint, second[edgeToRemove]);
		if (firstIter)
		{
			originalGraph.removePath(startingPoint, first[first.size() - 2]);
			firstIter = false;
		}

		third = graph.dijkstra(startingPoint, endingPoint, originalGraph.getGraph());

		if (third.size() != 0)
			break;
		edgeToRemove--;
	}

	//print();
	//printPath(third);
	//std::cout << "\n\n\n";
}
