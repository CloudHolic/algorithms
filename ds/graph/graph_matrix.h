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
    graph_matrix(int vertices, vector<graph_edge> edges, bool weighted, bool directed);

    bool dfs(const int start, void(*worker)(int)) const;
    bool bfs(const int start, void(*worker)(int)) const;

    bool is_connected() const;
    bool is_bipartite() const;
    bool is_planar() const;
    bool is_cactus() const;

private:
    int vertices_;
    bool weighted_;
    bool directed_;

    vector<vector<int>> internal_matrix_;
};