#include <gtest/gtest.h>
#include <test/test_graph.h>
#include <test/test_graph_algorithms.h>

int main() {
	std::cout << "Running tests\n";
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}