#pragma once
#include <vector>

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

class Graph
{
public:
	Graph() {}
	void addPath(unsigned int from, unsigned int to, unsigned int length)
	{
		graph[from].emplace_back(Path(to, length));
	}

	void print() const
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < graph[i].size(); j++)
				std::cout << i << " " << graph[i][j] << "\n";
	}
private:
	std::vector<Path> graph[int(1e4) + 5];
};