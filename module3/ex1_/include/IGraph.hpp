#pragma once
#include <iostream>
#include <string>
#include <typeinfo>
#include <unordered_set>
#include <utility>
#include <vector>

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};
