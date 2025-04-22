#pragma once


#include "map/map.hxx"

namespace torment {
  namespace sparse {
    template<class Value, class Key, class Cont> struct base;

    #ifdef _IOSTREAM_ // {

    template< class Value, class Key, class Cont>
    std::ostream& operator<<(std::ostream &os, base<Value, Key, Cont> const &sarr);

    #endif // } _IOSTREAM_

    template< class Value,
              class Key = std::size_t,
              class Cont = map<Key, Value>>
    struct base : Cont {
      typedef Key key_type;
      typedef Value value_type;
      typedef Cont container_type;

      typedef typename container_type::elem_type elem_type;
      typedef typename container_type::list_type list_type;

      // template<class T>
      // using enable_if_integral_t =
      //   typename std::enable_if_t<  std::is_integral_v<T>
      //                                                         , bool>;

      // template<class T>
      // using enable_if_rank_gt_1 =
      //   typename std::enable_if_t< !std::is_integral_v<T> &&
      //                               std::is_same_v<T, K>
      //                                                         , bool>;

      // template<class T>
      // using enable_if_rank_1 =
      //   typename std::enable_if_t<  std::is_integral_v<T> &&
      //                               std::is_same_v<T, K>
      //                                                         , bool>;

      // using container_type::container_type;

      // base(elem_type const& default_elem = {key_type(-1), value_type(0)});
      // base(list_type const& list, elem_type const& default_elem = {key_type(-1), value_type(0)});

      // base( key_type && dims = key_type(-1),
      //       value_type && default_v = 0                                       );

      explicit base(  key_type const& dims = key_type(0),
                      value_type const& default_v = 0,
                      list_type const& list = {}                              );

      // base( list_type const& list,
      //       key_type && dims = key_type(-1),
      //       value_type && default_v = 0                                       );

      // base( list_type const& list,
      //       key_type const& dims = key_type(0),
      //       value_type const& default_v = 0                                   );

      base& operator=(list_type &&list);


      std::size_t n_elem() const;

      key_type const& shape() const;
      void shape(key_type const& new_shape);

      value_type const& default_value() const;
      void default_value(value_type const&);

      value_type const& operator()(key_type const& key) const;
      // std::size_t size() const;

      #ifdef _IOSTREAM_ // {

      friend std::ostream& operator<< <Value, Key, Cont>(std::ostream &os, base const &sarr);

      #endif // } _IOSTREAM_
    };

    // template<class V, class K, class C>
    // base<V,K,C>::key_type const& base<V,K,C>::shape() const {

    // }
  };

};