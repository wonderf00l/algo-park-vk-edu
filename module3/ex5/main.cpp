#include <iostream>
#include <numeric>
#include <set>
#include <utility>
#include <vector>

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to, int weight) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<std::pair<int, int>> GetNearByVertices(int vertex) const = 0;
};

class ListGraph : public IGraph {
   public:
    explicit ListGraph(size_t vert_quantity);

    ~ListGraph();

    void AddEdge(int from, int to, int weight) override;
    int VerticesCount() const override;

    std::vector<std::pair<int, int>> GetNearByVertices(int vertex) const override;

   private:
    std::vector<std::vector<std::pair<int, int>>> graph;
    size_t vertices_quantity;
};

ListGraph::ListGraph(size_t vert_quantity) : graph(vert_quantity), vertices_quantity(vert_quantity) {}

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to, int weight) {
    graph[from].emplace_back(std::make_pair(to, weight));
    graph[to].emplace_back(std::make_pair(from, weight));
}

int ListGraph::VerticesCount() const { return vertices_quantity; }

std::vector<std::pair<int, int>> ListGraph::GetNearByVertices(int vertex) const { return graph[vertex]; }

size_t prima_traverse(const ListGraph& graph) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::vector<size_t> mid_edges(graph.VerticesCount(), INT32_MAX);
    std::vector<size_t> vertices(graph.VerticesCount(), -1);
    mid_edges[0] = 0;
    std::set<std::pair<int, int>> priority_queue;
    priority_queue.emplace(std::make_pair(0, 0));
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        size_t v = priority_queue.begin()->second;
        priority_queue.erase(priority_queue.begin());

        if (visited[v]) {
            continue;
        }

        visited[v] = true;
        for (const std::pair<int, int>& dist_n_vert : graph.GetNearByVertices(v)) {
            size_t to = dist_n_vert.first, edge = dist_n_vert.second;

            if (!visited[to] && edge < mid_edges[to]) {
                priority_queue.erase(std::make_pair(mid_edges[to], to));
                vertices[to] = v;
                mid_edges[to] = edge;
                priority_queue.emplace(std::make_pair(mid_edges[to], to));
            }
        }
    }
    return std::accumulate(mid_edges.begin(), mid_edges.end(), 0);
}

int main() {
    size_t vertices_quantity, edges_quantity;

    std::cin >> vertices_quantity >> edges_quantity;

    ListGraph l_graph(vertices_quantity);

    for (size_t i = 0; i != edges_quantity; ++i) {
        size_t from, to, weight;
        std::cin >> from >> to >> weight;
        --from;
        --to;

        l_graph.AddEdge(from, to, weight);
    }

    std::cout << prima_traverse(l_graph) << std::endl;
}