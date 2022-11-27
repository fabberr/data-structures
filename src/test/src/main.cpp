#include <graphs/adjacency_matrix.hpp>

int main() {
    using namespace graph::adjacency_matrix;

    graph_t* g1 = create(5, GraphType::undirected);

    add_edge(g1, 0, 0);
    add_edge(g1, 0, 1);
    add_edge(g1, 0, 2);
    add_edge(g1, 1, 2);
    add_edge(g1, 1, 3);
    add_edge(g1, 2, 4);
    add_edge(g1, 3, 1);
    add_edge(g1, 3, 0);

    print(g1);

    destroy(g1);
}
