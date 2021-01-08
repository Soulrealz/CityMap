#include "Graph.h"

void Graph::Algorithms::dijkstra(std::size_t startingPoint)
{
	std::priority_queue<pair, std::vector<pair>, std::greater<pair>> toVisit;

	std::fill(visited, visited + MAXSIZE, false);
	std::fill(distance, distance + MAXSIZE, INT_MAX);
	distance[startingPoint] = 0;
	toVisit.push({ 0, startingPoint });

	while (!toVisit.empty())
	{
		int node = toVisit.top().second;
		toVisit.pop();

		if (visited[node])
			continue;

		visited[node] = true;
		for (Path neighbor : Graph::graph[node])
		{
			if (distance[node] + neighbor.length >= distance[neighbor.to])
				continue;

			distance[neighbor.to] = distance[node] + neighbor.length;
			toVisit.push({ distance[neighbor.to], neighbor.to });
		}
	}
}
