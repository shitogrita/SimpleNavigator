#include "console_interface.h"

#include <filesystem>
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
        std::cerr << "\nInput error: please enter a number from the menu.\n";
        continue;
      }

      try {
        std::cout << '\n';

        switch (choice) {
          case 1:
            std::cout << ">>> Loading graph\n";
            HandleLoadGraph();
            break;
          case 2:
            std::cout << ">>> Running depth-first search\n";
            HandleDepthFirstSearch();
            break;
          case 3:
            std::cout << ">>> Running breadth-first search\n";
            HandleBreadthFirstSearch();
            break;
          case 4:
            std::cout << ">>> Searching shortest path between two vertices\n";
            HandleShortestPathBetweenVertices();
            break;
          case 5:
            std::cout << ">>> Computing shortest paths between all vertices\n";
            HandleShortestPathsBetweenAllVertices();
            break;
          case 6:
            std::cout << ">>> Building minimum spanning tree\n";
            HandleLeastSpanningTree();
            break;
          case 7:
            std::cout << ">>> Solving traveling salesman problem\n";
            HandleTravelingSalesmanProblem();
            break;
          case 8:
            std::cout << ">>> Comparing TSP algorithms\n";
            HandleCompareTravelingSalesmanAlgorithms();
            break;
          case 0:
            std::cout << "Program finished.\n";
            break;
          default:
            std::cout << "Incorrect menu item. Please choose a valid option.\n";
            break;
        }
      } catch (const std::exception& e) {
        std::cerr << "Operation failed: " << e.what() << '\n';
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }

  void ConsoleInterface::PrintMenu() const {
    std::cout << "\n========================================\n";
    std::cout << "         SIMPLE NAVIGATOR MENU\n";
    std::cout << "========================================\n";

    std::cout << "Graph status: ";
    if (graph_loaded_) {
      std::cout << "LOADED (" << graph_.GetSize() << " vertices)\n";
    } else {
      std::cout << "NOT LOADED\n";
    }

    std::cout << "\n[ Graph ]\n";
    std::cout << "  1. Load graph from file\n";

    std::cout << "\n[ Traversal ]\n";
    std::cout << "  2. Depth-first search (DFS)\n";
    std::cout << "  3. Breadth-first search (BFS)\n";

    std::cout << "\n[ Shortest paths ]\n";
    std::cout << "  4. Shortest path between two vertices\n";
    std::cout << "  5. All-pairs shortest paths\n";

    std::cout << "\n[ Spanning tree ]\n";
    std::cout << "  6. Minimum spanning tree\n";

    std::cout << "\n[ Traveling Salesman Problem ]\n";
    std::cout << "  7. Solve TSP (requires complete graph)\n";
    std::cout << "  8. Compare TSP algorithms (requires complete graph)\n";

    std::cout << "\n[ Exit ]\n";
    std::cout << "  0. Exit\n";

    std::cout << "\nSelect menu item: ";
  }

  void ConsoleInterface::HandleLoadGraph() {
    std::string filename;
    std::cout << "Enter graph file path: ";
    std::cin >> filename;

    const std::vector<std::string> candidates = {
      filename,
      "./" + filename,
      "../" + filename,
      "data/" + filename,
      "../data/" + filename,
      "../../data/" + filename
  };

    bool loaded = false;
    std::string used_path;

    for (const auto& path : candidates) {
      if (std::filesystem::exists(path)) {
        graph_.LoadGraphFromFile(path);
        loaded = true;
        used_path = path;
        break;
      }
    }

    if (!loaded) {
      std::cout << "Current working directory: "
                << std::filesystem::current_path() << '\n';
      throw std::invalid_argument(
          "Could not open graph file. Check file name and path.");
    }

    graph_loaded_ = true;

    std::cout << "Graph loaded successfully.\n";
    std::cout << "File: " << used_path << '\n';
    std::cout << "Vertices: " << graph_.GetSize() << '\n';
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

    int distance =
        GraphAlgorithms::GetShortestPathBetweenVertices(graph_, vertex1, vertex2);

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

    try {
      TsmResult result = GraphAlgorithms::SolveTravelingSalesmanProblem(graph_);

      std::cout << "TSP route: ";
      PrintVector(result.vertices);
      std::cout << "Route length: " << result.distance << '\n';
    } catch (const std::exception&) {
      throw std::runtime_error(
          "TSP cannot be solved for the current graph. "
          "A complete graph is required.");
    }
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
    std::cout << '\n';
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
  void ConsoleInterface::HandleCompareTravelingSalesmanAlgorithms() const {
    if (!IsGraphLoaded()) {
      throw std::logic_error("Graph is not loaded");
    }

    int iterations = 0;
    std::cout << "Enter number of runs: ";
    std::cin >> iterations;

    if (!std::cin || iterations <= 0) {
      throw std::invalid_argument("Number of runs must be positive");
    }

    try {
      std::vector<TspBenchmarkResult> results =
          TspComparison::CompareAll(graph_, iterations);

      std::cout << "\n========================================\n";
      std::cout << "       TSP ALGORITHMS COMPARISON\n";
      std::cout << "========================================\n";

      for (const auto& result : results) {
        std::cout << result.algorithm_name << '\n';
        std::cout << "  total time:   " << result.total_time_ms << " ms\n";
        std::cout << "  average time: " << result.average_time_ms << " ms\n";
        std::cout << "  route length: " << result.last_result.distance << '\n';
        std::cout << "  route: ";
        PrintVector(result.last_result.vertices);
        std::cout << '\n';
      }
    } catch (const std::exception&) {
      throw std::runtime_error(
          "TSP comparison is unavailable for this graph.\n"
          "Reason: TSP algorithms in this project require a complete graph.");
    }
  }
}