// ДОПУСТИМЫ КРАТНЫЕ РЕБРА(ИДЕМ ПО САМОГО ДЕШЕВОМУ ИЗ КРАТНЫХ РЕБЕР)
// ТАКЖЕ УЧИТЫВАЕМ РАЗРЕЖЕННЫЕ ГРАФЫ

#include <iostream>
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
    graph[from].emplace_back(std::make_pair(weight, to));
    graph[to].emplace_back(std::make_pair(weight, from));
}

int ListGraph::VerticesCount() const { return vertices_quantity; }

std::vector<std::pair<int, int>> ListGraph::GetNearByVertices(int vertex) const {  // O(V*E)
    return graph[vertex];
}

void bfs_aux(const IGraph& graph, std::vector<bool>& visited, std::vector<int>& distance, int vertex, int to) {
    std::set<std::pair<int, int>> priority_queue;

    if (vertex ==
        to) {  // если оказалось так, что текущая вершина - целевая, значит, мы вошли в функцию в рамках внешнего цикла bfs.
        // Поскольку обход всегда начинается с корневой вершины, то если от корня к цели можно проложить маршрут через
        // другие вершины, вершина to должна оказаться в очереди, то есть она точно будет обработана во внутреннем
        // цикле. А если пришли извне, значит, она обособлена от корневой вершины, оставляем значение по умолчанию
        return;
    }

    priority_queue.emplace(std::make_pair(0, vertex));
    visited[vertex] = true;
    distance[vertex] = 0;

    while (!priority_queue.empty()) {
        std::pair<int, int> v = *priority_queue.begin();
        priority_queue.erase(v);

        for (const std::pair<int, int>& dist_n_vert : graph.GetNearByVertices(v.second)) {
            if (!visited[dist_n_vert.second]) {
                priority_queue.insert(dist_n_vert);
                visited[dist_n_vert.second] = true;
                distance[dist_n_vert.second] = distance[v.second] + dist_n_vert.first;
            } else if (distance[dist_n_vert.second] > distance[v.second] + dist_n_vert.first) {
                priority_queue.erase(std::make_pair(distance[dist_n_vert.second], dist_n_vert.second));
                distance[dist_n_vert.second] = distance[v.second] + dist_n_vert.first;
                priority_queue.insert(std::make_pair(distance[dist_n_vert.second], dist_n_vert.second));
            }
        }
    }
}

int bfs(const IGraph& graph, std::vector<int>& distance, int from, int to) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int v = from; v != graph.VerticesCount() + from; ++v) {
        if (!visited[v % graph.VerticesCount()]) {
            bfs_aux(graph, visited, distance, v % graph.VerticesCount(), to);
        }
    }
    return distance[to];  // в итоге в distance будет лежать min расстояние от from до to
}

int min_dist(const IGraph& graph, int from, int to) {
    std::vector<int> distance(graph.VerticesCount());  // вектор кратчайших расстояний от заданной корневой вершины
    return bfs(graph, distance, from, to);
}

int main() {
    size_t vertices_quantity, edges_quantity;

    std::cin >> vertices_quantity >> edges_quantity;

    ListGraph l_graph(vertices_quantity);

    for (size_t i = 0; i != edges_quantity; ++i) {
        size_t from, to, weight;
        std::cin >> from >> to >> weight;

        l_graph.AddEdge(from, to, weight);
    }

    size_t u, w;
    std::cin >> u >> w;

    std::cout << min_dist(l_graph, u, w) << std::endl;
}