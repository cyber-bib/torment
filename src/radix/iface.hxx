#pragma once

#include <type_traits>

namespace torment {
  namespace radix {
    template<class D>
    struct arithmetic_interface {
      D& operator++();
      // D& operator--();

      // D& operator++(int);
      // D& operator--(int);
    };
  };
};