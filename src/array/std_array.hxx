#pragma once

#include <array>
#include <concepts>
#include <utility>

namespace torment {

  namespace dense {

    template<class T, std::size_t Sz>
    struct std_array : std::array<T, Sz> {
      typedef std::array<T, Sz> base_type;
      typedef std::initializer_list<T> list_type;

      using base_type::base_type;
      // using base_type::operator=;

      constexpr std_array(std::array<T, Sz> const &_array);
      constexpr std_array(std::array<T, Sz> &&_array);

      constexpr std_array(list_type list);
    };

  };

};

// namespace torment {

//   namespace dense {

//     template<class T, std::size_t Sz>
//     struct std_array : std::array<T, Sz> {
//       typedef std::array<T, Sz> base_type;
//       // typedef std::initializer_list<T> list_type;

//       // using base_type::base_type;
//       // using base_type::operator=;
//       constexpr std_array() = default;

//       constexpr std_array(std::array<T, Sz> const &_array);
//       constexpr std_array(std::array<T, Sz> &&_array);

//       template<class... U>
//       requires (sizeof...(U) == Sz) && (std::convertible_to<U, T> && ...)
//       constexpr std_array(U&&... u);

//       // // Block runtime-size list path
//       // std_array(std::initializer_list<T>) = delete;

//       // constexpr std_array(list_type list);
//     };

//   };

// };