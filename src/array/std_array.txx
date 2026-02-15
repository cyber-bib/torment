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
    constexpr std_array<T, Sz>::std_array(
      std::array<T, Sz> &&_array)
    : std::array<T, Sz>(std::move(_array)) {}

    template<class T, std::size_t Sz>
    constexpr std_array<T, Sz>::std_array(list_type list) {
      if(list.size() != Sz) throw std::logic_error("wrong initializer list size");

      std::copy(list.begin(), list.end(), this->begin());
    }


  };

};

// namespace torment {

//   namespace dense {

//     template<class T, std::size_t Sz>
//     constexpr std_array<T, Sz>::std_array(
//       std::array<T, Sz> const &_array)
//     : std::array<T, Sz>(_array) {}

//     template<class T, std::size_t Sz>
//     constexpr std_array<T, Sz>::std_array(
//       std::array<T, Sz> &&_array)
//     : std::array<T, Sz>(std::move(_array)) {}

//     template<class T, std::size_t Sz>
//     template<class... U>
//     requires (sizeof...(U) == Sz) && (std::convertible_to<U, T> && ...)
//     constexpr std_array<T, Sz>::std_array(U&&... u)
//       : base_type{ static_cast<T>(std::forward<U>(u))... } {}


//     // template<class T, std::size_t Sz>
//     // constexpr std_array<T, Sz>::std_array(list_type list) {

//     //   static_assert(Sz == list.size(), "wrong initializer list size");

//     //   // if(list.size() != Sz) throw std::logic_error("wrong initializer list size");

//     //   std::copy(list.begin(), list.end(), this->begin());
//     // }


//   };

// };