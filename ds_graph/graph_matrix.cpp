#include <stdexcept>
#include <queue>
#include <stack>
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

bool graph_matrix::dfs(int start, void(*worker)(int)) const
{
    vector<bool> visited(vertices_, false);
    stack<int> visited_stack;

    visited[start] = true;
    visited_stack.push(start);
    worker(start);

    while(!visited_stack.empty())
    {
        int source = visited_stack.top();
        visited_stack.pop();

        for(int i = 0; i < vertices_; i++)
        {
            if(i == source)
                continue;

            if(internal_matrix_[source][i] != 0 && !visited[i])
            {
                visited[i] = true;
                visited_stack.push(i);
                worker(i);
            }
        }
    }
}

bool graph_matrix::bfs(int start, void(*worker)(int)) const
{
    vector<bool> visited(vertices_, false);
    queue<int> visited_queue;

    visited[start] = true;
    visited_queue.push(start);
    worker(start);

    while(!visited_queue.empty())
    {
        int source = visited_queue.front();
        visited_queue.pop();

        for(int i = 0; i < vertices_; i++)
        {
            if(i == source)
                continue;

            if(internal_matrix_[source][i] != 0 && !visited[i])
            {
                visited[i] = true;
                visited_queue.push(i);
                worker(i);
            }
        }
    }
}

bool graph_matrix::is_connected() const
{
    vector<bool> visited(vertices_, false);
    stack<int> visited_stack;
    int count = 0;

    visited[0] = true;
    visited_stack.push(0);
    count++;

    while(!visited_stack.empty())
    {
        int source = visited_stack.top();
        visited_stack.pop();

        for(int i = 0; i < vertices_; i++)
        {
            if(i == source)
                continue;

            if(internal_matrix_[source][i] != 0 && !visited[i])
            {
                visited[i] = true;
                visited_stack.push(i);
                count++;
            }
        }
    }

    return count == vertices_;
}

bool graph_matrix::is_bipartite() const
{
    vector<int> colors(vertices_, 0);
    queue<int> color_queue;

    for(int v = 0; v < vertices_; v++)
    {
        if(colors[v] != 0)
            continue;

        colors[v] = 1;
        color_queue.push(v);

        while(!color_queue.empty())
        {
            int source = color_queue.front();
            color_queue.pop();

            if(internal_matrix_[source][source] != 0)
                return false;

            for(int i = 0; i < vertices_; i++)
            {
                if(i == source)
                    continue;

                if(internal_matrix_[source][i] != 0 && colors[i] == 0)
                {
                    colors[i] = colors[source] * -1;
                    color_queue.push(i);
                }
                else if(internal_matrix_[source][i] != 0 && colors[source] == colors[i])
                    return false;
            }
        }
    }

    return true;
}

bool graph_matrix::is_planar() const
{
    if(directed_)
        return false;
}

bool graph_matrix::is_cactus() const
{

}