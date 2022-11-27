#ifndef __GRAPH_ADJACENCY_MATRIX_H__
#define __GRAPH_ADJACENCY_MATRIX_H__

#include <cstdint>

namespace graph { inline namespace v1 { namespace adjacency_matrix {

    enum class GraphType { directed, undirected };

    typedef struct adjacency_matrix_graph graph_t;

    graph_t* create(uint32_t node_count, GraphType type = graph::adjacency_matrix::GraphType::directed);
    
    void destroy(graph_t* graph);

    bool directed(const graph_t* graph);

    bool undirected(const graph_t* graph);

    bool has_edge(const graph_t* graph, uint32_t src_node, uint32_t dest_node);

    bool add_edge(graph_t* graph, uint32_t src_node, uint32_t dest_node);

    void remove_edge(graph_t* graph, uint32_t src_node, uint32_t dest_node);

    void print(const graph_t* graph);

}}} // namespace graph

#endif // __GRAPH_ADJACENCY_MATRIX_H__
