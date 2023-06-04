/* В ведомости вариант 2 - алгоритм Прима
*/


#include <iostream>
#include <vector>
#include <set>
#include <numeric>

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
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<std::pair<int, int>>> adjList(n);
    int b, e, w;
    for (int i = 0; i < m; ++i) {
        std::cin >> b >> e >> w;
        --b;  // Change tests, please. The other tasks do not require me to decrement vertex indices
        --e;  // This is annoying
        adjList[b].push_back(std::make_pair(e, w));
        adjList[e].push_back(std::make_pair(b, w));
    }
    std::cout << Prim(adjList);
}
