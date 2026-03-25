#pragma once

#include <string>
#include <iostream>

namespace s21 {
	class Graph {
	  public:
		Graph() = default;
		~Graph() = default;

		void LoadGraphFromFile(const std::string& filename);
		void ExportGraphToDot(const std::string& filename) const;


	  private:




	};
};