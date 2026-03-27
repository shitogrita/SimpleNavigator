#pragma once

#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <vector>

#include "../algorithms/s21_graph_algorithms.h"
#include "../graph/s21_graph.h"

class GraphAlgorithmsTest : public ::testing::Test {
 protected:
  void SetUp() override {
    {
      std::ofstream out("graph_algo_test_1.txt");
      out << "4\n"
          << "0 1 0 1\n"
          << "1 0 1 0\n"
          << "0 1 0 0\n"
          << "1 0 0 0\n";
    }

    {
      std::ofstream out("graph_algo_weighted.txt");
      out << "4\n"
          << "0 1 4 0\n"
          << "1 0 2 7\n"
          << "4 2 0 1\n"
          << "0 7 1 0\n";
    }

    {
      std::ofstream out("graph_algo_complete.txt");
      out << "4\n"
          << "0 2 9 10\n"
          << "2 0 6 4\n"
          << "9 6 0 8\n"
          << "10 4 8 0\n";
    }
  }

  void TearDown() override {
    std::remove("graph_algo_test_1.txt");
    std::remove("graph_algo_weighted.txt");
    std::remove("graph_algo_complete.txt");
  }
};

TEST_F(GraphAlgorithmsTest, DepthFirstSearchWorksCorrectly) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_algo_test_1.txt");

  std::vector<int> result =
      s21::GraphAlgorithms::DepthFirstSearch(graph, 1);
  std::vector<int> expected = {1, 2, 3, 4};

  EXPECT_EQ(result, expected);
}

TEST_F(GraphAlgorithmsTest, BreadthFirstSearchWorksCorrectly) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_algo_test_1.txt");

  std::vector<int> result =
      s21::GraphAlgorithms::BreadthFirstSearch(graph, 1);
  std::vector<int> expected = {1, 2, 4, 3};

  EXPECT_EQ(result, expected);
}

TEST_F(GraphAlgorithmsTest, DepthFirstSearchThrowsForInvalidStartVertex) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_algo_test_1.txt");

  EXPECT_THROW(s21::GraphAlgorithms::DepthFirstSearch(graph, 0),
               std::out_of_range);
  EXPECT_THROW(s21::GraphAlgorithms::DepthFirstSearch(graph, 5),
               std::out_of_range);
}

TEST_F(GraphAlgorithmsTest, BreadthFirstSearchThrowsForInvalidStartVertex) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_algo_test_1.txt");

  EXPECT_THROW(s21::GraphAlgorithms::BreadthFirstSearch(graph, 0),
               std::out_of_range);
  EXPECT_THROW(s21::GraphAlgorithms::BreadthFirstSearch(graph, 5),
               std::out_of_range);
}

TEST_F(GraphAlgorithmsTest, GetShortestPathBetweenVerticesWorksCorrectly) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_algo_weighted.txt");

  int result =
      s21::GraphAlgorithms::GetShortestPathBetweenVertices(graph, 1, 4);

  EXPECT_EQ(result, 4);
}

TEST_F(GraphAlgorithmsTest,
       GetShortestPathBetweenVerticesThrowsForInvalidVertex) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_algo_weighted.txt");

  EXPECT_THROW(
      s21::GraphAlgorithms::GetShortestPathBetweenVertices(graph, 0, 4),
      std::out_of_range);
  EXPECT_THROW(
      s21::GraphAlgorithms::GetShortestPathBetweenVertices(graph, 1, 5),
      std::out_of_range);
}

TEST_F(GraphAlgorithmsTest, GetShortestPathsBetweenAllVerticesWorksCorrectly) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_algo_weighted.txt");

  auto result =
      s21::GraphAlgorithms::GetShortestPathsBetweenAllVertices(graph);

  ASSERT_EQ(result.size(), 4U);
  ASSERT_EQ(result[0].size(), 4U);

  EXPECT_EQ(result[0][0], 0);
  EXPECT_EQ(result[0][1], 1);
  EXPECT_EQ(result[0][2], 3);
  EXPECT_EQ(result[0][3], 4);
  EXPECT_EQ(result[1][3], 3);
  EXPECT_EQ(result[3][0], 4);
}

TEST_F(GraphAlgorithmsTest, GetLeastSpanningTreeWorksCorrectly) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_algo_weighted.txt");

  auto mst = s21::GraphAlgorithms::GetLeastSpanningTree(graph);

  ASSERT_EQ(mst.size(), 4U);
  ASSERT_EQ(mst[0].size(), 4U);

  int total_weight = 0;
  for (size_t i = 0; i < mst.size(); ++i) {
    for (size_t j = i + 1; j < mst.size(); ++j) {
      total_weight += mst[i][j];
    }
  }

  EXPECT_EQ(total_weight, 4);
}

TEST_F(GraphAlgorithmsTest, SolveTravelingSalesmanProblemWorksCorrectly) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_algo_complete.txt");

  s21::TsmResult result =
      s21::GraphAlgorithms::SolveTravelingSalesmanProblem(graph);

  EXPECT_FALSE(result.vertices.empty());
  EXPECT_GT(result.distance, 0.0);
  EXPECT_EQ(result.vertices.front(), result.vertices.back());
}

TEST_F(GraphAlgorithmsTest,
       SolveTravelingSalesmanProblemThrowsForIncompleteGraph) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_algo_test_1.txt");

  EXPECT_THROW(s21::GraphAlgorithms::SolveTravelingSalesmanProblem(graph),
               std::runtime_error);
}