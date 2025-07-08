#pragma once

#include "smart_container.hxx"
#include "array/std_array.txx"

namespace torment {
  namespace dense {

    #ifdef _IOSTREAM_ // {

    template<class T, std::size_t Sz>
    std::ostream& operator<<(std::ostream &os, smart_container<T, Sz> const &arr) {

      os << "[ ";
      if(arr.size() != 0) os << arr[0];

      for(std::size_t i = 1; i < arr.size(); i++)
        os << ", " << arr[i];

      os << " ]";

      return os;
    }

    #endif // } _IOSTREAM_
  };
};
