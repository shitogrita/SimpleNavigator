#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

namespace s21 {
	class Graph {
	  public:
		Graph() = default;
		~Graph() = default;

		void LoadGraphFromFile(const std::string& filename);
		void ExportGraphToDot(const std::string& filename) const;

		int GetSize() const;
		int GetEdge(int from, int to) const;
		bool IsDirected() const;
		const std::vector<std::vector<int>>& GetAdjacencyMatrix() const;

	  private:
		std::vector<std::vector<int>> matrix_;
	};
};