// C++ standard library
#include <string_view>

// C standard library
#include <cstdint>
#include <cstdlib>

// lib{fmt}
#include <fmt/format.h>

// declarations
#include <graphs/adjacency_matrix.hpp>

constexpr static uint32_t MAX_NODES = 2048;

struct graph::adjacency_matrix::adjacency_matrix_graph {
    GraphType type = GraphType::directed;
    uint32_t node_count = 0;
    bool** edges = nullptr;
};

namespace utils {

    /**
     * @brief Prints a graph edge to stdout in Graphviz's dot format.
     * 
     * @param graph const pointer to an adjacency matrix graph data structure.
     * @param src_node Index of the source node in the adjacency matrix.
     * @param dest_node Index of the destination node in the adjacency matrix.
     * @param directed Flag, used to determine final output format (true: digraph or false: graph edge)
    */
    constexpr static 
    auto print_node_if_has_edge = 
    [](const graph::adjacency_matrix::graph_t* graph, uint32_t src_node, uint32_t dest_node, bool directed) -> void {
        if (graph::adjacency_matrix::has_edge(graph, src_node, dest_node)) {
            // each line in format: `  <from-node> ('->' | '--') <to-node>';'`
            fmt::print(stderr, "\t{} {} {};\n", src_node, (directed ? "->" : "--"), dest_node);
        }
    };

    /**
     * @brief Checks if an edge is out of bounds for a given graph, given its source and destination nodes.
     * 
     * @param graph const pointer to an adjacency matrix graph data structure.
     * @param src_node Index of the source node in the adjacency matrix.
     * @param dest_node Index of the destination node in the adjacency matrix.
     * @param caller Signature of the function that has called this utility. Only used if error logging is enabled.
     * @param err_logging Flag, enables or disables output to stderr. Optional, default is enabled.
     * 
     * @returns `true` if any of the nodes are out of range for the given graph, `false` otherwise.
    */
    constexpr static
    auto invalid_edge_range = 
    [](const graph::adjacency_matrix::graph_t* graph, uint32_t src_node, uint32_t dest_node, const std::string_view caller, bool err_logging = true) -> bool {

        if (graph == nullptr || graph->edges == nullptr) {
            if (err_logging) {
                fmt::print(stderr, 
                    "[error] In `{}`:\n"
                    "\tUnable to access graph data.\n",
                    caller
                );
            }
            return true;
        }
        if (src_node >= graph->node_count || dest_node >= graph->node_count) {
            if (err_logging) {
                fmt::print(stderr, 
                    "[error] In `{}`:\n"
                    "\tNode index out is of range! Interval must be in the range [0, {}).\n", 
                    caller, graph->node_count
                );
            }
            return true;
        }
        return false;
    };

}

graph::adjacency_matrix::graph_t* graph::adjacency_matrix::create(uint32_t node_count, GraphType type) {

    if (node_count > MAX_NODES) {
        fmt::print(stderr, 
            "[error] In `ggraph::adjacency_matrix::create(uint32_t)`:\n"
            "\tUnable to create a graph with {} nodes. Max number of nodes is set to {}.\n",
            node_count, MAX_NODES
        );
        return nullptr;
    }

    // allocate memory for and initialize struct
    graph_t* g = (graph_t*)malloc(sizeof(graph_t));
    if (g == nullptr) {
        fmt::print(stderr, 
            "[error] In `graph::adjacency_matrix::create(uint32_t)`:\n"
            "\tFailed to allocate memory for graph structure.\n"
        );
        return nullptr;
    }
    g->type = type;
    g->node_count = node_count;

    // allocate adjacency matrix rows
    g->edges = (bool**)calloc(sizeof(bool*), g->node_count);
    if (g->edges == nullptr) {
        fmt::print(stderr, 
            "[error] In `graph::adjacency_matrix::create(uint32_t)`:\n"
            "\tFailed to allocate memory for adjacency matrix.\n"
        );
        free(g);
        return nullptr;
    }

    // for each row, allocate an array of boolean values to represent edges
    auto failed = false;
    for (uint32_t row = 0; row < g->node_count; ++row) {
        g->edges[row] = (bool*)calloc(sizeof(bool), g->node_count);
        if (g->edges[row] == nullptr) {
            fmt::print(stderr, 
                "[error] In `graph::adjacency_matrix::create(uint32_t)`:\n"
                "\tFailed to allocate memory for adjacency matrix.\n"
            );
            failed = true;
            break;
        }
    }

    // allocator error, do cleanup
    if (failed) {
        destroy(g);
    }
    return g;
}

