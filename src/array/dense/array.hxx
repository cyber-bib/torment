#pragma once

#include "core.hxx"
#include "proxy.hxx"
#include "boost/operators.hpp"

namespace torment {

  // template<class LHS, class RHS = LHS>
  // struct multiplicable {

  //   friend LHS& operator*=(LHS& lhs, RHS const &rhs) {
  //     lhs.rhs_arg_operator_worker(  rhs,
  //       std::multiplies<typename LHS::value_type>()  );

  //     return lhs;
  //   }

  // };

  namespace dense {

    template< class T,
              std::size_t Rk = 0,
              class Idx = std::size_t,
              std::array<Idx, Rk> Sp
                = std::array<std::size_t, Rk>{}  >
    struct array
    : base<T,Rk,Idx,Sp>
      // ::torment::multiplicable< array<T,Rk,Sp> >,
      // ::torment::multiplicable< array<T,Rk,Sp>, T>
    {
      typedef base<T,Rk,Idx,Sp> base_type;

      using base_type::base_type;

      // template<class Arg, class BinaryOp>
      // void rhs_arg_operator_worker(Arg const& arg, BinaryOp const& binary_op);

      // template<class Arg, class BinaryOp>
      // void lhs_arg_operator_worker(Arg const& arg, BinaryOp const& binary_op);

      // template<
      //   std::size_t _Rk = Rk,
      //   typename = std::enable_if_t<is_multidimensional<_Rk>>  >
      // proxy_values_type& operator[](indices_type const& addr);

      // template<
      //   std::size_t _Rk = Rk,
      //   typename = std::enable_if_t<is_multidimensional<_Rk>>  >
      // const_proxy_values_type& operator[](indices_type const& addr) const;

    };

  };

};
