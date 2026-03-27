#pragma once

#include <string>
#include <vector>

#include "../algorithms/s21_graph_algorithms.h"
#include "../graph/s21_graph.h"

namespace s21 {

	class ConsoleInterface {
	public:
		ConsoleInterface() = default;
		~ConsoleInterface() = default;

		void Run();

	private:
		void PrintMenu() const;
		void HandleLoadGraph();
		void HandleDepthFirstSearch() const;
		void HandleBreadthFirstSearch() const;
		void HandleShortestPathBetweenVertices() const;
		void HandleShortestPathsBetweenAllVertices() const;
		void HandleLeastSpanningTree() const;
		void HandleTravelingSalesmanProblem() const;

		void PrintVector(const std::vector<int>& data) const;
		void PrintMatrix(const std::vector<std::vector<int>>& matrix) const;

		bool IsGraphLoaded() const;

		Graph graph_;
		bool graph_loaded_ = false;
	};
}