#pragma once

#ifndef TORMENT_DENSE_ARRAY_H // {

#include <array>
#include <iostream>

namespace torment {

  template< class T, std::size_t Sz> struct  dense_array;

  template<class T, std::size_t Sz> std::ostream&
  operator<< (std::ostream &os, dense_array<T, Sz> const & array);
  // template<class T> std::ostream&
  // operator<< (std::ostream &os, T const & arg);

  template<class T, std::size_t Sz = 0>
  struct dense_array : std::array<T, Sz> {
    typedef std::initializer_list<T> list_type;

                explicit  dense_array(int val = 0);
                          dense_array(list_type const &list);

            dense_array&  operator=  (list_type const &list);

    #ifdef _IOSTREAM_ // {

    friend std::ostream&
    operator<< <T, Sz>(std::ostream &os, dense_array const & array);

    #endif // } ifdef _IOSTREAM_
  };

};

#endif // } TORMENT_DENSE_ARRAY_H