#pragma once

#include "std_array.hxx"
#include <stdexcept>

namespace torment {

  namespace dense {

    template<class T, std::size_t Sz>
    constexpr std_array<T, Sz>::std_array(
      std::array<T, Sz> const &_array)
    : std::array<T, Sz>(_array) {}
    template<class T, std::size_t Sz>
    constexpr std_array<T, Sz>::std_array(list_type list) {
      if(list.size() != Sz) throw std::logic_error("wrong initializer list size");
      
      std::copy(list.begin(), list.end(), this->begin());
    }

  };

};