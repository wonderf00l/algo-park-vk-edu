#pragma once
#include "IGraph.hpp"

class ArcGraph : public IGraph {
   public:
    explicit ArcGraph(size_t vert_quantity);
    explicit ArcGraph(const IGraph& other);

    ~ArcGraph();

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

   private:
    std::vector<std::pair<int, int>> graph;
    size_t vertices_quantity;
};