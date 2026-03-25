#pragma once

#include "../graph/s21_graph.h"

namespace s21 {
	class GraphAlgorithms {
	  public:
		int GetShortestPathBetweenVertices(Graph &graph, int vertex1, int vertex2);
		Graph GetShortestPathsBetweenAllVertices(Graph &graph);
	  private:




	};
}