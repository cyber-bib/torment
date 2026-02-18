#pragma once

#include "iterator.hxx"

namespace torment {
  namespace dense {

    template<class Derived>
    struct proxy_iterable {
      using iterator = proxy_iterator<Derived, false>;
      using const_iterator = proxy_iterator<Derived, true>;

      iterator begin();
      iterator end();
      const_iterator begin() const;
      const_iterator end() const;
      const_iterator cbegin() const;
      const_iterator cend() const;
    };

  };
};
