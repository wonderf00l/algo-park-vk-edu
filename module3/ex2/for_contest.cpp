#include <iostream>
#include <queue>
#include <vector>

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNearByVertices(int vertex) const = 0;
};


class ListGraph : public IGraph {
   public:
    explicit ListGraph(size_t vert_quantity);

    ~ListGraph();

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;

    std::vector<int> GetNearByVertices(int vertex) const override;

   private:
    std::vector<std::vector<int>> graph;
    size_t vertices_quantity;
};

ListGraph::ListGraph(size_t vert_quantity) : graph(vert_quantity), vertices_quantity(vert_quantity) {}

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to) {
    graph[from].push_back(to);
    graph[to].push_back(from);
}

int ListGraph::VerticesCount() const { return vertices_quantity; }

std::vector<int> ListGraph::GetNearByVertices(int vertex) const {  // O(V*E)
    return graph[vertex];
}

void bfs_aux(const IGraph& graph, std::vector<bool>& visited, std::vector<int>& distance, std::vector<int>& dist_count,
             int vertex, int to) {
    std::queue<int> q;

    if (vertex == to) {  // если оказалось так, что текущая вершина - целевая, значит, мы вошли в функцию в рамках внешнего цикла bfs.
               // Поскольку обход всегда начинается с корневой вершины, то если от корня к цели можно проложить маршрут через
               // другие вершины, вершина to должна оказаться в очереди, то есть она точно будет обработана во внутреннем
               // цикле. А если пришли извне, значит, она обособлена от корневой вершины, оставляем значение по умолчанию
        return;
    }

    q.push(vertex);
    visited[vertex] = true;
    distance[vertex] = 0;
    dist_count[vertex] = 1;

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int next_vert : graph.GetNearByVertices(v)) {

            if (!visited[next_vert]) {
                q.push(next_vert);
                visited[next_vert] = true;
                distance[next_vert] = distance[v] + 1;
                dist_count[next_vert] = dist_count[v];
            } else if (distance[v] + 1 <
                       distance[next_vert]) {
                distance[next_vert] = distance[v] + 1;
                dist_count[next_vert] = dist_count[v];
            } else if (distance[v] + 1 == distance[next_vert]) {
                dist_count[next_vert] += dist_count[v];
            }
        }
    }
}

int bfs(const IGraph& graph, std::vector<int>& distance, std::vector<int>& dist_count, int from, int to) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int v = from; v != graph.VerticesCount() + from; ++v) {
        if (!visited[v % graph.VerticesCount()]) {
            bfs_aux(graph, visited, distance, dist_count, v % graph.VerticesCount(), to);
        }
    }
    return dist_count[to];
}

int dist_count(const IGraph& graph, int from, int to) {
    std::vector<int> distance(graph.VerticesCount());
    std::vector<int> dist_count(graph.VerticesCount());
    return bfs(graph, distance, dist_count, from, to);
}

int main() {
    size_t vertices_quantity, edges_quantity;

    std::cin >> vertices_quantity >> edges_quantity;

    ListGraph l_graph(vertices_quantity);

    for (size_t i = 0; i != edges_quantity; ++i) {
        size_t from, to;
        std::cin >> from >> to;

        l_graph.AddEdge(from, to);
    }

    size_t u, w;
    std::cin >> u >> w;

    std::cout << dist_count(l_graph, u, w) << std::endl;
}