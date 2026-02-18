#pragma once

#include "iterator.hxx"

namespace torment {
  namespace dense {

    template<class Proxy, bool IsConst>
    proxy_iterator<Proxy, IsConst>::proxy_iterator(
      typename proxy_iterator<Proxy, IsConst>::proxy_type *owner,
      std::size_t pos
    ) : m_owner(owner),
        m_pos(pos) {}

    template<class Proxy, bool IsConst>
    void proxy_iterator<Proxy, IsConst>::increment() {
      ++m_pos;
    }

    template<class Proxy, bool IsConst>
    bool proxy_iterator<Proxy, IsConst>::equal(
      proxy_iterator const& other
    ) const {
      return m_owner == other.m_owner && m_pos == other.m_pos;
    }

    template<class Proxy, bool IsConst>
    typename proxy_iterator<Proxy, IsConst>::reference
    proxy_iterator<Proxy, IsConst>::dereference() const {
      if constexpr(is_multidimensional<Proxy::proxy_rank>)
        return (*m_owner)[m_owner->addr_from(m_pos)];
      else
        return (*m_owner)[static_cast<index_type>(m_pos)];
    }

  };
};

