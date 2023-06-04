#include <iostream>
#include <set>
#include <utility>
#include <vector>
#include <numeric>

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

   //private:
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

// void bfs_aux(const IGraph& graph, std::vector<bool>& visited, std::vector<int>& distance, int vertex) {
//     std::set<std::pair<int, int>> priority_queue;

//     // if (vertex == to) {
//     //     return;
//     // }

//     priority_queue.emplace(std::make_pair(0, vertex));
//     visited[vertex] = true;
//     distance[vertex] = 0;

//     while (!priority_queue.empty()) {
//         std::pair<int, int> v = *priority_queue.begin();
//         priority_queue.erase(v);

//         for (const std::pair<int, int>& dist_n_vert : graph.GetNearByVertices(v.second)) {
//             if (!visited[dist_n_vert.second]) {
//                 priority_queue.insert(dist_n_vert);
//                 visited[dist_n_vert.second] = true;
//                 distance[dist_n_vert.second] = dist_n_vert.first;
//             } else if (distance[dist_n_vert.second] > dist_n_vert.first) {
//                 priority_queue.erase(std::make_pair(distance[dist_n_vert.second], dist_n_vert.second));
//                 distance[dist_n_vert.second] = dist_n_vert.first;
//                 priority_queue.insert(std::make_pair(distance[dist_n_vert.second], dist_n_vert.second));
//             }
//         }
//     }
// }
// int Prim(const ListGraph& graph) {
//     std::vector<int> minWeights(graph.VerticesCount(), INT32_MAX), edgeEnds(graph.VerticesCount(), -1);
//     std::vector<bool> visited(graph.VerticesCount(), false);
//     minWeights[0] = 0;
//     std::set<std::pair<int, int>> q;
//     q.insert(std::make_pair(0, 0));
//     for (int i = 0; i < graph.VerticesCount(); ++i) {
//         int v = q.begin()->second;
//         q.erase(q.begin());
        
//         if (visited[v]) {
//             continue;
//         }
        
//         visited[v] = true;
//         for (const std::pair<int, int>& dist_n_vert : graph.GetNearByVertices(v)) {
//             int to = dist_n_vert.first,
//                 cost = dist_n_vert.second;
            
//             if (cost < minWeights[to] && !visited[to]) {
//                 q.erase(std::make_pair(minWeights[to], to));
//                 minWeights[to] = cost;
//                 edgeEnds[to] = v;
//                 q.insert(std::make_pair(minWeights[to], to));
//             }
//         }
//     }   
//     return std::accumulate(minWeights.begin(), minWeights.end(), 0);
// }

// int sum(const std::vector<int>& distance) {
//     int sum = 0;
//     for (int dist : distance) {
//         sum += dist;
//     }
//     return sum;
// }



// int bfs(const IGraph& graph, std::vector<int>& distance) {
//     std::vector<bool> visited(graph.VerticesCount(), false);
//     for (int v = 0; v != graph.VerticesCount(); ++v) {
//         if (!visited[v]) {
//             bfs_aux(graph, visited, distance, v);
//         }
//     }
//     return std::accumulate(distance.begin(), distance.end(), 0);
//     //return sum(distance);
// }

// int min_weight(const IGraph& graph) {
//     std::vector<int> distance(graph.VerticesCount());
//     return bfs(graph, distance);
// }

int Prim(const std::vector<std::vector<std::pair<int, int>>> &adjList) {
    std::vector<int> minWeights(adjList.size(), INT32_MAX), edgeEnds(adjList.size(), -1);
    std::vector<bool> visited(adjList.size(), false);
    minWeights[0] = 0;
    std::set<std::pair<int, int>> q;
    q.insert(std::make_pair(0, 0));
    for (int i = 0; i < adjList.size(); ++i) {
        int v = q.begin()->second;
        q.erase(q.begin());
        
        if (visited[v]) {
            continue;
        }
        
        visited[v] = true;
        for (size_t j = 0; j < adjList[v].size(); ++j) {
            int to = adjList[v][j].first,
                cost = adjList[v][j].second;
            
            if (cost < minWeights[to] && !visited[to]) {
                q.erase(std::make_pair(minWeights[to], to));
                minWeights[to] = cost;
                edgeEnds[to] = v;
                q.insert(std::make_pair(minWeights[to], to));
            }
        }
    }   
    return std::accumulate(minWeights.begin(), minWeights.end(), 0);
}

int main() {
    size_t vertices_quantity, edges_quantity;

    std::cin >> vertices_quantity >> edges_quantity;

    // vertices_quantity += 1;

    ListGraph l_graph(vertices_quantity);

    for (size_t i = 0; i != edges_quantity; ++i) {
        size_t from, to, weight;
        std::cin >> from >> to >> weight;
        from -= 1;
        to -= 1;

        l_graph.AddEdge(from, to, weight);
    }

    // size_t u, w;
    // std::cin >> u >> w;

    std::cout << Prim(l_graph.graph) << std::endl;
}