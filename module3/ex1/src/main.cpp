#include <iostream>
#include "ListGraph.hpp"

int main() {
    size_t vertices_quantity = 5;
    ListGraph graph(vertices_quantity);

    graph.AddEdge(0,1);
    graph.AddEdge(0,2);
    graph.AddEdge(2,4);
    graph.AddEdge(4,3);
    graph.AddEdge(2,3);

    std::vector<int> verts = graph.GetNextVertices(2);
    for (int v : verts) {
        std::cout << v << " ";
    }

}