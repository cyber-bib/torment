#pragma once

#include <array>

namespace torment {

  namespace dense {

    template<class T, std::size_t Sz>
    struct std_array : std::array<T, Sz> {
      typedef std::initializer_list<T> list_type;

      using std::array<T, Sz>::array;

      constexpr std_array(std::array<T, Sz> const &_array);
      constexpr std_array(list_type list);
    };

  };

};