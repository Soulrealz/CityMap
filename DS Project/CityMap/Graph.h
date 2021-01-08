#pragma once
#include <vector>
#include <iostream>
#include <queue>
#include <limits.h>

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
	Graph() {}
	void addPath(unsigned int from, unsigned int to, unsigned int length) { graph[from].emplace_back(Path(to, length)); }

	void print() const
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < graph[i].size(); j++)
				std::cout << i << " " << graph[i][j] << "\n";
	}

	struct Algorithms
	{
	public:
#define pair std::pair<int, int>
		static void dijkstra(std::size_t startingPoint);
		static bool checkPath(std::size_t to) { return !(distance[to] == INT_MAX); }

		static void print(std::size_t size)
		{
			for (std::size_t i = 1; i < size; i++)
			{
				std::cout << i << " " << distance[i] << "\n";
			}
		}

	private:
		static bool visited[MAXSIZE];
		static unsigned int distance[MAXSIZE];
	};
private:
	static std::vector<Path> graph[MAXSIZE];
};