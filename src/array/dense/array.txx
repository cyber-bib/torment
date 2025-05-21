#pragma once

#include "array.hxx"
#include "base.txx"


namespace torment {

namespace dense {

// template< class T,
//           std::size_t Rk,
//           std::array<std::size_t,Rk> S,
//           std::size_t Sz  >
// array<T,Rk,S,Sz>::array()
// : shape_type(shape_t<std::size_t, Rk>()) {}

template< class T,
          std::size_t Rk,
          std::array<std::size_t,Rk> S,
          std::size_t Sz  >
// template<typename>
array<T,Rk,S,Sz>::array(
  shape_array_type const& _shape,
  value_type const& val
) : shape_type(_shape),
    base_type(
      std::reduce(
        _shape.begin(),
        _shape.end(),
        std::size_t(1),
        std::multiplies{}), val)
{
  // this->m_shape = _shape;
}

template< class T,
          std::size_t Rk,
          std::array<std::size_t,Rk> S,
          std::size_t Sz  >
typename array<T,Rk,S,Sz>::shape_array_type
array<T,Rk,S,Sz>::shape() const {
  shape_array_type dst;

  // if constexpr(is_dynamic<Sz>)
    dst = this->m_shape;
  // else
    // dst = S;

  return dst;
}

template<
  class T,
  std::size_t Rk,
  std::array<std::size_t, Rk> Sp,
  std::size_t Sz>
std::ostream&
operator<<(
  std::ostream &os,
  array<T,Rk,Sp,Sz> const &arrg  )
{
  os << "testing out the stream output...";
  return os;
}

}; // namespace dense

}; // namespace torment
