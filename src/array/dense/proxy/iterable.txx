#pragma once

#include "iterable.hxx"

namespace torment {
  namespace dense {

    template<class Derived>
    typename proxy_iterable<Derived>::iterator
    proxy_iterable<Derived>::begin() {
      return iterator(
        static_cast<Derived*>(this), 0);
    }

    template<class Derived>
    typename proxy_iterable<Derived>::iterator
    proxy_iterable<Derived>::end() {
      auto self = static_cast<Derived*>(this);
      return iterator(self, self->size());
    }

    template<class Derived>
    typename proxy_iterable<Derived>::const_iterator
    proxy_iterable<Derived>::begin() const {
      return cbegin();
    }

    template<class Derived>
    typename proxy_iterable<Derived>::const_iterator
    proxy_iterable<Derived>::end() const {
      return cend();
    }

    template<class Derived>
    typename proxy_iterable<Derived>::const_iterator
    proxy_iterable<Derived>::cbegin() const {
      return const_iterator(
        static_cast<Derived const*>(this), 0);
    }

    template<class Derived>
    typename proxy_iterable<Derived>::const_iterator
    proxy_iterable<Derived>::cend() const {
      auto self = static_cast<Derived const*>(this);
      return const_iterator(self, self->size());
    }

  };
};
