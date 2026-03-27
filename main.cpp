#include <iostream>
#include <string>
#include "graph.h"
#include "graph_algorithms/graph_algorithms.h"

void PrintMenu() {
    std::cout << "\n===== SimpleNavigator =====\n";
    std::cout << "1. Load graph from file\n";
    std::cout << "2. Depth First Search\n";
    std::cout << "3. Breadth First Search\n";
    std::cout << "4. Shortest path between two vertices\n";
    std::cout << "5. Shortest paths between all vertices\n";
    std::cout << "6. Minimum spanning tree\n";
    std::cout << "7. Solve TSP (Ant Colony)\n";
    std::cout << "8. Compare TSP algorithms (Ant, BruteForce, HeldKarp)\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

void PrintVector(const std::vector<int>& vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i + 1 < vec.size()) std::cout << " -> ";
    }
    std::cout << std::endl;
}

void PrintMatrix(const std::vector<std::vector<int>>& mat) {
    for (const auto& row : mat) {
        for (int val : row) {
            if (val >= INF/2) std::cout << "INF\t";
            else std::cout << val << "\t";
        }
        std::cout << std::endl;
    }
}

int main() {
    Graph graph;
    bool graph_loaded = false;

    int choice;
    do {
        PrintMenu();
        std::cin >> choice;
        try {
            switch (choice) {
                case 1: {
                    std::string filename;
                    std::cout << "Enter filename: ";
                    std::cin >> filename;
                    graph.LoadGraphFromFile(filename);
                    graph_loaded = true;
                    std::cout << "Graph loaded successfully.\n";
                    break;
                }
                case 2: {
                    if (!graph_loaded) throw std::runtime_error("No graph loaded");
                    int start;
                    std::cout << "Enter start vertex (1.." << graph.GetVertexCount() << "): ";
                    std::cin >> start;
                    auto result = GraphAlgorithms::DepthFirstSearch(graph, start);
                    std::cout << "DFS order: ";
                    PrintVector(result);
                    break;
                }
                case 3: {
                    if (!graph_loaded) throw std::runtime_error("No graph loaded");
                    int start;
                    std::cout << "Enter start vertex (1.." << graph.GetVertexCount() << "): ";
                    std::cin >> start;
                    auto result = GraphAlgorithms::BreadthFirstSearch(graph, start);
                    std::cout << "BFS order: ";
                    PrintVector(result);
                    break;
                }
                case 4: {
                    if (!graph_loaded) throw std::runtime_error("No graph loaded");
                    int v1, v2;
                    std::cout << "Enter two vertices: ";
                    std::cin >> v1 >> v2;
                    int dist = GraphAlgorithms::GetShortestPathBetweenVertices(graph, v1, v2);
                    std::cout << "Shortest distance: " << dist << std::endl;
                    break;
                }
                case 5: {
                    if (!graph_loaded) throw std::runtime_error("No graph loaded");
                    auto mat = GraphAlgorithms::GetShortestPathsBetweenAllVertices(graph);
                    std::cout << "Shortest paths matrix:\n";
                    PrintMatrix(mat);
                    break;
                }
                case 6: {
                    if (!graph_loaded) throw std::runtime_error("No graph loaded");
                    auto mst = GraphAlgorithms::GetLeastSpanningTree(graph);
                    std::cout << "Minimum spanning tree adjacency matrix:\n";
                    PrintMatrix(mst);
                    break;
                }
                case 7: {
                    if (!graph_loaded) throw std::runtime_error("No graph loaded");
                    auto result = GraphAlgorithms::SolveTravelingSalesmanProblem(graph);
                    std::cout << "TSP route: ";
                    PrintVector(result.vertices);
                    std::cout << "Total distance: " << result.distance << std::endl;
                    break;
                }
                case 8: {
                    if (!graph_loaded) throw std::runtime_error("No graph loaded");
                    int N;
                    std::cout << "Enter number of runs for each algorithm: ";
                    std::cin >> N;

                    auto start = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < N; ++i) GraphAlgorithms::SolveTravelingSalesmanProblem(graph);
                    auto end = std::chrono::high_resolution_clock::now();
                    auto ant_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

                    start = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < N; ++i) GraphAlgorithms::SolveTSP_BruteForce(graph);
                    end = std::chrono::high_resolution_clock::now();
                    auto brute_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

                    start = std::chrono::high_resolution_clock::now();
                    for (int i = 0; i < N; ++i) GraphAlgorithms::SolveTSP_HeldKarp(graph);
                    end = std::chrono::high_resolution_clock::now();
                    auto held_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

                    std::cout << "Ant Colony: " << ant_time << " ms\n";
                    std::cout << "Brute Force: " << brute_time << " ms\n";
                    std::cout << "Held-Karp: " << held_time << " ms\n";
                    break;
                }
                case 0:
                    std::cout << "Exiting...\n";
                    break;
                default:
                    std::cout << "Invalid choice\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    } while (choice != 0);

    return 0;
}