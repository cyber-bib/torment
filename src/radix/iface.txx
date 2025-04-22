#pragma once

#include "iface.hxx"

namespace torment {

  namespace radix {

    template<class D>
    typename D& arithmetic_interface<D>::operator++() {
      auto _this = static_cast<D&>(*this);

      return _this++;
    }

      // D& operator--();

      // D& operator++(int);
      // D& operator--(int);
  };
};