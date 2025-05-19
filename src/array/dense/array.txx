#pragma once

#include "array.hxx"
#include "base.txx"


namespace torment {

namespace dense {

// template< class T,
//           std::size_t Rk,
//           std::array<std::size_t,Rk> S,
//           std::size_t Sz  >
// template<typename>
// array<T,Rk,S,Sz>::array(shape_type const& shape)
// : base_type(
//     std::reduce(
//       shape.begin(),
//       shape.end(),
//       std::size_t(1),
//       std::multiplies{})),
//   m_shape(shape)
// {

// }

template< class T,
          std::size_t Rk,
          std::array<std::size_t,Rk> S,
          std::size_t Sz  >
typename array<T,Rk,S,Sz>::shape_type
array<T,Rk,S,Sz>::shape() const {
  shape_type dst;

  // if constexpr(is_dynamic<Sz>)
    dst = this->m_shape;
  // else
    // dst = S;

  return dst;
}

}; // namespace dense

}; // namespace torment