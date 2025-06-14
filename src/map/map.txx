#pragma once
#include "map.hxx"

namespace torment {


  // template< class K,
  //           class V,
  //           class C,
  //           class E,
  //           class A,
  //           class T  >
  // map<K,V,C,E,A,T>::map(list_type const & list)
  // {

  // }

  #ifdef _IOSTREAM_ // {

  template< class Key,
            class Value,
            class Compare,
            class Elem,
            class Alloc,
            class Cont  > std::ostream&
  operator<<(
    std::ostream &os,
    map<Key, Value, Compare, Elem, Alloc, Cont> const & mapa
  ) {

    if(auto it = mapa.begin(); it != mapa.end()) {
      os << "{ (" << it->first << ": " << (it++)->second << ")";
      for(; it != mapa.end(); it++)
        os << ",\n  (" << it->first << ": " << it->second << ")";
      os << " }";
    }
    return os;
  }

  #endif // } _IOSTREAM_
};