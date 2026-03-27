#include "console_interface.h"

#include <iostream>
#include <limits>
#include <stdexcept>

namespace s21 {
  namespace {
    const int kInf = std::numeric_limits<int>::max() / 2;
  }

  void ConsoleInterface::Run() {
    int choice = -1;

    while (choice != 0) {
      PrintMenu();

      if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Input error. Please enter a number.\n";
        continue;
      }

      try {
        switch (choice) {
          case 1:
            HandleLoadGraph();
            break;
          case 2:
            HandleDepthFirstSearch();
            break;
          case 3:
            HandleBreadthFirstSearch();
            break;
          case 4:
            HandleShortestPathBetweenVertices();
            break;
          case 5:
            HandleShortestPathsBetweenAllVertices();
            break;
          case 6:
            HandleLeastSpanningTree();
            break;
          case 7:
            HandleTravelingSalesmanProblem();
            break;
          case 0:
            std::cout << "Program завершена.\n";
            break;
          default:
            std::cout << "Incorrect menu item.\n";
            break;
        }
      } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
      }
    }
  }

  void ConsoleInterface::PrintMenu() const {
    std::cout << "\n========== SimpleNavigator ==========\n";
    std::cout << "1. Load graph from file\n";
    std::cout << "2. Depth First Search\n";
    std::cout << "3. Breadth First Search\n";
    std::cout << "4. Shortest path between two vertices\n";
    std::cout << "5. Shortest paths between all vertices\n";
    std::cout << "6. Minimum spanning tree\n";
    std::cout << "7. Solve Traveling Salesman Problem\n";
    std::cout << "0. Exit\n";
    std::cout << "Select menu item: ";
  }

  void ConsoleInterface::HandleLoadGraph() {
    std::string filename;
    std::cout << "Enter graph file path: ";
    std::cin >> filename;

    graph_.LoadGraphFromFile(filename);
    graph_loaded_ = true;

    std::cout << "Graph loaded successfully.\n";
    std::cout << "Number of vertices: " << graph_.GetSize() << '\n';
  }

  void ConsoleInterface::HandleDepthFirstSearch() const {
    if (!IsGraphLoaded()) {
      throw std::logic_error("Graph is not loaded");
    }

    int start_vertex = 0;
    std::cout << "Enter start vertex (1.." << graph_.GetSize() << "): ";
    std::cin >> start_vertex;

    std::vector<int> result =
        GraphAlgorithms::DepthFirstSearch(graph_, start_vertex);

    std::cout << "DFS result: ";
    PrintVector(result);
  }

  void ConsoleInterface::HandleBreadthFirstSearch() const {
    if (!IsGraphLoaded()) {
      throw std::logic_error("Graph is not loaded");
    }

    int start_vertex = 0;
    std::cout << "Enter start vertex (1.." << graph_.GetSize() << "): ";
    std::cin >> start_vertex;

    std::vector<int> result =
        GraphAlgorithms::BreadthFirstSearch(graph_, start_vertex);

    std::cout << "BFS result: ";
    PrintVector(result);
  }

  void ConsoleInterface::HandleShortestPathBetweenVertices() const {
    if (!IsGraphLoaded()) {
      throw std::logic_error("Graph is not loaded");
    }

    int vertex1 = 0;
    int vertex2 = 0;

    std::cout << "Enter first vertex: ";
    std::cin >> vertex1;
    std::cout << "Enter second vertex: ";
    std::cin >> vertex2;

    int distance = GraphAlgorithms::GetShortestPathBetweenVertices(
        graph_, vertex1, vertex2);

    if (distance >= kInf) {
      std::cout << "Path between vertices does not exist.\n";
    } else {
      std::cout << "Shortest path length: " << distance << '\n';
    }
  }

  void ConsoleInterface::HandleShortestPathsBetweenAllVertices() const {
    if (!IsGraphLoaded()) {
      throw std::logic_error("Graph is not loaded");
    }

    std::vector<std::vector<int>> result =
        GraphAlgorithms::GetShortestPathsBetweenAllVertices(graph_);

    std::cout << "Matrix of shortest paths:\n";
    PrintMatrix(result);
  }

  void ConsoleInterface::HandleLeastSpanningTree() const {
    if (!IsGraphLoaded()) {
      throw std::logic_error("Graph is not loaded");
    }

    std::vector<std::vector<int>> result =
        GraphAlgorithms::GetLeastSpanningTree(graph_);

    std::cout << "Minimum spanning tree adjacency matrix:\n";
    PrintMatrix(result);
  }

  void ConsoleInterface::HandleTravelingSalesmanProblem() const {
    if (!IsGraphLoaded()) {
      throw std::logic_error("Graph is not loaded");
    }

    TsmResult result = GraphAlgorithms::SolveTravelingSalesmanProblem(graph_);

    std::cout << "Route: ";
    PrintVector(result.vertices);
    std::cout << "Route length: " << result.distance << '\n';
  }

  void ConsoleInterface::PrintVector(const std::vector<int>& data) const {
    for (size_t i = 0; i < data.size(); ++i) {
      std::cout << data[i];
      if (i + 1 < data.size()) {
        std::cout << " -> ";
      }
    }
    std::cout << '\n';
  }

  void ConsoleInterface::PrintMatrix(
      const std::vector<std::vector<int>>& matrix) const {
    for (const auto& row : matrix) {
      for (int value : row) {
        if (value >= kInf) {
          std::cout << "INF\t";
        } else {
          std::cout << value << '\t';
        }
      }
      std::cout << '\n';
    }
  }

  bool ConsoleInterface::IsGraphLoaded() const {
    return graph_loaded_;
  }
}