#include <stdexcept>
#include <queue>
#include "graph_list.h"

graph_list::graph_list(int vertices, vector<graph_edge> edges, bool weighted, bool directed)
    : vertices_(vertices), weighted_(weighted), directed_(directed)
{
    vector<graph_edge>::iterator edge_ptr;

    internal_list_.resize(vertices);
    for(int i = 0; i < vertices; i++)
        internal_list_[i] = nullptr;

    for(edge_ptr = edges.begin(); edge_ptr != edges.end(); edge_ptr++)
    {
        if(!weighted && edge_ptr->weight != 1)        
            throw std::invalid_argument("Edges should be unweighted.");

        node* current = internal_list_[edge_ptr->start_vertex];
        while(true)
        {
            if(current == nullptr)
            {
                node* new_node = new node();
                new_node->vertex = edge_ptr->end_vertex;
                new_node->weight = edge_ptr->weight;
                new_node->next_node = nullptr;

                current->next_node = new_node;
                break;
            }

            current = current->next_node;
        }
    }

    if(!directed)
    {
        for(int i = 0; i < vertices; i++)
        {
            node* current = internal_list_[i];
            while(true)
            {
                if(current == nullptr)
                    break;

                node* target = internal_list_[current->vertex];
                while(true)
                {
                    if(target == nullptr)
                        throw std::invalid_argument("Edges should be undirected.");
                    if(target->vertex == i)
                        break;
                    target = target->next_node;
                }

                current = current->next_node;
            }
        }
    }
}

bool graph_list::is_connected() const
{
    
}

bool graph_list::is_bipartite() const
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

            node* new_node = internal_list_[source];
            while(true)
            {
                if(new_node == nullptr)
                    break;
                
                if(new_node->vertex == source)
                    return false;
                
                if(colors[new_node->vertex] == 0)
                {
                    colors[new_node->vertex] = colors[source] * -1;
                    color_queue.push(new_node->vertex);
                }
                else if(colors[new_node->vertex] == colors[source])
                    return false;
                
                new_node = new_node->next_node;
            }
        }
    }

    return true;
}

bool graph_list::is_planar() const
{
    if(directed_)
        return false;
}

bool graph_list::dfs(int start, void(*worker)(int)) const
{

}

bool graph_list::bfs(int start, void(*worker)(int)) const
{
    
}