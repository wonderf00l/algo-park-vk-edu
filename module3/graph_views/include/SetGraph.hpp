#pragma once
#include "IGraph.hpp"

class SetGraph : public IGraph {
   public:
    explicit SetGraph(size_t vert_quantity);
    explicit SetGraph(const IGraph& other);

    ~SetGraph();

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

   private:
    std::vector<std::unordered_set<int>> graph;
    size_t vertices_quantity;
};