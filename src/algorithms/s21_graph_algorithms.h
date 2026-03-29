#pragma once

#include "../graph/s21_graph.h"

namespace s21 {
	struct TsmResult {
		std::vector<int> vertices;
		double distance;
	};

	class GraphAlgorithms {
	public:
		static std::vector<int> DepthFirstSearch(const Graph& graph, int start_vertex);
		static std::vector<int> BreadthFirstSearch(const Graph& graph, int start_vertex);
		static int GetShortestPathBetweenVertices(const Graph& graph, int vertex1, int vertex2);
		static std::vector<std::vector<int>> GetShortestPathsBetweenAllVertices(const Graph& graph);
		static std::vector<std::vector<int>> GetLeastSpanningTree(const Graph& graph);
		static TsmResult SolveTravelingSalesmanProblem(const Graph& graph);
		// дополнительные алгоритмы для Part 6
		static TsmResult SolveTravelingSalesmanProblemNearestNeighbor(const Graph& graph);

		static TsmResult SolveTravelingSalesmanProblemBruteForce(const Graph& graph);
	};

}