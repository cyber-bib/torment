#pragma once

#include "array/dense.hxx"

#include <string>

namespace torment {

  template< class T, std::size_t Sz>
  dense_array<T, Sz>::dense_array(int val) {
    for(auto &e : *this) e = val;
  }

  template< class T, std::size_t Sz>
  dense_array<T, Sz>::dense_array(
    list_type const &list
  ) {
    this->operator=(list);
  }

  template< class T, std::size_t Sz> dense_array<T, Sz>&
  dense_array<T, Sz>::operator=(
    list_type const &list
  ) {
    if (list.size() != Sz)
      throw std::invalid_argument("list size != " + std::to_string(Sz));

    auto it = list.begin();
    for(auto &e : *this) e = *(it++);

    return *this;
  }

  #ifdef _IOSTREAM_ // {

  // template< class T, std::size_t Sz> std::ostream&
  template< class T, std::size_t Sz> std::ostream&
  operator<<(
    std::ostream &os,
    dense_array<T,Sz> const & array
  ) {
    auto size = array.size();

    os << "{";
    if(size != 0) {
      for(std::uint64_t i = 0; i < size - 1; i++)
        os << static_cast<std::size_t>(array[i]) << ", ";
      os << static_cast<std::size_t>(array[size - 1]);
    }
    os << "}";

    return os;
  }

  #endif // } ifdef _IOSTREAM_


};