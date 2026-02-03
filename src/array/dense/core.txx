#pragma once

#include <iostream>
#include "core.hxx"
#include "smart_container.txx"


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
base<T,Rk,S,Sz>::base(
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
typename base<T,Rk,S,Sz>::shape_array_type
base<T,Rk,S,Sz>::shape() const {
  shape_array_type dst;

  // if constexpr(is_dynamic<Sz>)
    dst = this->m_shape;
  // else
    // dst = S;

  return dst;
}

template< class                        T,
          std::size_t                 Rk,
          std::array<std::size_t,Rk>   S,
          std::size_t                 Sz  >
template< std::size_t                _Rk,
          typename                        >
typename base<T,Rk,S,Sz>::value_type &
base<T,Rk,S,Sz>::operator[](
  shape_array_type const& addr
) {
  static_assert(Rk == _Rk,
                "_Rk is not meant to be changed manually.");

  value_type &dst = this->at(
    this->id_from(addr)
  );

  return dst;
}

template< class                        T,
          std::size_t                 Rk,
          std::array<std::size_t,Rk>   S,
          std::size_t                 Sz  >
template< std::size_t                _Rk,
          typename                        >
typename base<T,Rk,S,Sz>::value_type const &
base<T,Rk,S,Sz>::operator[](
  shape_array_type const& addr
) const {
  static_assert(Rk == _Rk,
                "_Rk is not meant to be changed manually.");

  value_type const &dst = this->at(
    this->id_from(addr)
  );

  return dst;
}

template< class                         T,
          std::size_t                  Rk,
          std::array<std::size_t,Rk>    S,
          std::size_t                  Sz  >
template< std::size_t                 _Rk,
          typename                         >
typename base<T,Rk,S,Sz>::shape_array_type
base<T,Rk,S,Sz>::strides() const {
  static_assert(Rk == _Rk,
                "_Rk is not meant to be changed manually.");

  shape_array_type dst;

  if constexpr(is_dynamic<Sz>) {
    dst.resize(this->m_shape.size());
  }


  for(  std::size_t cnt = 0, stride = 1;
        auto const &e : this->m_shape
  ) {
    dst[cnt++] = stride;
    stride *= e;
  };

  return dst;
}

template< class                         T,
          std::size_t                  Rk,
          std::array<std::size_t,Rk>    S,
          std::size_t                  Sz  >
template< std::size_t _Rk,
          typename                         >
std::size_t
base<T,Rk,S,Sz>::id_from(
  shape_array_type const& addr
) const {
  static_assert(Rk == _Rk,
                "_Rk is not meant to be changed manually.");

  std::size_t id = 0;
  auto tmp = this->strides();

  for(  std::size_t cnt = 0;
        auto const & e: tmp
  ) {
    id += addr[cnt++]*e;
  };

  return id;
}

template< class                         T,
          std::size_t                  Rk,
          std::array<std::size_t,Rk>    S,
          std::size_t                  Sz  >
template< std::size_t _Rk,
          typename                         >
typename base<T,Rk,S,Sz>::shape_array_type
base<T,Rk,S,Sz>::addr_from(
  std::size_t const& id
) const {
  static_assert(Rk == _Rk,
                "_Rk is not meant to be changed manually.");
  
  shape_array_type addr;

  if constexpr(is_dynamic<Sz>) {
    addr.resize(this->m_shape.size());
  }
  
  return addr;
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
  base<T,Rk,Sp,Sz> const &arg  )
{

  if constexpr(Rk != 1) {

    typedef base<T,Rk,Sp,Sz> arg_type;
    typedef typename arg_type::shape_array_type indices_type;

    typedef void (*unwrap_type)(
      arg_type const &,
      indices_type &,
      std::size_t,
      std::ostream &,
      std::size_t const &,
      void*  );

    unwrap_type unwrap;
    std::size_t w = os.width();
    os.width(0);

    unwrap = [](
      arg_type const &arg,
      indices_type &indices,
      std::size_t index,
      std::ostream &os,
      std::size_t const &width,
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
            unwrap(arg, indices, index - 1, os, width, self);
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
        os.width(width);
        os << arg[counter++];
        os.width(0);
        // os << indices[index];
      }
    };

    indices_type indices;
    if constexpr(Rk == 0)
      indices.assign(arg.shape().size(), 0);
    else
      indices.fill(0);

    os << "[";
    unwrap(arg, indices, arg.shape().size() - 1, os, w,
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
