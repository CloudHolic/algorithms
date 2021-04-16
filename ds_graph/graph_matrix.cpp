#include <stdexcept>
#include "graph_matrix.h"

graph_matrix::graph_matrix(int vertices, vector<graph_edge> edges, bool weighted, bool directed)
     : vertices_(vertices), weighted_(weighted), directed_(directed)
{
    vector<graph_edge>::iterator edge_ptr;

    internal_matrix_.resize(vertices);
    for(int i = 0; i < vertices; i++)
    {
        internal_matrix_[i].resize(vertices);
        for(int j = 0; j < vertices; j++)
            internal_matrix_[i][j] = 0;
    }

    for(edge_ptr = edges.begin(); edge_ptr != edges.end(); edge_ptr++)
    {
        if(!weighted && (edge_ptr->weight > 1 || edge_ptr->weight < 0))
            throw std::invalid_argument("Edges should be unweighted.");
        internal_matrix_[edge_ptr->start_vertex][edge_ptr->end_vertex] = edge_ptr->weight;
    }

    if(!directed)
        for(int i = 0; i < vertices; i++)
            for(int j = 0; j < vertices; j++)
                if(internal_matrix_[i][j] != internal_matrix_[j][i])
                    throw std::invalid_argument("Edges should be undirected.");
}

bool graph_matrix::is_bipartite() const
{

}

bool graph_matrix::is_planar() const
{

}