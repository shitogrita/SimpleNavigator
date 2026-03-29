#include "s21_tsp_compare.h"

#include <chrono>
#include <stdexcept>

namespace s21 {
  namespace {

    using Clock = std::chrono::high_resolution_clock;
    using Ms = std::chrono::duration<double, std::milli>;

    template <typename Solver>
    TspBenchmarkResult MeasureAlgorithm(const Graph& graph, int iterations,
                                        const std::string& name, Solver solver) {
      if (iterations <= 0) {
        throw std::invalid_argument("Iterations count must be positive");
      }

      auto start = Clock::now();

      TsmResult last_result;
      for (int i = 0; i < iterations; ++i) {
        last_result = solver(graph);
      }

      auto finish = Clock::now();
      double total_time_ms = std::chrono::duration_cast<Ms>(finish - start).count();

      TspBenchmarkResult result;
      result.algorithm_name = name;
      result.total_time_ms = total_time_ms;
      result.average_time_ms = total_time_ms / iterations;
      result.last_result = last_result;

      return result;
  }
}

  std::vector<TspBenchmarkResult> TspComparison::CompareAll(const Graph& graph,
                                                            int iterations) {
    std::vector<TspBenchmarkResult> results;

    results.push_back(MeasureAntColony(graph, iterations));
    results.push_back(MeasureNearestNeighbor(graph, iterations));
    results.push_back(MeasureBruteForce(graph, iterations));

    return results;
  }

  TspBenchmarkResult TspComparison::MeasureAntColony(const Graph& graph,
                                                     int iterations) {
    return MeasureAlgorithm(
        graph, iterations, "Ant colony algorithm",
        [](const Graph& g) {
          return GraphAlgorithms::SolveTravelingSalesmanProblem(g);
        });
  }

  TspBenchmarkResult TspComparison::MeasureNearestNeighbor(const Graph& graph,
                                                           int iterations) {
    return MeasureAlgorithm(
        graph, iterations, "Nearest neighbor",
        [](const Graph& g) {
          return GraphAlgorithms::SolveTravelingSalesmanProblemNearestNeighbor(g);
        });
  }

  TspBenchmarkResult TspComparison::MeasureBruteForce(const Graph& graph,
                                                      int iterations) {
    return MeasureAlgorithm(
        graph, iterations, "Brute force",
        [](const Graph& g) {
          return GraphAlgorithms::SolveTravelingSalesmanProblemBruteForce(g);
        });
  }
}