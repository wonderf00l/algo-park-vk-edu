#include <functional>
#include <iostream>
#include <queue>

#include "ArcGraph.hpp"
#include "ListGraph.hpp"
#include "MatrixGraph.hpp"
#include "SetGraph.hpp"

void print(int vertex) { std::cout << vertex << " "; }

void dfs_aux(const IGraph& graph, std::vector<bool>& visited, int vertex, std::function<void(int)> processor = print) {
    processor(vertex);
    visited[vertex] = true;

    for (int next_vert : graph.GetNextVertices(vertex)) {
        if (!visited[next_vert]) {
            dfs_aux(graph, visited, next_vert);
        }
    }
}

void dfs(const IGraph& graph) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int v = 0; v != graph.VerticesCount(); ++v) {
        if (!visited[v]) {
            dfs_aux(graph, visited, v);
        }
    }
}

void bfs_aux(const IGraph& graph, std::vector<bool>& visited, int vertex, std::function<void(int)> processor = print) {
    std::queue<int> q;

    q.push(vertex);
    visited[vertex] = true;

    while (!q.empty()) {
        int v = q.front();
        processor(v);
        q.pop();

        for (int next_vert : graph.GetNextVertices(v)) {
            if (!visited[next_vert]) {
                q.push(next_vert);
                visited[next_vert] = true;
            }
        }
    }
}

void bfs(const IGraph& graph) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int v = 0; v != graph.VerticesCount(); ++v) {
        if (!visited[v]) {
            bfs_aux(graph, visited, v);
        }
    }
}

void output(const std::vector<int>& res) {
    for (int v : res) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

void test_next_verts(const IGraph& graph1, const IGraph& graph2, size_t verts_count) {
    std::cout << "NEXT VERTS: " << std::endl;
    for (size_t i = 0; i != verts_count; ++i) {
        std::cout << "GRAPH1: " << std::endl;
        output(graph1.GetNextVertices(i));
        std::cout << "GRAPH2: " << std::endl;
        output(graph2.GetNextVertices(i));
    }
}

void test_prev_verts(const IGraph& graph1, const IGraph& graph2, size_t verts_count) {
    std::cout << "PREV VERTS: " << std::endl;
    for (size_t i = 0; i != verts_count; ++i) {
        std::cout << "GRAPH1: " << std::endl;
        output(graph1.GetPrevVertices(i));
        std::cout << "GRAPH2: " << std::endl;
        output(graph2.GetPrevVertices(i));
    }
}

int main() {
    size_t vertices_quantity = 8;
    ListGraph list_graph(vertices_quantity);

    list_graph.AddEdge(0, 1);
    list_graph.AddEdge(0, 2);
    list_graph.AddEdge(0, 3);
    list_graph.AddEdge(0, 7);
    list_graph.AddEdge(1, 7);
    list_graph.AddEdge(2, 6);
    list_graph.AddEdge(2, 7);
    list_graph.AddEdge(3, 1);
    list_graph.AddEdge(3, 4);
    list_graph.AddEdge(3, 5);
    list_graph.AddEdge(4, 1);
    list_graph.AddEdge(5, 6);
    list_graph.AddEdge(6, 7);

    MatrixGraph matrix_graph(list_graph);
    SetGraph set_graph(matrix_graph);
    ArcGraph arc_graph(set_graph);

    test_next_verts(matrix_graph, arc_graph, vertices_quantity);
    test_prev_verts(matrix_graph, arc_graph, vertices_quantity);
}