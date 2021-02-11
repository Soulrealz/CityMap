#include <queue>
#include <limits.h>

#include "GraphAlgorithms.h"
#define pair std::pair<int, int>

void GraphAlgorithm::dijkstra(std::size_t startingPoint)
{
	// getting array of vectors
	std::vector<Path>* graph = this->graph.getAdjList();

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
	// Space Complexity - KN where K is amount of desired paths - 1
	// Time Complexity - O(KN ^ 3) 		

	Paths threePaths;
	threePaths.setStartEnd(startingPoint, endingPoint);
	// first.size() - 1 would give starting node so we do - 2
	// required to see if there are any nodes that can be removed or if there is a path at all
	int edgeToRemove = threePaths.firstPath(graph) - 2;
	
	if (edgeToRemove >= 0)
	{
		edgeToRemove = threePaths.extractShortestPathsFromFirst(graph, edgeToRemove) - 2;

		if (edgeToRemove >= 0)
			threePaths.thirdPath(graph, edgeToRemove);
	}
	
	std::vector<std::vector<int>> paths;
	// Reserving space for 3 paths
	paths.resize(3);
	fillPathsVector(paths, threePaths);

	// Returning array of 3 vectors with the paths
	return paths;
}


void GraphAlgorithm::fillArrays(std::size_t startingPoint)
{
	std::fill(visited, visited + MAXSIZE, false);
	std::fill(distance, distance + MAXSIZE, INT_MAX);
	distance[startingPoint] = 0;
}
void GraphAlgorithm::fillPathsVector(std::vector<std::vector<int>>& paths, Paths& threePaths) const
{
	// first
	auto path = threePaths.getFirst();
	std::size_t size = path.size();
	for (int i = size - 1; i >= 0; i--)
	{
		paths[0].emplace_back(path[i]);
	}

	// second
	path = threePaths.getSecond();
	size = path.size();
	for (int i = size - 1; i >= 0; i--)
	{
		paths[1].emplace_back(path[i]);
	}
	// third
	path = threePaths.getThird();
	size = path.size();
	for (int i = size - 1; i >= 0; i--)
	{
		paths[2].emplace_back(path[i]);
	}


}

