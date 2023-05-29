#include "SetGraph.hpp"

SetGraph::SetGraph(size_t vert_quantity) : graph(vert_quantity), vertices_quantity(vert_quantity) {}

SetGraph::SetGraph(const IGraph& other) : graph(other.VerticesCount()), vertices_quantity(other.VerticesCount()) {
    for (size_t vert = 0; vert != vertices_quantity; ++vert) {
        for (int next_vert : other.GetNextVertices(vert)) {
            AddEdge(vert, next_vert);
        }
    }
}

SetGraph::~SetGraph() {}

void SetGraph::AddEdge(int from, int to) { graph[from].insert(to); }
int SetGraph::VerticesCount() const { return vertices_quantity; }

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    return std::vector<int>(graph[vertex].begin(), graph[vertex].end());
}
std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prev_vert;
    for (size_t v = 0; v != graph.size(); ++v) {
        if (graph[v].find(vertex) != graph[v].end()) {
            prev_vert.push_back(v);
        }
    }
    return prev_vert;
}