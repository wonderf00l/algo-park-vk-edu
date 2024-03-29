#include "ListGraph.hpp"

ListGraph::ListGraph(size_t vert_quantity) : graph(vert_quantity), vertices_quantity(vert_quantity) {}

ListGraph::ListGraph(const IGraph& other) : graph(other.VerticesCount()), vertices_quantity(other.VerticesCount()) {
    for (size_t vert = 0; vert != vertices_quantity; ++vert) {
        for (int next_vert : other.GetNextVertices(vert)) {
            AddEdge(vert, next_vert);
        }
    }
}

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to) {
    graph[from].push_back(to);  // one direction - oriented graph
    // graph[to].push_back(from); // now 2 directions - non-oriented
}

int ListGraph::VerticesCount() const { return vertices_quantity; }

std::vector<int> ListGraph::GetNextVertices(int vertex) const { return graph[vertex]; }

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {  // O(V*E)
    std::vector<int> prev_vert;
    for (size_t v = 0; v != graph.size(); ++v) {
        if (static_cast<int>(v) == vertex) {
            continue;
        }
        for (size_t linked_v = 0; linked_v != graph[v].size(); ++linked_v) {
            if (graph[v][linked_v] == vertex) {
                prev_vert.push_back(v);
                break;
            }
        }
    }
    return prev_vert;
}