void graph::adjacency_matrix::destroy(graph_t* graph) {
    
    if (graph == nullptr) { return; }

    if (graph->edges) {
        for (uint32_t row = 0; row < graph->node_count; ++row) {
            if (graph->edges[row]) {
                free(graph->edges[row]);
            }
        }
        free(graph->edges);
        graph->edges = nullptr;
    }

    free(graph);
    graph = nullptr;
}

bool graph::adjacency_matrix::directed(const graph_t* graph) {
    return (graph != nullptr && graph->type == GraphType::directed);
}

bool graph::adjacency_matrix::undirected(const graph_t* graph) {
    return (graph != nullptr && graph->type == GraphType::undirected);
}

bool graph::adjacency_matrix::has_edge(const graph_t* graph, uint32_t src_node, uint32_t dest_node) {
    bool valid = (
        graph != nullptr && 
        graph->edges != nullptr && 
        (src_node < graph->node_count && dest_node < graph->node_count)
    );
    
    return (valid) ? (graph->edges[src_node][dest_node]) : false;
}

bool graph::adjacency_matrix::add_edge(graph_t* graph, uint32_t src_node, uint32_t dest_node) {

    if (utils::invalid_edge_range(graph, src_node, dest_node, 
        "graph::adjacency_matrix::add_edge(graph_t *, uint32_t, uint32_t)")
    ) { return false; }

    const bool _directed = directed(graph);

    // add edge if it doesn't already exist
    if (not graph->edges[src_node][dest_node]) {
        graph->edges[src_node][dest_node] = true;

        // if undirected graph, add inverse of the edge as well
        if (not _directed && not graph->edges[dest_node][src_node]) {
            graph->edges[dest_node][src_node] = true;
        }
    }

    // return successfully only if all edges were added
    return (_directed) 
        ? graph->edges[src_node][dest_node] 
        : graph->edges[src_node][dest_node] && graph->edges[dest_node][src_node]
    ;
}

void graph::adjacency_matrix::remove_edge(graph_t* graph, uint32_t src_node, uint32_t dest_node) {
    
    if (utils::invalid_edge_range(graph, src_node, dest_node, 
        "graph::adjacency_matrix::remove_edge(graph_t *, uint32_t, uint32_t)")
    ) { return; }

    // remove edge if it exists
    if (graph->edges[src_node][dest_node]) {
        graph->edges[src_node][dest_node] = false;

        // if undirected graph, remove inverse of the edge as well
        if (not directed(graph) && graph->edges[dest_node][src_node]) {
            graph->edges[dest_node][src_node] = false;
        }
    }
}

void graph::adjacency_matrix::print(const graph_t* graph) {

    const bool _directed = directed(graph);

    // BEGIN graph
    fmt::print("{} {{\n", _directed ? "digraph" : "graph");

    // print edge list, if any
    for (uint32_t row = 0; graph && row < graph->node_count; ++row) {
        // if undirected, start col from an offset to prevent printing duplicate edges
        // essentially, traverse only half the adjacency matrix if undirected
        const uint32_t offset = _directed ? 0 : row;
        for (uint32_t col = offset; col < graph->node_count; ++col) {
            utils::print_node_if_has_edge(graph, row, col, _directed);
        }
    }

    // END graph, flush stdout
    fmt::print("}}\n");
    fflush(stdout);
}

/**
 * @todo
 *   [x] Convert to a CMake project
 *       [x] targets: graphs, viz, tests
 *       [x] link graphs against lib{fmt}
 *   [x] Use lib{fmt} for outputting to stdout and stderr
 *   [ ] Create utility for logging to standard streams
 *   [ ] Use C++ features
 *       [ ] Make this a class instead so you don't have to keep passing pointers to graphs around
 *       [ ] Add private const member that indicates whether the graph is directed or not (initialize on constructor)
 *       [ ] use new and delete instead of malloc/calloc and free
 *   [ ] Use managed pointers for holding edge data (std::unique_ptr)
 *   [ ] Implemet a graph_node_t structure to hold edge information instead of using just a matrix of boolean values
 *   [ ] Implement a graph using the adjacency list approach
*/
