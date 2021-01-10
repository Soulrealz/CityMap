#include "Graph.h"

void Graph::removePath(unsigned int from, unsigned int to)
{
	// As it is not mxn matrix
	// we need to get size of vector in "from" position in array
	for (int i = 0; i < graph[from].size(); i++)
	{
		// From pos in Array
		// i pos in vector
		// so if we have 1 -> 2, 1 -> 3 graph and want to remove 2 and 3
		// graph[1][1].2 == 2
		// graph[1][2].3 == 3
		if (graph[from][i].to == to)
		{
			graph[from][i].length = INT_MAX;
			break;
		}
	}
}
