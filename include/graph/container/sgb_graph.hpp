#pragma once

/**
 * git clone https://github.com/ascherer/sgb
 * sudo apt-get install sgb
 */

#ifndef _WINVER
#define ENABLE_SGB
#endif

#ifdef ENABLE_SGB
#  include "container_utility.hpp"

extern "C" {
#  include "/home/phil/sgb/gb_graph.h"
}

namespace std::graph::container {

using sgb_graph = graph_struct;

using sgb_vertex = vertex_struct;

using sgb_edge = arc_struct;

template <class G>
requires convertible_to<G, sgb_graph>
constexpr auto tag_invoke(::std::graph::tag_invoke::vertices_fn_t, G&& g) {
  return std::ranges::subrange(g.vertices, g.vertices + g.n);
}


class sgb_edge_iterator {
public:
  using value_type = sgb_edge;

  using pointer         = value_type*;
  using const_pointer   = const value_type*;
  using reference       = value_type&;
  using const_reference = const value_type&;

  using size_type         = decltype(declval<sgb_graph>().n); //long;
  using difference_type   = size_type;
  using iterator_category = forward_iterator_tag;

public:
  constexpr sgb_edge_iterator()                         = default;
  constexpr sgb_edge_iterator(const sgb_edge_iterator&) = default;
  constexpr sgb_edge_iterator(sgb_edge_iterator&&)      = default;

  constexpr sgb_edge_iterator& operator=(const sgb_edge_iterator&) = default;
  constexpr sgb_edge_iterator& operator=(sgb_edge_iterator&&)      = default;

  constexpr sgb_edge_iterator(sgb_graph& g, sgb_vertex& u, bool begin) : g_(&g) {
    if (begin)
      uv_ = u.arcs;
  }

  constexpr reference operator*() const { return *uv_; }
  constexpr pointer   operator->() const { return uv_; }

  constexpr sgb_edge_iterator& operator++() {
    if (uv_)
      uv_ = uv_->next;
    return *this;
  }
  constexpr sgb_edge_iterator operator++(int) {
    auto tmp = *this;
    ++(*this);
    return tmp;
  }

  constexpr bool operator==(const sgb_edge_iterator& rhs) const { return g_ == rhs.g_ && uv_ == rhs.uv_; }

private:
  sgb_graph* g_  = nullptr;
  sgb_edge*  uv_ = nullptr;
};

constexpr auto tag_invoke(::std::graph::tag_invoke::edges_fn_t, sgb_graph& g, sgb_vertex& u) {
  return std::ranges::subrange<sgb_edge_iterator>(sgb_edge_iterator(g, u, true), sgb_edge_iterator(g, u, false));
}

constexpr auto tag_invoke(::std::graph::tag_invoke::target_id_fn_t, const sgb_graph& g, const sgb_edge& uv) noexcept {
  return uv.tip - g.vertices;
}
constexpr auto tag_invoke(::std::graph::tag_invoke::target_fn_t, const sgb_graph& g, const sgb_edge& uv) noexcept {
  return *uv.tip;
}

constexpr auto& tag_invoke(::std::graph::tag_invoke::vertex_value_fn_t, sgb_graph& g, sgb_vertex& u) { return u; }

constexpr auto& tag_invoke(::std::graph::tag_invoke::edge_value_fn_t, sgb_graph& g, sgb_edge& uv) { return uv; }


#  if 0
tag_invoke(::std::graph::tag_invoke::vertex_value_fn_t, sgb_graph& g, sgb_vertex& u) {
  //static_assert(ranges::contiguous_range<row_index_vector>, "row_index_ must be a contiguous range to evaluate uidx");
  //auto            uidx     = g.index_of(u);
  //csr_row_values& row_vals = g;
  //return row_vals.v_[uidx];
  
}
friend constexpr const vertex_value_type&
tag_invoke(::std::graph::tag_invoke::vertex_value_fn_t, const graph_type& g, const vertex_type& u) {
  static_assert(ranges::contiguous_range<row_index_vector>, "row_index_ must be a contiguous range to evaluate uidx");
  auto                  uidx     = g.index_of(u);
  const csr_row_values& row_vals = g;
  return row_vals.v_[uidx];
}
#  endif //0

} // namespace std::graph::container
#endif // ENABLE_SGB
