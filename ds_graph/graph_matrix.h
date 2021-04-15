// Implement graph as adjacency matrix
#pragma once

#include <vector>

using namespace std;

struct graph_edge
{
    int start_vertex, end_vertex;
    int weight;
};

class graph_matrix
{
public:
    // Constructor
    graph_matrix(int size, graph_edge edges[]);

    bool is_weighted() const;
    bool is_directed() const;
    bool is_bipartite() const;
    bool is_planar() const;

private:
    int size_;
    bool weighted_;
    bool directed_;

    vector<vector<int>> internal_matrix_;
};