#pragma once

#include "core.hxx"
#include "smart_container.txx"

#ifndef IMPL_TMPLT_PARAM_HEADER
# define  IMPL_TMPLT_PARAM_HEADER                                               \
            template< class T,                                                  \
                      ::std::size_t Rk,                                         \
                      class Idx,                                                \
                      std::array<Idx,Rk> Sp,                                    \
                      ::std::size_t Sz            >                             \

#else
# error IMPL_TMPLT_PARAM_HEADER is already defined
#endif

#ifndef IMPL_TMPLT_CLASS
# define  IMPL_TMPLT_CLASS base<T,Rk,Idx,Sp,Sz>
#else
# error IMPL_TMPLT_CLASS_NAMESPACE is already defined
#endif

namespace torment {
namespace dense {

// template< class T,
//           std::size_t Rk,
//           std::array<std::size_t,Rk> S,
//           std::size_t Sz  >
// array<T,Rk,S,Sz>::array()
// : shape_type(shape_t<std::size_t, Rk>()) {}

IMPL_TMPLT_PARAM_HEADER
template<std::size_t _Sz, typename>
IMPL_TMPLT_CLASS::base (
  index_type const& _shape,
  value_type const& val
) : shape_type(_shape),
    base_type([&]{
      if constexpr(is_multidimensional<Rk>)
        return zredmult(_shape);
      else
        return static_cast<std::size_t>(_shape);
    }(), val)
{
  // this->m_shape = _shape;
}
IMPL_TMPLT_PARAM_HEADER
IMPL_TMPLT_CLASS::base (
  list_type list)
: base_type(list) {
  if constexpr(Rk == 0) {
    static_assert( Rk != 0,
                    "initializer list constructor for fully dynamic "
                    "multidimentional array-like objects is ill-formed.");
  } else if constexpr(Rk == 1) {
    if constexpr(Sz == 0) {
      this->m_shape = list.size();
    }
  } else if constexpr(Rk > 1) {
    static_assert( Sz != 0,
                    "initializer list constructor for partially dynamic "
                    "multidimentional array-like objects is ill-formed.");
  }
};
IMPL_TMPLT_PARAM_HEADER
IMPL_TMPLT_CLASS&
IMPL_TMPLT_CLASS::operator= (
  list_type list
) {
  if constexpr(Sz == 0) {
    if constexpr(Rk == 1)
      this->m_shape = list.size();
    else {
      if(list.size() != this->size())
        throw std::logic_error( "initializer list size does not match dynamic "
                                "shape size." );
    }
  } else {
    if(list.size() != Sz)
      throw std::logic_error( "initializer list size does not match static "
                              "shape size." );
  }
  this->base_type::operator=(list);

  return *this;
}

IMPL_TMPLT_PARAM_HEADER
IMPL_TMPLT_CLASS&
IMPL_TMPLT_CLASS::assign (
  index_type const& _shape,
  value_type const& val
) {
  this->m_shape = _shape;

  std::size_t n;

  if constexpr(is_multidimensional<Rk>)
    n = zredmult(_shape);
  else
    n = static_cast<std::size_t>(_shape);

  this->base_type::assign(n, val);

  return *this;
}
IMPL_TMPLT_PARAM_HEADER
void IMPL_TMPLT_CLASS::resize (
  index_type const& _shape,
  value_type const& val
) {
  static_assert( Rk == 1,
                  "resize of multidimensional array-like objects is "
                  "ill-formed" );

  this->base_type::resize(static_cast<std::size_t>(_shape), val);
}

IMPL_TMPLT_PARAM_HEADER
typename IMPL_TMPLT_CLASS::index_type
IMPL_TMPLT_CLASS::shape() const {
  index_type dst;

  // if constexpr(is_dynamic<Sz>)
    dst = this->m_shape;
  // else
    // dst = S;

  return dst;
}

IMPL_TMPLT_PARAM_HEADER
template< ::std::size_t _Rk,
          typename         >
typename IMPL_TMPLT_CLASS::value_type &
IMPL_TMPLT_CLASS::operator[](
  index_type const& addr
) {
  static_assert(Rk == _Rk,
                "_Rk is not meant to be changed manually.");

  value_type &dst = this->at(
    this->id_from(addr)
  );

  return dst;
}

IMPL_TMPLT_PARAM_HEADER
template< ::std::size_t _Rk,
          typename         >
typename IMPL_TMPLT_CLASS::value_type const &
IMPL_TMPLT_CLASS::operator[](
  index_type const& addr
) const {
  static_assert(Rk == _Rk,
                "_Rk is not meant to be changed manually.");

  value_type const &dst = this->at(
    this->id_from(addr)
  );

  return dst;
}

IMPL_TMPLT_PARAM_HEADER
template< ::std::size_t _Rk,
          typename         >
typename IMPL_TMPLT_CLASS::index_type
IMPL_TMPLT_CLASS::strides() const {
  static_assert(Rk == _Rk,
                "_Rk is not meant to be changed manually.");

  index_type dst;

  if constexpr(is_dynamic<Rk>) {
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

IMPL_TMPLT_PARAM_HEADER
template< ::std::size_t _Rk,
          typename         >
std::size_t
IMPL_TMPLT_CLASS::id_from(
  index_type const& addr
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

IMPL_TMPLT_PARAM_HEADER
template< ::std::size_t _Rk,
          typename         >
typename IMPL_TMPLT_CLASS::index_type
IMPL_TMPLT_CLASS::addr_from(
  std::size_t const& id
) const {
  static_assert(Rk == _Rk,
                "_Rk is not meant to be changed manually.");

  index_type addr;

  if constexpr(is_dynamic<Rk>) {
    addr.resize(this->m_shape.size());
  }

  return addr;
}


#ifdef _IOSTREAM_ // [

IMPL_TMPLT_PARAM_HEADER
std::ostream& operator<<(
  std::ostream &os,
  IMPL_TMPLT_CLASS const &arg  )
{

  if constexpr(Rk != 1) {

    typedef IMPL_TMPLT_CLASS arg_type;
    typedef typename arg_type::index_type index_type;

    typedef void (*unwrap_type)(
      arg_type const &,
      index_type &,
      std::size_t,
      std::ostream &,
      std::size_t const &,
      void*  );

    unwrap_type unwrap;
    std::size_t w = os.width();
    os.width(0);

    unwrap = [](
      arg_type const &arg,
      index_type &indices,
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

    index_type indices;
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

#endif // ] _IOSTREAM_

}; // namespace dense

}; // namespace torment

#undef IMPL_TMPLT_PARAM_HEADER
#undef IMPL_TMPLT_CLASS