#pragma once

#include <gtest/gtest.h>

#include <cstdio>
#include <fstream>
#include <string>

#include "../graph/s21_graph.h"

class GraphTest : public ::testing::Test {
 protected:
  void SetUp() override {
    {
      std::ofstream out("graph_test_1.txt");
      out << "4\n"
          << "0 1 0 1\n"
          << "1 0 1 0\n"
          << "0 1 0 0\n"
          << "1 0 0 0\n";
    }

    {
      std::ofstream out("graph_test_directed.txt");
      out << "3\n"
          << "0 1 0\n"
          << "0 0 1\n"
          << "1 0 0\n";
    }
  }

  void TearDown() override {
    std::remove("graph_test_1.txt");
    std::remove("graph_test_directed.txt");
    std::remove("graph_test_1.dot");
    std::remove("graph_test_directed.dot");
  }
};

TEST_F(GraphTest, LoadGraphFromFileCorrectSize) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_test_1.txt");
  EXPECT_EQ(graph.GetSize(), 4);
}

TEST_F(GraphTest, GetEdgeWorksCorrectly) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_test_1.txt");

  EXPECT_EQ(graph.GetEdge(1, 2), 1);
  EXPECT_EQ(graph.GetEdge(1, 3), 0);
  EXPECT_EQ(graph.GetEdge(2, 3), 1);
  EXPECT_EQ(graph.GetEdge(4, 1), 1);
}

TEST_F(GraphTest, GetEdgeThrowsForInvalidVertex) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_test_1.txt");

  EXPECT_THROW(graph.GetEdge(0, 1), std::out_of_range);
  EXPECT_THROW(graph.GetEdge(1, 0), std::out_of_range);
  EXPECT_THROW(graph.GetEdge(5, 1), std::out_of_range);
  EXPECT_THROW(graph.GetEdge(1, 5), std::out_of_range);
}

TEST_F(GraphTest, UndirectedGraphDetection) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_test_1.txt");
  EXPECT_FALSE(graph.IsDirected());
}

TEST_F(GraphTest, DirectedGraphDetection) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_test_directed.txt");
  EXPECT_TRUE(graph.IsDirected());
}

TEST_F(GraphTest, GetAdjacencyMatrixWorksCorrectly) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_test_1.txt");

  const auto& matrix = graph.GetAdjacencyMatrix();
  ASSERT_EQ(matrix.size(), 4U);
  ASSERT_EQ(matrix[0].size(), 4U);

  EXPECT_EQ(matrix[0][1], 1);
  EXPECT_EQ(matrix[0][3], 1);
  EXPECT_EQ(matrix[2][3], 0);
}

TEST_F(GraphTest, ExportUndirectedGraphToDot) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_test_1.txt");
  graph.ExportGraphToDot("graph_test_1.dot");

  std::ifstream in("graph_test_1.dot");
  ASSERT_TRUE(in.is_open());

  std::string content((std::istreambuf_iterator<char>(in)),
                      std::istreambuf_iterator<char>());

  EXPECT_NE(content.find("graph G"), std::string::npos);
  EXPECT_NE(content.find("1 -- 2"), std::string::npos);
  EXPECT_NE(content.find("1 -- 4"), std::string::npos);
  EXPECT_NE(content.find("2 -- 3"), std::string::npos);
}

TEST_F(GraphTest, ExportDirectedGraphToDot) {
  s21::Graph graph;
  graph.LoadGraphFromFile("graph_test_directed.txt");
  graph.ExportGraphToDot("graph_test_directed.dot");

  std::ifstream in("graph_test_directed.dot");
  ASSERT_TRUE(in.is_open());

  std::string content((std::istreambuf_iterator<char>(in)),
                      std::istreambuf_iterator<char>());

  EXPECT_NE(content.find("digraph G"), std::string::npos);
  EXPECT_NE(content.find("1 -> 2"), std::string::npos);
  EXPECT_NE(content.find("2 -> 3"), std::string::npos);
  EXPECT_NE(content.find("3 -> 1"), std::string::npos);
}

TEST_F(GraphTest, LoadMissingFileThrows) {
  s21::Graph graph;
  EXPECT_THROW(graph.LoadGraphFromFile("missing_file.txt"),
               std::invalid_argument);
}