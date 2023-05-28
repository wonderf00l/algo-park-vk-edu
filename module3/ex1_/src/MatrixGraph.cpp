#include "MatrixGraph.hpp"

MatrixGraph::MatrixGraph(size_t vert_quantity)
    : graph(vert_quantity, std::vector<int>(vert_quantity, 0)), vertices_quantity(vert_quantity) {}

MatrixGraph::MatrixGraph(const IGraph& other)
    : graph(other.VerticesCount(), std::vector<int>(other.VerticesCount(), 0)), vertices_quantity(other.VerticesCount()) {
    for (size_t vert = 0; vert != vertices_quantity; ++vert) {
        for (int next_vert : other.GetNextVertices(vert)) {
            AddEdge(vert, next_vert);
        }
    }
}

MatrixGraph::~MatrixGraph() {}

void MatrixGraph::AddEdge(int from, int to) { graph[from][to] += 1; }

int MatrixGraph::VerticesCount() const { return vertices_quantity; }

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    std::vector<int> next_verts;
    for (size_t v = 0; v != vertices_quantity; ++v) {
        if (graph[vertex][v]) {
            next_verts.push_back(v);
        }
    }
    return next_verts;
}
std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prev_verts;
    for (size_t v = 0; v != vertices_quantity; ++v) {
        if (graph[v][vertex]) {
            prev_verts.push_back(v);
        }
    }
    return prev_verts;
}