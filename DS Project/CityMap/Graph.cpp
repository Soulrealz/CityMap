#include "Graph.h"

void Graph::createMatrix(const unsigned int size)
{
	adjMatrix.resize(size);
	// Filling with 0s;
	for (std::size_t i = 0; i < size; i++)
	{
		for (std::size_t j = 0; j < size; j++)
		{
			adjMatrix[i].emplace_back(false);
		}
	}
	// First col/row arent used
	for (std::size_t i = 1; i < size; i++)
	{
		std::size_t size = graph[i].size();
		for (std::size_t j = 0; j < size; j++)
		{
			adjMatrix[i][graph[i][j].to] = true;
		}
	}
}
void Graph::printMatrix() const
{
	std::cout << "  ";
	for (std::size_t i = 1; i < adjMatrix.size(); i++)
		std::cout << i << " ";
	std::cout << "\n";

	for (std::size_t i = 1; i < adjMatrix.size(); i++)
	{
		std::cout << i << " ";
		for (std::size_t j = 1; j < adjMatrix[i].size(); j++)
			std::cout << adjMatrix[i][j] << " ";
		std::cout << "\n";
	}
}

bool Graph::isNotDeadEnd(const std::size_t row, const std::size_t size) const
{
	for (std::size_t i = 0; i < size; i++)
		if (adjMatrix[row][i] == true)
			return true;
	return false;
}
std::vector<int> Graph::getIncomingStreets(const std::size_t col, const std::size_t size) const
{
	std::vector<int> incomingStreets;
	for (std::size_t i = 0; i < size; i++)
		if (adjMatrix[i][col])
			incomingStreets.emplace_back(i);
	return incomingStreets;
}
std::vector<int> Graph::getOutgoingStreets(const std::size_t row, const std::size_t size) const
{
	std::vector<int> outGoingStreets;
	for (std::size_t i = 0; i < size; i++)
		if (adjMatrix[row][i])
			outGoingStreets.emplace_back(i);
	return outGoingStreets;
}

int Graph::getLength(const std::size_t from, const std::size_t to) const
{
	for (std::size_t i = 0; i < graph[from].size(); i++)
		if (graph[from][i].to == to)
			return graph[from][i].length;
	return -1;
}

int Graph::adjustPath(const std::size_t from, const std::size_t to, int value)
{
	// As it is not mxn matrix
	// we need to get size of vector in "from" position in array
	int original = -1;
	for (std::size_t i = 0; i < graph[from].size(); i++)
	{
		// From pos in Array
		// i pos in vector
		// so if we have 1 -> 2, 1 -> 3 graph and want to remove 2 and 3
		// graph[1][1].2 == 2
		// graph[1][2].3 == 3
		if (graph[from][i].to == to)
		{
			original = graph[from][i].length;
			graph[from][i].length = value;
			break;
		}
	}
	return original;
}
