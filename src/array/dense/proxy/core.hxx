#pragma once

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
    constexpr bool unique(std::array<T, N> const& axes);

    template<class T, std::size_t N1, std::size_t N2>
    constexpr bool disjoint(
      std::array<T, N1> const& a1,
      std::array<T, N2> const& a2  );

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
              std::array<Idx, Rk> Sp,
              std::size_t PxRk,
              std::array<Idx, PxRk> PxSp>
    struct proxy;

    #ifdef _IOSTREAM_ // [

    template< class T,
              std::size_t Rk,
              class Idx,
              std::array<Idx, Rk> Sp,
              std::size_t PxRk,
              std::array<Idx, PxRk> PxSp>
    std::ostream&
    operator<<(
      std::ostream &os,
      proxy<T,Rk,Idx,Sp,PxRk, PxSp> const &arrg  );

    #endif // ] _IOSTREAM_

    template< class T,
              std::size_t Rk              = 0,
              class Idx                   = std::size_t,
              std::array<Idx, Rk> Sp      = std::array<Idx, Rk>{},
              std::size_t PxRk            = Rk,
              std::array<Idx, PxRk> PxSp  = std::array<Idx, PxRk>{}            >
    struct proxy : base<shape_t<Idx, Rk>, PxRk, Idx, PxSp>,
                   proxy_iterable<proxy<T,Rk,Idx,Sp,PxRk,PxSp>>
    {
      static_assert(
        PxRk <= Rk,
        "proxy rank \"PxRk\" is not less than or equal to rank \"Rk\".");
      static_assert(
         is_subset_of(PxSp, Sp),
        "proxy shape \"PxSp\" is not a subset of shape \"Sp\".");

      typedef base<shape_t<Idx, Rk>, PxRk, Idx, PxSp> base_type;
      typedef proxy_iterable<proxy<T,Rk,Idx,Sp,PxRk,PxSp>> iterable_type;

      typedef base<T, Rk, Idx, Sp> _xed_type;

      typedef base_type::value_type proxy_val_t;
      typedef base_type::index_type proxy_idx_t;

      typedef _xed_type::value_type value_type;
      typedef _xed_type::index_type index_type;

      static constexpr std::size_t proxy_rank = PxRk;

      // using base_type::base_type;
      using iterable_type::begin;
      using iterable_type::end;
      using iterable_type::cbegin;
      using iterable_type::cend;

      _xed_type &m_data;

      proxy(_xed_type &data);

      void bind_view(
        std::array<Idx, PxRk> const& free_axes,
        std::array<Idx, Rk - PxRk> const& fixed_axes,
        std::array<Idx, Rk - PxRk> const& fixed_vals);

      proxy_val_t& operator()(proxy_idx_t const& addr);
      proxy_val_t const& operator()(proxy_idx_t const& addr) const;

      value_type& operator[](proxy_idx_t const& addr);
      value_type const& operator[](proxy_idx_t const& addr) const;

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
              operator<< <T,Rk,Idx,Sp,PxRk,PxSp> (  std::ostream &os,
                                                    proxy const &arg  );

      #endif // } _IOSTREAM_
    };

    template<
      class T,
      std::size_t Rk,
      class Idx,
      std::array<Idx, Rk> Sp,
      std::size_t NFree,
      std::array<Idx, NFree> FreeAxes,
      std::size_t NFixed,
      std::array<Idx, NFixed> FixedAxes>
    proxy<
      T,
      Rk,
      Idx,
      Sp,
      NFree,
      select_shape(Sp, FreeAxes)>
    make_view(base<T, Rk, Idx, Sp> &data, std::array<Idx, NFixed> const& fixed_vals);

  };

};
