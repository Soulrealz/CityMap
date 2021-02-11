#pragma once
#include <vector>
#include <iostream>


// Struct to construct edges/connections between 2 nodes(intersections)
struct Path
{
	unsigned int to, length;
	Path(const unsigned int _to, const unsigned int _length) : to(_to), length(_length) {}

	friend std::ostream& operator<<(std::ostream& out, const Path& element)
	{
		out << element.to << " " << element.length;
		return out;
	}
};

const int MAXSIZE = 1000;
class Graph
{
public:
	Graph() = default;

	// Constructing Path with to,length and assigning it to vector on pos "from" in array
	void addPath(const unsigned int from, const unsigned int to, const unsigned int length) { graph[from].emplace_back(Path(to, length)); }	

	// Utility function to "remove" paths
	// required in order to find more than one shortest path
	int adjustPath(const std::size_t from, const std::size_t to, int value = INT_MAX);

	std::vector<Path>* getAdjList() const { return graph; }

	void createMatrix(const unsigned int size);
	void printMatrix() const;

	bool isNotDeadEnd(const std::size_t row, const std::size_t size) const;
	// Function to get all streets that lead into given street(used for deadend and neighbours);
	std::vector<int> getIncomingStreets(const std::size_t col, const std::size_t size) const;
	std::vector<int> getOutgoingStreets(const std::size_t row, const std::size_t size) const;

	void close(const std::size_t row, const std::size_t col) { adjMatrix[row][col] = false; }
	void open(const std::size_t row, const std::size_t col) { adjMatrix[row][col] = true; }

	std::size_t getSize() {return adjMatrix.size();}

	int getLength(const std::size_t from, const std::size_t to) const;
private:
	static std::vector<Path> graph[MAXSIZE];
	std::vector<std::vector<bool>> adjMatrix;
};