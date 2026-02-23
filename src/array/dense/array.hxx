#pragma once

#include "core.hxx"
#include "proxy/core.hxx"
#include "boost/operators.hpp"

namespace torment {
  namespace dense {

    template< class T,
              std::size_t Rk = 0,
              class Idx = std::size_t,
              std::array<Idx, Rk> Sp
                = std::array<std::size_t, Rk>{}  >
    struct array
    : base<T,Rk,Idx,Sp>,
      boost::ordered_euclidean_ring_operators<array<T,Rk,Idx,Sp>>,
      boost::ordered_euclidean_ring_operators<array<T,Rk,Idx,Sp>, T>
    {
      typedef base<T,Rk,Idx,Sp> base_type;
      typedef typename base_type::value_type value_type;

      using base_type::base_type;
      using base_type::operator=;

      array& operator=(value_type const& rhs);

      array& operator+=(array const& rhs);
      array& operator-=(array const& rhs);
      array& operator*=(array const& rhs);
      array& operator/=(array const& rhs);
      array& operator%=(array const& rhs);

      array& operator+=(value_type const& rhs);
      array& operator-=(value_type const& rhs);
      array& operator*=(value_type const& rhs);
      array& operator/=(value_type const& rhs);
      array& operator%=(value_type const& rhs);

      bool operator==(array const& rhs) const;
      bool operator<(array const& rhs) const;

      bool operator==(value_type const& rhs) const;
      bool operator<(value_type const& rhs) const;

    };

  };

};
