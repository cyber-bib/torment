#pragma once

#include "../std_array.hxx"

#include <vector>
#include <type_traits>

namespace torment {

  namespace dense {

    template<std::size_t Sz>
    constexpr bool is_dynamic = (Sz == 0);

    template<class T, std::size_t Sz>
    using container_type = std::conditional_t<  is_dynamic<Sz>,
                                                std::vector<T>,
                                                std_array<T, Sz>  >;

    template<class T, std::size_t Sz> struct smart_container;



    // template <class T>
    // struct is_base : std::false_type {};

    // template <class T, std::size_t Sz>
    // struct is_base<smart_container<T, Sz>> : std::true_type {};

    // template <class T>
    // inline constexpr bool is_base_v = is_base<T>::value;

    #ifdef _IOSTREAM_ // {

    template<class T, std::size_t Sz>
    std::ostream& operator<<(std::ostream &os, smart_container<T, Sz> const &smart_container);

    #endif // } _IOSTREAM_

    template<class T, std::size_t Sz = 0>
    struct smart_container : container_type<T, Sz> {
      typedef T value_type;
      typedef std::initializer_list<T> list_type;
      typedef container_type<value_type, Sz> container_t;

      using container_t::container_t;
      // using container_t::operator=;


      #ifdef _IOSTREAM_ // {

      friend std::ostream& operator<< <T, Sz>(std::ostream &os, smart_container const &arr);

      #endif // } _IOSTREAM_
    };


  };

};
