#pragma once
#include "IGraph.hpp"

class MatrixGraph : public IGraph {
   public:
    explicit MatrixGraph(size_t vert_quantity);
    explicit MatrixGraph(const IGraph& other);

    ~MatrixGraph();

    void AddEdge(int from, int to) override;
    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

   private:
    std::vector<std::vector<int>> graph;
    size_t vertices_quantity;
};