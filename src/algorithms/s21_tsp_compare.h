#pragma once

#include "s21_graph_algorithms.h"

#include <string>
#include <vector>

namespace s21 {
	struct TspBenchmarkResult {
		std::string algorithm_name;
		double total_time_ms;
		double average_time_ms;
		TsmResult last_result;
	};

	class TspComparison {
	public:
		static std::vector<TspBenchmarkResult> CompareAll(const Graph& graph, int iterations);
	private:
		static TspBenchmarkResult MeasureAntColony(const Graph& graph, int iterations);
		static TspBenchmarkResult MeasureNearestNeighbor(const Graph& graph, int iterations);
		static TspBenchmarkResult MeasureBruteForce(const Graph& graph, int iterations);
	};
}