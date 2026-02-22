#pragma once

#include "array.hxx"

#include "core.txx"
#include "strided_view.txx"
#include "proxy/core.txx"

#include <algorithm>
#include <execution>

namespace torment {

  namespace dense {

    // template< class T,
    //           std::size_t Rk,
    //           std::array<std::size_t, Rk> Sp  >
    // template< class Arg,
    //           class BinaryOp  >
    // void array<T,Rk,Sp>::rhs_arg_operator_worker(Arg const& arg, BinaryOp const& binary_op) {

    //   if constexpr(std::is_same_v<array<T,Rk,Sp>, Arg>) {
    //     std::transform( std::execution::seq,
    //                     this->begin(),
    //                     this->end(),
    //                     arg.begin(),
    //                     this->begin(),
    //                     binary_op
    //     );
    //   } else if constexpr(std::is_same_v<T, Arg>) {
    //     std::for_each(std::execution::seq,
    //                   this->begin(),
    //                   this->end(),
    //                   [&](auto &x) {
    //                   x = binary_op(x, arg);
    //     });
    //   } else {
    //     static_assert(false,
    //                   "TODO: additional functionality needs to be "
    //                   "implemented."  );
    //   }
    // }

  };

};
