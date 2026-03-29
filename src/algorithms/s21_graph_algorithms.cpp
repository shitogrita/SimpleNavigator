#include "s21_graph_algorithms.h"
#include <../graph/s21_graph.h>
#include "../containers/queue.h"
#include "../containers/stack.h"
#include <algorithm>
#include <limits>
#include <queue>
#include <cmath>
#include <random>
#include <chrono>


namespace s21 {
    const int INF = std::numeric_limits<int>::max() / 2;

    std::vector<int> GraphAlgorithms::DepthFirstSearch(const Graph& graph, int start_vertex) {
        int n = graph.GetSize();
        if (start_vertex < 1 || start_vertex > n) throw std::out_of_range("Invalid start vertex");

        std::vector<bool> visited(n, false);
        std::vector<int> result;
        Stack<int> stack;
        stack.push(start_vertex - 1);

        while (!stack.empty()) {
            int v = stack.top();
            stack.pop();
            if (!visited[v]) {
                visited[v] = true;
                result.push_back(v + 1); // нумерация с 1
                const auto& matrix = graph.GetAdjacencyMatrix();
                // кладем соседей в обратном порядке для сохранения порядка обхода
                for (int u = n - 1; u >= 0; --u) {
                    if (matrix[v][u] != 0 && !visited[u]) {
                        stack.push(u);
                    }
                }
            }
        }
        return result;
    }

    std::vector<int> GraphAlgorithms::BreadthFirstSearch(const Graph& graph, int start_vertex) {
        int n = graph.GetSize();
        if (start_vertex < 1 || start_vertex > n) throw std::out_of_range("Invalid start vertex");

        std::vector<bool> visited(n, false);
        std::vector<int> result;
        Queue<int> queue;
        int start = start_vertex - 1;
        visited[start] = true;
        queue.push(start);

        while (!queue.empty()) {
            int v = queue.front();
            queue.pop();
            result.push_back(v + 1);
            const auto& matrix = graph.GetAdjacencyMatrix();
            for (int u = 0; u < n; ++u) {
                if (matrix[v][u] != 0 && !visited[u]) {
                    visited[u] = true;
                    queue.push(u);
                }
            }
        }
        return result;
    }

    int GraphAlgorithms::GetShortestPathBetweenVertices(const Graph& graph, int vertex1, int vertex2) {
        int n = graph.GetSize();
        if (vertex1 < 1 || vertex1 > n || vertex2 < 1 || vertex2 > n) throw std::out_of_range("Invalid vertex");
        --vertex1; --vertex2;

        const auto& matrix = graph.GetAdjacencyMatrix();
        std::vector<int> dist(n, INF);
        std::vector<bool> visited(n, false);
        dist[vertex1] = 0;

        for (int i = 0; i < n; ++i) {
            int u = -1;
            for (int j = 0; j < n; ++j) {
                if (!visited[j] && (u == -1 || dist[j] < dist[u])) u = j;
            }
            if (u == -1 || dist[u] == INF) break;
            visited[u] = true;
            for (int v = 0; v < n; ++v) {
                if (matrix[u][v] != 0 && !visited[v] && dist[u] + matrix[u][v] < dist[v]) {
                    dist[v] = dist[u] + matrix[u][v];
                }
            }
        }
        return dist[vertex2];
    }

