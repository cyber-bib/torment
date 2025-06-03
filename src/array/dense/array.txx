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


#ifdef _IOSTREAM_ // {

template<
  class T,
  std::size_t Rk,
  std::array<std::size_t, Rk> Sp,
  std::size_t Sz>
std::ostream&
operator<<(
  std::ostream &os,
  array<T,Rk,Sp,Sz> const &arg  )
{

  if constexpr(Rk != 1) {

    typedef array<T,Rk,Sp,Sz> arg_type;
    typedef typename arg_type::shape_array_type indices_type;

    typedef void (*unwrap_type)(
      arg_type const &,
      indices_type &,
      std::size_t,
      std::ostream &,
      void*  );

    unwrap_type unwrap;

    unwrap = [](
      arg_type const &arg,
      indices_type &indices,
      std::size_t index,
      std::ostream &os,
      void* self = nullptr
    ) {
      static std::size_t counter = 0;
      counter = (index == arg.shape().size()-1 ? 0 : counter);

      auto rank = arg.shape().size();
      if(index < rank) {
        // os << "[";
        // std::cout << arg.shape();
        for(std::size_t i = 0; i < arg.shape()[index]; i++) {
          indices[index] = i;
          // os << (i != 0 ? " " : "");

          if(i != 0 && index != 0) {
            auto k = arg.shape().size() - index;
            for(std::size_t j = 0; j < k; j++) os << " ";
          }
          os << (index == 0 ? "" : "[");
          if(self) {
            auto unwrap = reinterpret_cast<unwrap_type>(self);
            unwrap(arg, indices, index - 1, os, self);
          }
          os << (index == 0 ? "" : "]");
          // os << ((i + 1) == arg.shape()[index] ? "" :
          //         (index == 0 ? "" : ":"));
          if(i + 1 != arg.shape()[index]) {
            os << ", ";
            os << (index != 0 ? "\n" : "");
            //for(std::size_t j = 0; j < index; j++) os << "\n";
          }
        }
        // os << "]\n";
      } else {
        // auto rank = arg.shape().size();
        // auto &s = arg.shape();
        // auto &is = indices;
        // auto arg_index = 1ULL;
        // for(auto i = 0ULL; i < rank; i++) {
        //   auto term = 1ULL;
        //   for(auto j = 0ULL; j < (i+1 < rank ? s[i+1] : 0); j++) term *= s[j]
        //   term = is[i]*s[]
        //   arg_index += term;
        // }
        os << arg[counter++];
        // os << indices[index];
      }
    };

    indices_type indices;
    if constexpr(Rk == 0)
      indices.assign(arg.shape().size(), 0);
    else
      indices.fill(0);

    os << "[";
    unwrap(arg, indices, arg.shape().size() - 1, os,
      reinterpret_cast<void*>(unwrap));
    os << "]";

  } else {

    os << "[";
    if(arg.size() != 0) {
      os << arg[0];
      for(std::size_t i = 1; i < arg.size(); i++) {
        os << ", " << arg[i];
      }
    }
    os << "]";

  }


  return os;
}

#endif // } _IOSTREAM_

}; // namespace dense

}; // namespace torment
