#pragma once

#include <array>

namespace torment {
  namespace dense {
    template<class T, std::size_t Sz>
    struct std_array : std::array<T, Sz> {
      typedef std::initializer_list<T> list_type;

      using std::array<T, Sz>::array;
      std_array(list_type list);
    };

    template<class T, std::size_t Sz>
    std_array<T, Sz>::std_array(list_type list) {
      if(list.size() != Sz) throw std::logic_error("wrong initializer list size");

      std::copy(list.begin(), list.end(), this->begin());
    }
  };
};