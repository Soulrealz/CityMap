#pragma once
#include <vector>
#include <iostream>


// Struct to construct edges/connections between 2 nodes(intersections)
struct Path
{
	unsigned int to, length;
	Path(unsigned int _to, unsigned int _length) : to(_to), length(_length) {}

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
	void addPath(unsigned int from, unsigned int to, unsigned int length) { graph[from].emplace_back(Path(to, length)); }	

	// Utility function to "remove" paths
	// required in order to find more than one shortest path
	void removePath(unsigned int from, unsigned int to);
	std::vector<Path>* getGraph() const { return graph; }
	std::size_t getSize() const { return size; }

	// Printing entire graph with every connection : <node> <node> <length>
	// 1 2 100
	// 1 3 150
	// etc
	void print() const
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < graph[i].size(); j++)
				std::cout << i << " " << graph[i][j] << "\n";
	}
private:
	static std::vector<Path> graph[MAXSIZE];
	std::size_t size;
};