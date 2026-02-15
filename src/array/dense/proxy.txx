#pragma once

#include "proxy.hxx"
#include "core.txx"


#ifndef IMPL_TMPLT_PARAM_HEADER
# define  IMPL_TMPLT_PARAM_HEADER                                               \
            template< class T,                                                  \
              std::size_t Rk,                                                   \
              class Idx,                                                        \
              std::array<Idx, Rk> Sp,                                           \
              std::size_t PxRk,                                                 \
              std::array<Idx, PxRk> PxSp  >                                     \

#else
# error IMPL_TMPLT_PARAM_HEADER is already defined
#endif



#ifndef IMPL_TMPLT_CLASS
# define  IMPL_TMPLT_CLASS proxy<T,Rk,Idx,Sp,PxRk,PxSp>
#else
# error IMPL_TMPLT_CLASS_NAMESPACE is already defined
#endif

namespace torment {

namespace dense {

  // IMPL_TMPLT_PARAM_HEADER
  // IMPL_TMPLT_CLASS::proxy(
  //   typename IMPL_TMPLT_CLASS::_xed_type &data
  // ) : m_data(data) {}

  IMPL_TMPLT_PARAM_HEADER
  IMPL_TMPLT_CLASS::proxy(typename IMPL_TMPLT_CLASS::_xed_type& data)
    : base_type(data.shape(), typename base_type::value_type{}),
      m_data(data) {}

  // template<
  // std::size_t _Rk = Rk,
  // typename = std::enable_if_t<is_multidimensional<_Rk>>  >
  // value_type& operator[](index_type const& addr);

  // template<
  // std::size_t _Rk = Rk,
  // typename = std::enable_if_t<is_multidimensional<_Rk>>  >
  // value_type const& operator[](index_type const& addr) const;
};

};

#undef IMPL_TMPLT_PARAM_HEADER
#undef IMPL_TMPLT_CLASS