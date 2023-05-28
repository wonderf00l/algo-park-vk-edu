#include "ArcGraph.hpp"

ArcGraph::ArcGraph(size_t vert_quantity) : graph(vert_quantity), vertices_quantity(vert_quantity) {}
ArcGraph::ArcGraph(const IGraph& other) : graph(other.VerticesCount()), vertices_quantity(other.VerticesCount()) {
    for (size_t vert = 0; vert != vertices_quantity; ++vert) {
        for (int next_vert : other.GetNextVertices(vert)) {
            AddEdge(vert, next_vert);
        }
    }
}

ArcGraph::~ArcGraph() {}

void ArcGraph::AddEdge(int from, int to) { graph.push_back(std::make_pair(from, to)); }

int ArcGraph::VerticesCount() const { return vertices_quantity; }

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> next_verts;
    for (const auto& edge : graph) {
        if (edge.first == vertex) {
            next_verts.push_back(edge.second);
        }
    }
    return next_verts;
}
std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prev_verts;
    for (const auto& edge : graph) {
        if (edge.second == vertex) {
            prev_verts.push_back(edge.first);
        }
    }
    return prev_verts;
}