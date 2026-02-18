#pragma once

#include "core.hxx"
#include "iterator.txx"
#include "iterable.txx"
#include "core.txx"
#include <stdexcept>
#include <type_traits>


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
  template<class Idx, std::size_t Rk1, std::size_t Rk2>
  constexpr bool is_subset_of (
    std::array<Idx, Rk1> const &arg1,
    std::array<Idx, Rk2> const &arg2
  ) {
    auto N = std::min(arg1.size(), arg2.size());

    bool dst = true;
    for(std::size_t n = 0; n < N; n++) {
      if(!dst) break;
      dst = arg1[n] <= arg2[n];
    }

    return dst;
  }

  template<
    std::size_t Axis,
    class Idx,
    std::size_t Rk,
    std::array<Idx, Rk> Sp>
  constexpr std::array<Idx, Rk - 1> remove_axis_shape() {
    static_assert(Rk > 0, "Cannot remove axis from rank-0 shape.");
    static_assert(Axis < Rk, "Axis out of range.");

    std::array<Idx, Rk - 1> dst{};
    for(std::size_t i = 0, j = 0; i < Rk; i++) {
      if(i == Axis) continue;
      dst[j++] = Sp[i];
    }
    return dst;
  }

  template<class T, std::size_t N>
  constexpr bool in_range(
    std::array<T, N> const& axes,
    T rank
  ) {
    static_assert(std::is_integral_v<T>,
      "Axis type must be integral.");

    for(std::size_t i = 0; i < N; i++) {
      if(!(axes[i] < rank)) return false;
    }
    return true;
  }

  template<class T, std::size_t N>
  constexpr bool unique(std::array<T, N> const& axes) {
    for(std::size_t i = 0; i < N; i++) {
      for(std::size_t j = i + 1; j < N; j++) {
        if(axes[i] == axes[j]) return false;
      }
    }
    return true;
  }

  template<class T, std::size_t N1, std::size_t N2>
  constexpr bool disjoint(
    std::array<T, N1> const& a1,
    std::array<T, N2> const& a2
  ) {
    for(std::size_t i = 0; i < N1; i++) {
      for(std::size_t j = 0; j < N2; j++) {
        if(a1[i] == a2[j]) return false;
      }
    }
    return true;
  }

  template<
    class Idx,
    std::size_t sRk,
    std::size_t pRk   >
  constexpr std::array<Idx, pRk> select_shape(
    std::array<Idx, sRk> const& shape,
    std::array<Idx, pRk> const& proj
  ) {
    static_assert(std::is_integral_v<Idx>, "'T' is not of integral type.");

    std::array<Idx, pRk> dst{};
    for(std::size_t i = 0; i < pRk; i++) {
      dst[i] = shape[static_cast<std::size_t>(proj[i])];
    }

    return dst;
  }

  IMPL_TMPLT_PARAM_HEADER
  IMPL_TMPLT_CLASS::proxy(typename IMPL_TMPLT_CLASS::_xed_type& data)
    // : base_type(data.shape(), typename base_type::value_type{}),
    : base_type(),
      m_data(data) {}

  IMPL_TMPLT_PARAM_HEADER
  void IMPL_TMPLT_CLASS::bind_view(
    std::array<Idx, PxRk> const& free_axes,
    std::array<Idx, Rk - PxRk> const& fixed_axes,
    std::array<Idx, Rk - PxRk> const& fixed_vals
  ) {
    if(!in_range(free_axes, static_cast<Idx>(Rk)))
      throw std::logic_error("Free axis out of range.");
    if(!in_range(fixed_axes, static_cast<Idx>(Rk)))
      throw std::logic_error("Fixed axis out of range.");
    if(!unique(free_axes))
      throw std::logic_error("Free axes must be unique.");
    if(!unique(fixed_axes))
      throw std::logic_error("Fixed axes must be unique.");
    if(!disjoint(free_axes, fixed_axes))
      throw std::logic_error("Free and fixed axes must be disjoint.");

    for(std::size_t id = 0; id < this->size(); id++) {
      proxy_idx_t px{};
      if constexpr(is_multidimensional<PxRk>)
        px = this->addr_from(id);
      else
        px = static_cast<proxy_idx_t>(id);

      index_type full{};
      for(std::size_t axis = 0; axis < Rk; axis++) {
        bool assigned = false;

        for(std::size_t fi = 0; fi < PxRk; fi++) {
          if(free_axes[fi] == static_cast<Idx>(axis)) {
            if constexpr(is_multidimensional<PxRk>)
              full[axis] = px[fi];
            else
              full[axis] = static_cast<Idx>(px);
            assigned = true;
            break;
          }
        }

        if(!assigned) {
          for(std::size_t ki = 0; ki < (Rk - PxRk); ki++) {
            if(fixed_axes[ki] == static_cast<Idx>(axis)) {
              full[axis] = fixed_vals[ki];
              assigned = true;
              break;
            }
          }
        }
      }

      (*this)(px) = full;
    }
  }

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

template<
  class T,
  std::size_t Rk,
  class Idx,
  std::array<Idx, Rk> Sp,
  std::size_t Fr,
  std::array<Idx, Fr> FreeAxes,
  std::size_t NFixed,
  std::array<Idx, NFixed> FixedAxes>
proxy<
  T,
  Rk,
  Idx,
  Sp,
  Fr,
  select_shape(Sp, FreeAxes)>
make_view(base<T, Rk, Idx, Sp> &data, std::array<Idx, NFixed> const& fixed_vals) {
  static_assert(Rk > 0, "make_view requires rank > 0.");
  static_assert(Fr + NFixed == Rk,
    "Free and fixed axis counts must add up to tensor rank.");
  static_assert(in_range(FreeAxes, static_cast<Idx>(Rk)), "Free axis out of range.");
  static_assert(in_range(FixedAxes, static_cast<Idx>(Rk)), "Fixed axis out of range.");
  static_assert(unique(FreeAxes), "Free axes must be unique.");
  static_assert(unique(FixedAxes), "Fixed axes must be unique.");
  static_assert(disjoint(FreeAxes, FixedAxes),
    "Free and fixed axes must be disjoint.");

  using proxy_type = proxy<
    T,
    Rk,
    Idx,
    Sp,
    Fr,
    select_shape(Sp, FreeAxes)>;

  proxy_type dst(data);
  dst.bind_view(FreeAxes, FixedAxes, fixed_vals);
  return dst;
}
};

};

#undef IMPL_TMPLT_PARAM_HEADER
#undef IMPL_TMPLT_CLASS
