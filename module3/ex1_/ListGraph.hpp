#include "IGraph.hpp"

class ListGraph : public IGraph {
public:    
    ListGraph(size_t vert_quantity);
    ListGraph(const IGraph& other);

    ~ListGraph();
	
    void AddEdge(int from, int to) override;
	int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> graph;
    size_t vertices_quantity;
}; 