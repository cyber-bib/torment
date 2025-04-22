#pragma once

// #include "pair/pair.hxx"
#include <map>


namespace torment {
  template< class Key,
            class Value,
            class Compare,
            class Elem,
            class Alloc,
            class Cont      > struct map;

  #ifdef _IOSTREAM_ // {

  template< class K, class V, class C, class E, class A, class T>
  std::ostream& operator<<(
    std::ostream &os,
    map<K,V,C,E,A,T> const & mp
  );

  #endif // } _IOSTREAM_

  template< class Key,
            class Value,
            class Compare = std::less<Key>,
            class Elem    = std::pair<const Key, Value>,
            class Alloc   = std::allocator<Elem>,
            class Cont    = std::map<Key, Value, Compare, Alloc>>
  struct map : public Cont
  {
    typedef Cont container_type;
    typedef std::pair<const Key, Value> elem_type;
    typedef std::initializer_list<elem_type> list_type;

    using container_type::container_type;

    // map(list_type const& list);

    #ifdef _IOSTREAM_ // {

    friend std::ostream&  operator<< <Key, Value>(std::ostream &os, map const & cont);

    #endif // } _IOSTREAM_
  };
}