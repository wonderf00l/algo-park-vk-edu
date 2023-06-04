#include <vector>
#include <iostream>

class Solution {
public:

    std::vector<int> linked_vertices(const std::vector<std::vector<int>> graph, int vertex) {
        std::vector<int> linked;
        for (int j = 0;j != graph[vertex].size(); ++j) {
            if (graph[vertex][j] && j != vertex) {
                linked.push_back(j);
            }
        }
        return linked;
    }

    void dfs_aux(const std::vector<std::vector<int>>& isConnected, std::vector<bool>& visited, int vertex) {
        visited[vertex] = true;
        
        for (int linked_vertex : linked_vertices(isConnected, vertex)) {
            if (!visited[linked_vertex]) {
                dfs_aux(isConnected, visited, linked_vertex);
            }
        }
    }

    int findCircleNum(std::vector<std::vector<int>>& isConnected) {
        std::vector<bool> visited(isConnected.size(), false);
        int provinces_quantity = 0;
        for (int i = 0; i != isConnected.size(); ++i) {
            if (!visited[i]) {
                ++provinces_quantity;
                dfs_aux(isConnected, visited, i);
            }
        }
        return provinces_quantity;
    }
};

int main () {
    Solution sol;
    std::vector<std::vector<int>> graph = {{1,1,0}, {1,1,0}, {0,0,1}};
    sol.findCircleNum(graph);
}