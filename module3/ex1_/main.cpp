#include <iostream>
#include <functional>
#include "ListGraph.hpp"

void print(int vertex) {
    std::cout << vertex << " ";
}

void dfs_aux(const IGraph& graph, std::vector<bool> visited, int vertex, std::function<void(int)> processor=print) {

    processor(vertex);
    visited[vertex] = true; // обработка вершины

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

int main() {
    size_t vertices_quantity = 5;
    ListGraph graph(vertices_quantity);

    graph.AddEdge(0,1);
    graph.AddEdge(0,2);
    graph.AddEdge(2,4);
    graph.AddEdge(4,3);  
    graph.AddEdge(2,3);

    // {
    //     std::vector<int> verts = graph.GetNextVertices(2);
    //     for (int v : verts) {
    //         std::cout << v << " ";
    //     }
    // }

    // {
    //     std::vector<int> verts = graph.GetPrevVertices(2);
    //     for (int v : verts) {
    //         std::cout << v << " " ;
    //     }
    // } // DEBUG

    dfs(graph);
    
}