    std::vector<std::vector<int>> GraphAlgorithms::GetShortestPathsBetweenAllVertices(const Graph& graph) {
        int n = graph.GetSize();
        const auto& matrix = graph.GetAdjacencyMatrix();
        std::vector<std::vector<int>> dist = matrix;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i != j && dist[i][j] == 0) dist[i][j] = INF;
            }
        }
        for (int k = 0; k < n; ++k) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (dist[i][k] < INF && dist[k][j] < INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
        return dist;
    }

    std::vector<std::vector<int>> GraphAlgorithms::GetLeastSpanningTree(const Graph& graph) {
        int n = graph.GetSize();
        const auto& matrix = graph.GetAdjacencyMatrix();
        std::vector<bool> in_mst(n, false);
        std::vector<int> min_edge(n, INF);
        std::vector<int> parent(n, -1);
        min_edge[0] = 0;

        for (int i = 0; i < n; ++i) {
            int u = -1;
            for (int j = 0; j < n; ++j) {
                if (!in_mst[j] && (u == -1 || min_edge[j] < min_edge[u])) u = j;
            }
            in_mst[u] = true;
            for (int v = 0; v < n; ++v) {
                if (matrix[u][v] != 0 && !in_mst[v] && matrix[u][v] < min_edge[v]) {
                    min_edge[v] = matrix[u][v];
                    parent[v] = u;
                }
            }
        }

        std::vector<std::vector<int>> mst(n, std::vector<int>(n, 0));
        for (int v = 1; v < n; ++v) {
            int u = parent[v];
            mst[u][v] = mst[v][u] = matrix[u][v];
        }
        return mst;
    }

    // =================== Муравьиный алгоритм ===================
    TsmResult GraphAlgorithms::SolveTravelingSalesmanProblem(const Graph& graph) {
        int n = graph.GetSize();
        const auto& matrix = graph.GetAdjacencyMatrix();

        // проверка, что граф полный (все ребра ненулевые, кроме диагонали)
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i != j && matrix[i][j] == 0) {
                    throw std::runtime_error("Graph is not complete, TSP may be impossible");
                }
            }
        }

        // параметры
        int ants = n;
        double alpha = 1.0;
        double beta = 2.0;
        double evaporation = 0.1;
        int iterations = 100;

        // инициализация феромонов
        std::vector<std::vector<double>> pheromone(n, std::vector<double>(n, 1.0));
        // лучший результат
        TsmResult best;
        best.distance = INF;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        for (int iter = 0; iter < iterations; ++iter) {
            std::vector<std::vector<int>> paths(ants);
            std::vector<double> distances(ants, 0.0);

            // каждый муравей строит маршрут
            for (int ant = 0; ant < ants; ++ant) {
                std::vector<bool> visited(n, false);
                int start = ant % n; // разные стартовые вершины
                paths[ant].push_back(start);
                visited[start] = true;
                int current = start;

                for (int step = 1; step < n; ++step) {
                    // вычисление вероятностей перехода
                    std::vector<double> prob(n, 0.0);
                    double total = 0.0;
                    for (int next = 0; next < n; ++next) {
                        if (!visited[next] && matrix[current][next] > 0) {
                            double tau = pheromone[current][next];
                            double eta = 1.0 / matrix[current][next];
                            prob[next] = pow(tau, alpha) * pow(eta, beta);
                            total += prob[next];
                        }
                    }
                    if (total == 0.0) { // тупик
                        distances[ant] = INF;
                        break;
                    }
                    // выбор следующей вершины
                    double r = dis(gen);
                    double accum = 0.0;
                    int next = -1;
                    for (int cand = 0; cand < n; ++cand) {
                        if (prob[cand] > 0) {
                            accum += prob[cand] / total;
                            if (r <= accum) {
                                next = cand;
                                break;
                            }
                        }
                    }
                    if (next == -1) { // на всякий случай
                        distances[ant] = INF;
                        break;
                    }
                    visited[next] = true;
                    paths[ant].push_back(next);
                    distances[ant] += matrix[current][next];
                    current = next;
                }
                // возврат в стартовую вершину
                if (distances[ant] < INF && matrix[current][start] > 0) {
                    distances[ant] += matrix[current][start];
                    paths[ant].push_back(start);
                } else {
                    distances[ant] = INF;
                }
            }

            // обновление феромонов
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    pheromone[i][j] *= (1.0 - evaporation);
                }
            }
            for (int ant = 0; ant < ants; ++ant) {
                if (distances[ant] < INF) {
                    double delta = 1.0 / distances[ant];
                    for (size_t k = 0; k < paths[ant].size() - 1; ++k) {
                        int u = paths[ant][k];
                        int v = paths[ant][k+1];
                        pheromone[u][v] += delta;
                        pheromone[v][u] += delta;
                    }
                }
            }

            // обновление лучшего решения
            for (int ant = 0; ant < ants; ++ant) {
                if (distances[ant] < best.distance) {
                    best.distance = distances[ant];
                    best.vertices = paths[ant];
                    for (auto& v : best.vertices) v++; // перевод в 1-нумерацию
                }
            }
        }

        if (best.distance >= INF) {
            throw std::runtime_error("No Hamiltonian cycle found");
        }
        return best;
    }


    namespace {
        const int kInf = std::numeric_limits<int>::max() / 2;

        void ValidateCompleteGraph(const s21::Graph& graph) {
            int n = graph.GetSize();
            const auto& matrix = graph.GetAdjacencyMatrix();

            if (n == 0) {
                throw std::runtime_error("Graph is empty");
            }

            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (i != j && matrix[i][j] == 0) {
                        throw std::runtime_error("Graph is not complete, TSP may be impossible");
                    }
                }
            }
        }

        double CalculateRouteDistance(const std::vector<std::vector<int>>& matrix,
                                      const std::vector<int>& route) {
            double distance = 0.0;
            for (size_t i = 0; i + 1 < route.size(); ++i) {
                int u = route[i];
                int v = route[i + 1];
                if (matrix[u][v] == 0 && u != v) {
                    return kInf;
                }
                distance += matrix[u][v];
            }
            return distance;
        }
    }

    TsmResult GraphAlgorithms::SolveTravelingSalesmanProblemNearestNeighbor(
    const Graph& graph) {
        ValidateCompleteGraph(graph);

        int n = graph.GetSize();
        const auto& matrix = graph.GetAdjacencyMatrix();

        TsmResult best;
        best.distance = kInf;

        for (int start = 0; start < n; ++start) {
            std::vector<bool> visited(n, false);
            std::vector<int> route;
            double total_distance = 0.0;

            int current = start;
            visited[current] = true;
            route.push_back(current);

            for (int step = 1; step < n; ++step) {
                int next_vertex = -1;
                int min_edge = kInf;

                for (int v = 0; v < n; ++v) {
                    if (!visited[v] && matrix[current][v] != 0 &&
                        matrix[current][v] < min_edge) {
                        min_edge = matrix[current][v];
                        next_vertex = v;
                        }
                }

                if (next_vertex == -1) {
                    total_distance = kInf;
                    break;
                }

                visited[next_vertex] = true;
                route.push_back(next_vertex);
                total_distance += matrix[current][next_vertex];
                current = next_vertex;
            }

            if (total_distance < kInf && matrix[current][start] != 0) {
                total_distance += matrix[current][start];
                route.push_back(start);
            } else {
                total_distance = kInf;
            }

            if (total_distance < best.distance) {
                best.distance = total_distance;
                best.vertices = route;
            }
        }

        if (best.distance >= kInf) {
            throw std::runtime_error("No Hamiltonian cycle found");
        }

        for (auto& v : best.vertices) {
            ++v;
        }

        return best;
    }
    TsmResult GraphAlgorithms::SolveTravelingSalesmanProblemBruteForce(
    const Graph& graph) {
        ValidateCompleteGraph(graph);

        int n = graph.GetSize();
        const auto& matrix = graph.GetAdjacencyMatrix();

        if (n > 10) {
            throw std::runtime_error(
                "Brute force TSP is allowed only for graphs with up to 10 vertices");
        }

        std::vector<int> perm;
        for (int i = 1; i < n; ++i) {
            perm.push_back(i);
        }

        TsmResult best;
        best.distance = kInf;

        do {
            std::vector<int> route;
            route.push_back(0);
            for (int v : perm) {
                route.push_back(v);
            }
            route.push_back(0);

            double current_distance = CalculateRouteDistance(matrix, route);

            if (current_distance < best.distance) {
                best.distance = current_distance;
                best.vertices = route;
            }
        } while (std::next_permutation(perm.begin(), perm.end()));

        if (best.distance >= kInf) {
            throw std::runtime_error("No Hamiltonian cycle found");
        }

        for (auto& v : best.vertices) {
            ++v;
        }

        return best;
    }
};