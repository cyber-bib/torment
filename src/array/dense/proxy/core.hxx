#pragma once

#include <optional>

#include "core.hxx"
#include "iterator.hxx"
#include "iterable.hxx"

namespace torment {
  namespace dense {

    template<class Idx, std::size_t Rk1, std::size_t Rk2>
    constexpr bool is_subset_of (
      std::array<Idx, Rk1> const &arg1,
      std::array<Idx, Rk2> const &arg2
    );

    template<
      std::size_t Axis,
      class Idx,
      std::size_t Rk,
      std::array<Idx, Rk> Sp>
    constexpr std::array<Idx, Rk - 1> remove_axis_shape();

    template<class T, std::size_t N>
    constexpr bool in_range(
      std::array<T, N> const& axes,
      T rank);
    template<class T, std::size_t N>
    constexpr bool in_range(
      std::array<std::optional<T>, N> const& view,
      T rank);

    template<class T, std::size_t N>
    constexpr bool is_sorted_set(std::array<T, N> const& data);

    template<class T, std::size_t N1, std::size_t N2>
    constexpr bool are_disjoint_sets(
      std::array<T, N1> const& lhs,
      std::array<T, N2> const& rhs  );

  template<
    class Idx,
    std::size_t sRk,
    std::size_t pRk   >
  constexpr std::array<Idx, pRk> select_shape(
    std::array<Idx, sRk> const& shape,
    std::array<Idx, pRk> const& proj
  );

    template< class T,
              std::size_t Rk,
              class Idx,
              std::array<Idx, Rk> Sp >
    struct proxy;

    #ifdef _IOSTREAM_ // [

    template< class T,
              std::size_t Rk,
              class Idx,
              std::array<Idx, Rk> Sp>
    std::ostream&
    operator<<(
      std::ostream &os,
      proxy<T,Rk,Idx,Sp> const &arrg  );

    #endif // ] _IOSTREAM_

    template< class T,
              std::size_t Rk              = 0,
              class Idx                   = std::size_t,
              std::array<Idx, Rk> Sp      = std::array<Idx, Rk>{} >
    struct proxy
    : proxy_iterable<proxy<T,Rk,Idx,Sp>>
    {
      typedef proxy_iterable<proxy<T,Rk,Idx,Sp>> iterable_type;

      typedef base<T,Rk,Idx,Sp> _xed_type;

      typedef typename _xed_type::value_type value_type;
      typedef typename _xed_type::index_type index_type;

      typedef index_type proxy_val_t;
      typedef shape_t<Idx, 0> proxy_idx_t;

      typedef std::conditional_t<
        is_dynamic<Rk>,
        shape_t<std::optional<Idx>, 0>,
        std::array<std::optional<Idx>, Rk>
      > view_type;
      static constexpr std::size_t proxy_rank = 0;


      using iterable_type::begin;
      using iterable_type::end;
      using iterable_type::cbegin;
      using iterable_type::cend;

      _xed_type &m_data;
      view_type  m_view;

      explicit proxy(_xed_type &data);
      proxy(_xed_type &data, view_type const& view);

      proxy& bind_view(view_type const& view);

      proxy_val_t operator()(proxy_idx_t const& addr) const;

      value_type& operator[](proxy_idx_t const& addr);
      value_type const& operator[](proxy_idx_t const& addr) const;

      proxy_idx_t shape() const;
      std::size_t size() const;
      proxy_idx_t addr_from(std::size_t const& id) const;

      // template<
      //   std::size_t _Rk = Rk,
      //   typename = std::enable_if_t<is_multidimensional<_Rk>>  >
      // value_type& operator[](index_type const& addr);
      // template<
      //   std::size_t _Rk = Rk,
      //   typename = std::enable_if_t<is_multidimensional<_Rk>>  >
      // value_type const& operator[](index_type const& addr) const;

      #ifdef _IOSTREAM_ // {

      friend  std::ostream&
              operator<< <T,Rk,Idx,Sp> (  std::ostream &os,
                                          proxy const &arg  );

      #endif // } _IOSTREAM_
    };

  };

};
