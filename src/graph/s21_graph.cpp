#include "s21_graph.h"

namespace s21 {
  void Graph::LoadGraphFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      throw std::invalid_argument("Failed to open graph file");
    }

    int size = 0;
    file >> size;

    matrix_.assign(size, std::vector<int>(size, 0));

    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        file >> matrix_[i][j];
      }
    }
  }

  void Graph::ExportGraphToDot(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
      throw std::invalid_argument("Failed to open output file");
    }

    const int size = GetSize();
    const bool directed = IsDirected();

    if (directed) {
      file << "digraph G {\n";
    } else {
      file << "graph G {\n";
    }

    for (int i = 1; i <= size; ++i) {
      file << "  " << i << ";\n";
    }

    if (directed) {
      for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
          if (matrix_[i][j] != 0) {
            file << "  " << i + 1 << " -> " << j + 1;
            if (matrix_[i][j] > 1) {
              file << " [label=" << matrix_[i][j] << "]";
            }
            file << ";\n";
          }
        }
      }
    } else {
      for (int i = 0; i < size; ++i) {
        for (int j = i; j < size; ++j) {
          if (matrix_[i][j] != 0) {
            file << "  " << i + 1 << " -- " << j + 1;
            if (matrix_[i][j] > 1) {
              file << " [label=" << matrix_[i][j] << "]";
            }
            file << ";\n";
          }
        }
      }
    }

    file << "}\n";
  }

  int Graph::GetSize() const {
    return static_cast<int>(matrix_.size());
  }

  int Graph::GetEdge(int from, int to) const {
    int size = GetSize();
    if (from < 1 || from > size || to < 1 || to > size) {
      throw std::out_of_range("Vertex index is out of range");
    }

    return matrix_[from - 1][to - 1];
  }

  bool Graph::IsDirected() const {
    int size = GetSize();

    for (int i = 0; i < size; ++i) {
      for (int j = i + 1; j < size; ++j) {
        if (matrix_[i][j] != matrix_[j][i]) {
          return true;
        }
      }
    }
    return false;
  }

  const std::vector<std::vector<int>>& Graph::GetAdjacencyMatrix() const {
    return matrix_;
  }
}