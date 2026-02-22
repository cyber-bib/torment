#pragma once

#include "core.hxx"
#include "iterator.txx"
#include "iterable.txx"
#include "core.txx"

#include <stdexcept>
#include <type_traits>
#include <algorithm>
#include <optional>


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
    std::array<T, N> const& dims,
    T rank
  ) {
    static_assert(  std::is_integral_v<T>,
                    "'T' type must be integral."  );

    return std::all_of( dims.begin(),
                        dims.end(),
                        [rank](T el) { return el < rank; }  );

  }
  template<class T, std::size_t N>
  constexpr bool in_range(
    std::array<std::optional<T>, N> const& dims,
    T rank
  ) {
    static_assert(  std::is_integral_v<T>,
                    "'T' type must be integral."  );

    return std::all_of( dims.begin(),
                        dims.end(),
                        [rank](std::optional<T> el) { return el.value_or(0) < rank; }  );
  }

  template<class T, std::size_t N>
  constexpr bool is_sorted_set(std::array<T, N> const& data) {
    if constexpr(N == 0 || N == 1) {
      return true;
    }

    if( !std::is_sorted(  data.begin(),
                          data.end())
    ) return false;

    if( auto it = std::adjacent_find( data.begin(),
                                      data.end()    );
        it != data.end()
    ) return false;


    return true;
  }

  template<class T, std::size_t N1, std::size_t N2>
  constexpr bool are_disjoint_sets (
    std::array<T, N1> const& lhs,
    std::array<T, N2> const& rhs
  ) {
    if constexpr(N1 == 0 || N2 == 0) {
      return true;
    }

    if(!is_sorted_set(lhs))
      return false;

    if(!is_sorted_set(rhs))
      return false;


    auto min = std::min(lhs.back(), rhs.back());
    auto max = std::max(lhs.front(), rhs.front());

    if(min >= max)
      return false;

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
  IMPL_TMPLT_CLASS& IMPL_TMPLT_CLASS::bind_view (
    std::array<std::optional<Idx>, Rk> const& view
  ) {
    if constexpr(Rk == 0) {
      static_assert(false, "logic has not been implemented.");
      return;
    } else if constexpr(Rk == 1) {
      static_assert(PxRk == 1, "For Rk == 1, PxRk must be 1.");

      // this seems silly
      static_assert(false, "logic has not been implemented.");
    } else {
      for(std::size_t i = 0; auto const &elem : view)
        if(!(elem.value_or(0) < Sp[i++]))
          throw std::logic_error(
                  "'view' fixed value is out of shape bounds." );

      auto& ref = static_cast<base_type&>(*this);
      for(std::size_t i = 0; auto &ithel : ref) {
        auto iddr = ref.addr_from(i++);

        for(std::size_t j = 0, k = 0; auto &jthel : view) {
          ithel[j++] = jthel.value_or(iddr[k]);
          k += !jthel.has_value() ? 1 : 0;
        }
      }

    }

    return *this;
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
  static_assert(is_sorted_set(FreeAxes), "Free axes must be a sorted set.");
  static_assert(is_sorted_set(FixedAxes), "Fixed axes must be sorted set.");
  static_assert(are_disjoint_sets(FreeAxes, FixedAxes),
    "Free and fixed axes must be disjoint.");

  using proxy_type = proxy<
    T,
    Rk,
    Idx,
    Sp,
    Fr,
    select_shape(Sp, FreeAxes)>;

  proxy_type dst(data);
  std::array<std::optional<Idx>, Rk> view{};
  view.fill(std::nullopt);
  for(std::size_t i = 0; i < NFixed; i++) {
    view[static_cast<std::size_t>(FixedAxes[i])] = fixed_vals[i];
  }

  dst.bind_view(view);
  return dst;
}
};

};

#undef IMPL_TMPLT_PARAM_HEADER
#undef IMPL_TMPLT_CLASS
