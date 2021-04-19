// Implement graph as adjacency list
#pragma once

#include <vector>

using namespace std;

struct graph_edge
{
    int start_vertex, end_vertex;
    int weight;
};

class graph_list
{
public:
    // Constructor
    graph_list(int vertices, vector<graph_edge> edges, bool weighted, bool directed);

    bool is_connected() const;
    bool is_bipartite() const;
    bool is_planar() const;

    bool dfs(int start, void(*worker)(int)) const;
    bool bfs(int start, void(*worker)(int)) const;

private:
    struct node
    {
        int vertex, weight;
        node* next_node;
    };

    int vertices_;
    bool weighted_;
    bool directed_;

    vector<node*> internal_list_;
};