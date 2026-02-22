#pragma once

#include "core.hxx"
#include "iterator.txx"
#include "iterable.txx"

#include <stdexcept>
#include <type_traits>
#include <algorithm>
#include <optional>

#ifndef IMPL_TMPLT_PARAM_HEADER
# define IMPL_TMPLT_PARAM_HEADER                                                 \
  template< class T,                                                             \
            std::size_t Rk,                                                      \
            class Idx,                                                           \
            std::array<Idx, Rk> Sp >
#else
# error IMPL_TMPLT_PARAM_HEADER is already defined
#endif

#ifndef IMPL_TMPLT_CLASS
# define IMPL_TMPLT_CLASS proxy<T, Rk, Idx, Sp>
#else
# error IMPL_TMPLT_CLASS_NAMESPACE is already defined
#endif

namespace torment {
namespace dense {

template<class Idx, std::size_t Rk1, std::size_t Rk2>
constexpr bool is_subset_of(
  std::array<Idx, Rk1> const& arg1,
  std::array<Idx, Rk2> const& arg2
) {
  auto n = std::min(arg1.size(), arg2.size());

  bool dst = true;
  for(std::size_t i = 0; i < n; i++) {
    if(!dst) break;
    dst = arg1[i] <= arg2[i];
  }

  return dst;
}

template<std::size_t Axis, class Idx, std::size_t Rk, std::array<Idx, Rk> Sp>
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
constexpr bool in_range(std::array<T, N> const& dims, T rank) {
  static_assert(std::is_integral_v<T>, "'T' type must be integral.");
  return std::all_of(
    dims.begin(),
    dims.end(),
    [rank](T el) { return el < rank; }
  );
}

template<class T, std::size_t N>
constexpr bool in_range(std::array<std::optional<T>, N> const& dims, T rank) {
  static_assert(std::is_integral_v<T>, "'T' type must be integral.");
  return std::all_of(
    dims.begin(),
    dims.end(),
    [rank](std::optional<T> el) { return !el.has_value() || el.value() < rank; }
  );
}

template<class T, std::size_t N>
constexpr bool is_sorted_set(std::array<T, N> const& data) {
  if constexpr(N == 0 || N == 1) {
    return true;
  }

  if(!std::is_sorted(data.begin(), data.end()))
    return false;

  auto it = std::adjacent_find(data.begin(), data.end());
  if(it != data.end())
    return false;

  return true;
}

template<class T, std::size_t N1, std::size_t N2>
constexpr bool are_disjoint_sets(
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

template<class Idx, std::size_t sRk, std::size_t pRk>
constexpr std::array<Idx, pRk> select_shape(
  std::array<Idx, sRk> const& shape,
  std::array<Idx, pRk> const& proj
) {
  static_assert(std::is_integral_v<Idx>, "'Idx' is not of integral type.");
  std::array<Idx, pRk> dst{};
  for(std::size_t i = 0; i < pRk; i++) {
    dst[i] = shape[static_cast<std::size_t>(proj[i])];
  }
  return dst;
}

IMPL_TMPLT_PARAM_HEADER
IMPL_TMPLT_CLASS::proxy(typename IMPL_TMPLT_CLASS::_xed_type& data)
  : m_data(data),
    m_view{}
{
  if constexpr(is_dynamic<Rk>) {
    m_view.assign(this->m_data.shape().size(), std::nullopt);
  } else {
    m_view.fill(std::nullopt);
  }
}

IMPL_TMPLT_PARAM_HEADER
IMPL_TMPLT_CLASS::proxy(
  typename IMPL_TMPLT_CLASS::_xed_type& data,
  typename IMPL_TMPLT_CLASS::view_type const& view
) : m_data(data),
    m_view(view)
{
  this->bind_view(m_view);
}

IMPL_TMPLT_PARAM_HEADER
IMPL_TMPLT_CLASS& IMPL_TMPLT_CLASS::bind_view(
  view_type const& view
) {
  if constexpr(is_dynamic<Rk>) {
    auto src_shape = this->m_data.shape();
    auto rank = src_shape.size();
    if(view.size() != rank) {
      throw std::logic_error("view rank does not match data rank.");
    }

    for(std::size_t axis = 0; axis < rank; axis++) {
      if(view[axis].has_value()) {
        auto fixed = static_cast<std::size_t>(view[axis].value());
        if(fixed >= static_cast<std::size_t>(src_shape[axis])) {
          throw std::logic_error("'view' fixed value is out of shape bounds.");
        }
      }
    }
  } else {
    for(std::size_t axis = 0; axis < Rk; axis++) {
      if(view[axis].has_value()) {
        auto fixed = static_cast<std::size_t>(view[axis].value());
        if(fixed >= static_cast<std::size_t>(Sp[axis])) {
          throw std::logic_error("'view' fixed value is out of shape bounds.");
        }
      }
    }
  }

  this->m_view = view;

  return *this;
}

IMPL_TMPLT_PARAM_HEADER
typename IMPL_TMPLT_CLASS::proxy_val_t
IMPL_TMPLT_CLASS::operator()(proxy_idx_t const& addr) const {
  auto shp = this->shape();
  if(addr.size() != shp.size()) {
    throw std::logic_error("proxy address rank mismatch.");
  }

  proxy_val_t dst{};
  if constexpr(is_dynamic<Rk>) {
    dst.resize(this->m_data.shape().size());
    auto src_shape = this->m_data.shape();
    auto rank = src_shape.size();

    for(std::size_t axis = 0, free_idx = 0; axis < rank; axis++) {
      if(this->m_view[axis].has_value()) {
        dst[axis] = this->m_view[axis].value();
      } else {
        auto val = static_cast<std::size_t>(addr[free_idx]);
        auto lim = static_cast<std::size_t>(src_shape[axis]);
        if(val >= lim) {
          throw std::logic_error("proxy address is out of shape bounds.");
        }
        dst[axis] = addr[free_idx++];
      }
    }
  } else {
    for(std::size_t axis = 0, free_idx = 0; axis < Rk; axis++) {
      if(this->m_view[axis].has_value()) {
        dst[axis] = this->m_view[axis].value();
      } else {
        auto val = static_cast<std::size_t>(addr[free_idx]);
        auto lim = static_cast<std::size_t>(Sp[axis]);
        if(val >= lim) {
          throw std::logic_error("proxy address is out of shape bounds.");
        }
        dst[axis] = addr[free_idx++];
      }
    }
  }

  return dst;
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

IMPL_TMPLT_PARAM_HEADER
typename IMPL_TMPLT_CLASS::proxy_idx_t
IMPL_TMPLT_CLASS::shape() const {
  proxy_idx_t dst{};
  if constexpr(is_dynamic<Rk>) {
    auto src_shape = this->m_data.shape();
    auto rank = src_shape.size();
    for(std::size_t axis = 0; axis < rank; axis++) {
      if(!this->m_view[axis].has_value()) {
        dst.push_back(src_shape[axis]);
      }
    }
  } else {
    for(std::size_t axis = 0; axis < Rk; axis++) {
      if(!this->m_view[axis].has_value()) {
        dst.push_back(Sp[axis]);
      }
    }
  }
  if(dst.empty()) {
    dst.push_back(static_cast<Idx>(1));
  }
  return dst;
}

IMPL_TMPLT_PARAM_HEADER
std::size_t IMPL_TMPLT_CLASS::size() const {
  return zredmult(this->shape());
}

IMPL_TMPLT_PARAM_HEADER
typename IMPL_TMPLT_CLASS::proxy_idx_t
IMPL_TMPLT_CLASS::addr_from(std::size_t const& id) const {
  auto shp = this->shape();
  proxy_idx_t addr(shp.size(), 0);
  proxy_idx_t stride(shp.size(), 0);

  std::size_t s = 1;
  for(std::size_t i = 0; i < shp.size(); i++) {
    stride[i] = static_cast<Idx>(s);
    s *= static_cast<std::size_t>(shp[i]);
  }

  std::size_t rem = id;
  for(std::size_t i = shp.size(); i-- > 0;) {
    auto st = static_cast<std::size_t>(stride[i]);
    auto ex = static_cast<std::size_t>(shp[i]);
    if(st == 0 || ex == 0) {
      addr[i] = 0;
      continue;
    }
    addr[i] = static_cast<Idx>((rem / st) % ex);
    rem %= st;
  }

  return addr;
}

#ifdef _IOSTREAM_ // [

IMPL_TMPLT_PARAM_HEADER
std::ostream& operator<<(
  std::ostream& os,
  IMPL_TMPLT_CLASS const& arg
) {
  if(arg.shape().size() != 1) {
    typedef IMPL_TMPLT_CLASS arg_type;
    typedef typename arg_type::proxy_idx_t index_type;

    typedef void (*unwrap_type)(
      arg_type const&,
      index_type&,
      std::size_t,
      std::ostream&,
      std::size_t const&,
      void*);

    unwrap_type unwrap;
    std::size_t w = os.width();
    os.width(0);

    unwrap = [](
      arg_type const& arr,
      index_type& indices,
      std::size_t index,
      std::ostream& out,
      std::size_t const& width,
      void* self = nullptr
    ) {
      auto rank = arr.shape().size();
      if(index < rank) {
        for(std::size_t i = 0; i < arr.shape()[index]; i++) {
          indices[index] = static_cast<Idx>(i);

          if(i != 0 && index != 0) {
            auto k = rank - index;
            for(std::size_t j = 0; j < k; j++) out << " ";
          }
          out << (index == 0 ? "" : "[");
          if(self) {
            auto fn = reinterpret_cast<unwrap_type>(self);
            fn(arr, indices, index - 1, out, width, self);
          }
          out << (index == 0 ? "" : "]");

          if(i + 1 != arr.shape()[index]) {
            out << ", ";
            out << (index != 0 ? "\n" : "");
          }
        }
      } else {
        out.width(width);
        out << arr[indices];
        out.width(0);
      }
    };

    index_type indices;
    indices.assign(arg.shape().size(), 0);

    os << "[";
    unwrap(arg, indices, arg.shape().size() - 1, os, w,
      reinterpret_cast<void*>(unwrap));
    os << "]";
  } else {
    os << "[";
    if(arg.size() != 0) {
      typename IMPL_TMPLT_CLASS::proxy_idx_t idx(1, 0);
      os << arg[idx];
      for(std::size_t i = 1; i < arg.size(); i++) {
        idx[0] = static_cast<Idx>(i);
        os << ", " << arg[idx];
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
