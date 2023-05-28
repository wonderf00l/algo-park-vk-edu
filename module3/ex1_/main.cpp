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
    // std::cout << vertex << std::endl;
    visited[vertex] = true;  // обработка вершины

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
    std::queue<int> q;  // local queue for subgraph traverse

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

int main() {
    size_t vertices_quantity = 5;
    ListGraph list_graph(vertices_quantity);
    // ListGraph graph_(vertices_quantity);
    list_graph.AddEdge(0, 1);
    list_graph.AddEdge(0, 2);
    list_graph.AddEdge(2, 4);
    list_graph.AddEdge(4, 3);
    list_graph.AddEdge(2, 3);

    // graph_.AddEdge(0, 1);
    // graph_.AddEdge(0, 2);
    // graph_.AddEdge(2, 3);
    // graph_.AddEdge(1, 3);
    // graph_.AddEdge(3, 4);

    {
        std::vector<int> verts = list_graph.GetNextVertices(2);
        for (int v : verts) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }

    {
        std::vector<int> verts = list_graph.GetPrevVertices(4);
        for (int v : verts) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }

    dfs(list_graph);
    std::cout << std::endl;
    bfs(list_graph);

    std::cout << std::endl;

    // dfs(graph_);
    // std::cout << std::endl;
    // bfs(graph_);
    // std::cout << std::endl;

    MatrixGraph matrix_graph(list_graph);

    {  // assert векторы равны
        std::vector<int> verts = matrix_graph.GetPrevVertices(3);
        for (int v : verts) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }

    SetGraph s_graph(matrix_graph);

    {
        std::vector<int> verts = s_graph.GetPrevVertices(3);
        for (int v : verts) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }

    ArcGraph a_graph(s_graph);

    {
        std::vector<int> verts = a_graph.GetPrevVertices(3);
        for (int v : verts) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
}