bool GraphAlgorithm::isCyclic(int node)
{
	bool* recStack = new bool[size];

	std::fill(visited, visited + size, false);
	std::fill(recStack, recStack + size, false);

	return isCyclicUtil(node, recStack);
}
bool GraphAlgorithm::isCyclicUtil(int v, bool* recStack)
{
	if (visited[v] == false)
	{
		// Mark the current node as visited and part of recursion stack 
		visited[v] = true;
		recStack[v] = true;
		auto grph = graph.getAdjList();
		// Recur for all the vertices adjacent to this vertex 
		for (std::size_t i = 0; i < grph[v].size(); i++)
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


int Paths::firstPath(GraphAlgorithm graph)
{
	// normal dijkstra to find shortest path
	first = graph.dijkstra(startingPoint, endingPoint, graph.getGraphVector());
	return first.size();
}
int Paths::extractShortestPathsFromFirst(GraphAlgorithm graph, int edgeToRemove)
{
	std::vector<std::vector<int>> allPathsDerivedFromFirst;
	// to remove the second node in the path (as we cant remove the starting point)
	// if 1-2-3-4 remove 2 from path

	// Force Algorithm to start from THIS node;
	int start = first.size() - 1;
	while (edgeToRemove >= 0)	
	{
		Graph originalGraph = graph.getGraph();
		
		// remove a different node from first shortest path until a second shortest one is found (if there is any)
		int restoreValue = originalGraph.adjustPath(first[edgeToRemove + 1], first[edgeToRemove]);
		second = graph.dijkstra(first[start], endingPoint, originalGraph.getAdjList());

		// Restoring original graph to prepare it for next cycle
		originalGraph.adjustPath(first[edgeToRemove + 1], first[edgeToRemove], restoreValue);

		// If a path has been found
		// add it to the vector of paths
		if (second.size() != 0)
			addDerivedPath(allPathsDerivedFromFirst, second, first, start);

		// Move start to next node
		start--;
		// Remove next connection
		edgeToRemove--;
	}

	// Sets second and potentially third path
	return firstDerivedPaths(allPathsDerivedFromFirst, graph.getGraph());	
}
void Paths::thirdPath(GraphAlgorithm graph, int edgeToRemove)
{
	std::vector<std::vector<int>> allPathsDerivingFromSecond;
	// Start depleting from second path
	bool firstIter = true;
	int start = second.size() - 1;
	int restoreValue;
	while (edgeToRemove >= 0)
	{
		Graph originalGraph = graph.getGraph();

		// Restore damaged first path after first iteration
		if (!firstIter)
		{
			originalGraph.adjustPath(first[first.size() - 1], first[first.size() - 2], restoreValue);
		}
		originalGraph.adjustPath(second[edgeToRemove + 1], second[edgeToRemove]);

		// Block first path once to force algorithm
		// to search for path different than first
		if (firstIter)
		{
			restoreValue = originalGraph.adjustPath(first[first.size() - 1], first[first.size() - 2]);
			firstIter = false;
		}

		third = graph.dijkstra(second[start], endingPoint, originalGraph.getAdjList());

		// If path is found add it
		if (third.size() != 0)
			addDerivedPath(allPathsDerivingFromSecond, third, second, start);

		edgeToRemove--;
		start--;
	}

	secondDerivedPaths(allPathsDerivingFromSecond, graph.getGraph());
}

void Paths::addDerivedPath(std::vector<std::vector<int>>& allPaths, std::vector<int>& derived, std::vector<int>& derivative, int start)
{
	int size = derivative.size() - 1;
	while (start != size)
		derived.emplace_back(derivative[++start]);

	std::vector<int> copy;
	copy.resize(derived.size());

	std::size_t secondSize = derived.size();
	for (std::size_t i = 0; i < secondSize; i++)
		copy[i] = derived[i];

	derived.clear();
	allPaths.emplace_back(copy);
}

int Paths::firstDerivedPaths(std::vector<std::vector<int>>& allPaths, Graph graph)
{
	// If only one path is found set to second
	if (allPaths.size() == 1)
		return onePathDerivedFromFirst(allPaths);

	// If exactly two paths have been found set them as second and third paths
	else if (allPaths.size() == 2)
		return twoPathsDerivedFromFirst(allPaths, graph);

	// Find two shortest paths of the ones that have been found
	else if (allPaths.size() > 2)
		return severalPathsDerivedFromFirst(allPaths, graph);

	// There is no path
	else return -1;
}
void Paths::secondDerivedPaths(std::vector<std::vector<int>>& allPaths, Graph graph)
{
	if (allPaths.size() == 1)
		third = allPaths[0];
	else if (allPaths.size() > 1)
		numeralPathsDerivedFromSecond(allPaths, graph);
}

int Paths::onePathDerivedFromFirst(std::vector<std::vector<int>>& allPaths)
{
	second = allPaths[0];
	return second.size();
}
int Paths::twoPathsDerivedFromFirst(std::vector<std::vector<int>>& allPaths, Graph graph)
{
	// Code to determine which path is shorter
	unsigned int firstWeight = 0;
	unsigned int secondWeight = 0;
	std::size_t firstSize = allPaths[0].size() - 1;
	std::size_t secondSize = allPaths[1].size() - 1;

	for (std::size_t i = 0; i < firstSize; i++)
		firstWeight += graph.getLength(allPaths[0][i + 1], allPaths[0][i]);
	for (std::size_t i = 0; i < secondSize; i++)
		secondWeight += graph.getLength(allPaths[1][i + 1], allPaths[1][i]);

	// Shorter path is second path
	if (firstWeight < secondWeight)
	{
		second = allPaths[0];
		third = allPaths[1];
	}
	else
	{
		second = allPaths[1];
		third = allPaths[0];
	}

	return -1;
}
int Paths::severalPathsDerivedFromFirst(std::vector<std::vector<int>>& allPaths, Graph graph)
{
	unsigned int firstMinWeight = INT_MAX;
	unsigned int secondMinWeight = INT_MAX;
	std::size_t firstIndex = INT_MAX;
	std::size_t secondIndex = INT_MAX;

	std::size_t numOfPaths = allPaths.size();
	for (std::size_t i = 0; i < numOfPaths; i++)
	{
		unsigned int tmpWeight = 0;
		std::size_t currPathSize = allPaths[i].size() - 1;
		for (std::size_t j = 0; j < currPathSize; j++)
		{
			tmpWeight += graph.getLength(allPaths[i][j + 1], allPaths[i][j]);
		}

		if (tmpWeight <= firstMinWeight)
		{
			secondMinWeight = firstMinWeight;
			firstMinWeight = tmpWeight;

			secondIndex = firstIndex;
			firstIndex = i;
		}
		else if (tmpWeight <= secondMinWeight)
		{
			secondMinWeight = tmpWeight;
			secondIndex = i;
		}
	}

	second = allPaths[firstIndex];
	third = allPaths[secondIndex];

	return -1;
}

void Paths::numeralPathsDerivedFromSecond(std::vector<std::vector<int>>& allPaths, Graph graph)
{
	unsigned int minWeight = INT_MAX;
	std::size_t pathIndex;

	std::size_t numOfPaths = allPaths.size();
	for (std::size_t i = 0; i < numOfPaths; i++)
	{
		unsigned int tmpWeight = 0;
		std::size_t currPathSize = allPaths[i].size() - 1;
		for (std::size_t j = 0; j < currPathSize; j++)
		{
			tmpWeight += graph.getLength(allPaths[i][j + 1], allPaths[i][j]);
		}

		if (tmpWeight < minWeight)
		{
			minWeight = tmpWeight;
			pathIndex = i;
		}
	}
}
