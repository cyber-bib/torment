#pragma once

#include "core.hxx"

namespace torment {
  namespace dense {

    template<class Index, std::size_t Rank1, std::size_t Rank2>
    constexpr bool is_subset_of (
      std::array<Index, Rank1> const &arg1,
      std::array<Index, Rank2> const &arg2
    ) {
      auto N = std::min(arg1.size(), arg2.size());

      bool dst = true;
      for(std::size_t n = 0; n < N; n++) {
        if(!dst) break;
        dst = arg1[n] <= arg2[n];
      }

      return dst;
    }

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
    struct proxy : base<shape_t<Idx, Rk>, PxRk, Idx, PxSp>
    {
      static_assert(
        PxRk <= Rk,
        "proxy rank \"PxRk\" is not less than or equal to rank \"Rk\".");
      static_assert(
         is_subset_of(PxSp, Sp),
        "proxy shape \"PxSp\" is not a subset of shape \"Sp\".");

      typedef base<shape_t<Idx, Rk>, PxRk, Idx, PxSp> base_type;
      typedef base<T, Rk, Idx, Sp> _xed_type;

      typedef base_type::value_type proxy_val_t;
      typedef base_type::index_type proxy_idx_t;

      typedef _xed_type::value_type value_type;
      typedef _xed_type::index_type index_type;

      // using base_type::base_type;

      _xed_type &m_data;

      proxy(_xed_type &data);




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

  };

};
