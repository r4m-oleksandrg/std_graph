//
//  Authors
//  Copyright
//  License
//  No Warranty Disclaimer
//

#include <iomanip>
#include <iostream>
#include <list>

//#include "dijkstra.hpp"
#include "graph/algorithm/dijkstra_shortest_paths.hpp"
#include "ospf-graph.hpp"
#include "utilities.hpp"

int main() {

  static_assert(std::graph::adjacency_list<decltype(ospf_index_adjacency_list)>);

  //auto d = dijkstra(ospf_index_adjacency_list, 5UL);
  std::vector<size_t> d(size(ospf_index_adjacency_list));
  std::vector<size_t> p(size(ospf_index_adjacency_list));
  std::graph::init_shortest_paths(d, p);
  std::graph::dijkstra_shortest_paths(ospf_index_adjacency_list, 5UL, d, p, [](auto&& ee) { return std::get<1>(ee); });

  std::cout << "----------------" << std::endl;
  std::cout << "Contents of ospf_index_adjacency_list (the correct answer)" << std::endl;

  for (size_t i = 0; i < size(ospf_vertices); ++i) {
    std::cout << std::setw(6) << ospf_vertices[i] << std::setw(6) << d[i] << std::endl;
  }

  std::cout << "----------------" << std::endl;
  std::cout << "Results from make_property_graph(osp_vertices)" << std::endl;

  auto G = make_property_graph(ospf_vertices, ospf_edges, true);

  // Alternatively
  auto H = make_property_graph<decltype(ospf_vertices), decltype(ospf_edges),
                               std::vector<std::list<std::tuple<size_t, size_t>>>>(ospf_vertices, ospf_edges, true);
  auto I = make_property_graph<decltype(ospf_vertices), decltype(ospf_edges),
                               std::vector<std::vector<std::tuple<size_t, size_t>>>>(ospf_vertices, ospf_edges, true);

  static_assert(std::graph::adjacency_list<decltype(G)>);

  //auto e = dijkstra(G, 5UL, [](auto&& ee) { return std::get<1>(ee); });
  p.resize(std::graph::num_vertices(G));
  std::vector<size_t> e(std::graph::num_vertices(G));
  std::graph::init_shortest_paths(e, p);
  std::graph::dijkstra_shortest_paths(G, 5UL, e, p, [](auto&& ee) { return std::get<1>(ee); });

  for (size_t i = 0; i < size(ospf_vertices); ++i) {
    std::cout << std::setw(6) << ospf_vertices[i] << std::setw(6) << e[i] << std::endl;
  }

  std::cout << "----------------" << std::endl;
  std::cout << "Results from make_index_graph(osp_vertices)" << std::endl;

  auto J = make_index_graph(ospf_vertices, ospf_edges, true);

  //std::graph::dijkstra_shortest_paths(G, 5UL, e, p, w);

  //auto f = dijkstra(J, 5, [](auto&& ee) { return std::get<2>(ospf_edges[std::get<1>(ee)]); });
  p.resize(std::graph::num_vertices(G));
  std::vector<size_t> f(std::graph::num_vertices(G));
  std::graph::init_shortest_paths(f, p);
  std::graph::dijkstra_shortest_paths(J, 5UL, f, p, [](auto&& ee) { return std::get<2>(ospf_edges[std::get<1>(ee)]); });

  for (size_t i = 0; i < size(ospf_vertices); ++i) {
    std::cout << std::setw(6) << ospf_vertices[i] << std::setw(6) << e[i] << std::endl;
  }

  return 0;
}
