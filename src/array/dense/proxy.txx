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

  IMPL_TMPLT_PARAM_HEADER
  IMPL_TMPLT_CLASS::proxy(typename IMPL_TMPLT_CLASS::_xed_type& data)
    // : base_type(data.shape(), typename base_type::value_type{}),
    : base_type(),
      m_data(data) {}

  IMPL_TMPLT_PARAM_HEADER
  typename IMPL_TMPLT_CLASS::proxy_val_t&
  IMPL_TMPLT_CLASS::operator()(proxy_idx_t const& addr) {
    return this->base_type::operator[](addr);
  }

  IMPL_TMPLT_PARAM_HEADER
  typename IMPL_TMPLT_CLASS::proxy_val_t const&
  IMPL_TMPLT_CLASS::operator()(proxy_idx_t const& addr) const {
    return this->base_type::operator[](addr);
  }

  IMPL_TMPLT_PARAM_HEADER
  typename IMPL_TMPLT_CLASS::value_type&
  IMPL_TMPLT_CLASS::operator[](proxy_idx_t const& addr) {
    return this->m_data[this->operator()(addr)];
  }

  IMPL_TMPLT_PARAM_HEADER
  typename IMPL_TMPLT_CLASS::value_type const&
  IMPL_TMPLT_CLASS::operator[](proxy_idx_t const& addr) const {
    return this->m_data[this->operator()(addr)];
  }

#ifdef _IOSTREAM_ // [

  IMPL_TMPLT_PARAM_HEADER
  std::ostream& operator<<(
    std::ostream &os,
    IMPL_TMPLT_CLASS const &arg  )
  {
    if constexpr(PxRk != 1) {

      typedef IMPL_TMPLT_CLASS arg_type;
      typedef typename arg_type::proxy_idx_t index_type;

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
        auto rank = arg.shape().size();
        if(index < rank) {
          for(std::size_t i = 0; i < arg.shape()[index]; i++) {
            indices[index] = i;

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
            if(i + 1 != arg.shape()[index]) {
              os << ", ";
              os << (index != 0 ? "\n" : "");
            }
          }
        } else {
          os.width(width);
          os << arg[indices];
          os.width(0);
        }
      };

      index_type indices;
      if constexpr(PxRk == 0)
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
};

};

#undef IMPL_TMPLT_PARAM_HEADER
#undef IMPL_TMPLT_CLASS
