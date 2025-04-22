#pragma once

#include "array/std_array.hxx"

#include <type_traits>
#include <vector>

namespace torment {

  namespace dense {

    template<std::size_t Sz>
    constexpr bool is_dynamic = (Sz == 0);

    template<class T, std::size_t Sz>
    using container_type = std::conditional_t<  is_dynamic<Sz>,
                                                std::vector<T>,
                                                std_array<T, Sz>  >;

    template<class T, std::size_t Sz> struct base;


    #ifdef _IOSTREAM_ // {

    template<class T, std::size_t Sz>
    std::ostream& operator<<(std::ostream &os, base<T, Sz> const &base);

    #endif // } _IOSTREAM_

    template<class T, std::size_t Sz>
    struct base : container_type<T, Sz> {
      typedef typename T value_type;
      typedef std::initializer_list<T> list_type;
      typedef container_type<value_type, Sz> container_t;

      using container_t::container_t;


      #ifdef _IOSTREAM_ // {

      friend std::ostream& operator<< <T, Sz>(std::ostream &os, base const &arr);

      #endif // } _IOSTREAM_
    };


  };

};