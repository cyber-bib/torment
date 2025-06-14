#pragma once

#include <cassert>

#include "map/map.hxx"
#include "radix/mix.hxx"

namespace torment {

  namespace sparse {
    // template<class Value, class Key, class Cont> struct base;

    template<std::size_t Rank>
    constexpr bool is_multidimentional = (Rank != 1);

    template<class Index, std::size_t Rank>
    using key_type = std::conditional_t<  is_multidimentional<Rank>,
                                          ::torment::dense::base<Index, Rank>,
                                          Index  >;

    template< class       Value,
              std::size_t Rank,
              class       Index,
              class       Key,
              class       Cont    > struct base;

    #ifdef _IOSTREAM_ // {

    template< class V, std::size_t R, class I, class K, class C>
    std::ostream& operator<<(std::ostream &os, base<V,R,I,K,C> const &sarr);

    #endif // } _IOSTREAM_



    template< class Value,
              std::size_t Rank = 0,
              class Index = std::size_t,
              class Key = key_type<Index, Rank>,
              class Cont = map<Key, Value>>
    struct base : Cont {
      // static_assert(std::is_integral_v<Index>, "'Key' is not of integral type");
      // static_assert(!std::is_signed_v<Index>, "'Key' does not have unsigned property");

      typedef Index index_type;
      typedef Key key_type;
      typedef Value value_type;
      typedef Cont container_type;

      typedef typename container_type::element_type element_type;
      typedef typename container_type::list_type list_type;

      key_type m_dims;
      value_type m_dval;
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

      template<std::size_t R = Rank, typename = std::enable_if_t<R==0>>
      explicit base(  index_type dims = index_type(0),
                      value_type const& default_val = 0,
                      list_type const& list = {}                              );

      explicit base(  key_type dims = key_type(0),
                      value_type const& default_val = 0,
                      list_type const& list = {}                              );

      // base( list_type const& list,
      //       key_type && dims = key_type(-1),
      //       value_type && default_v = 0                                       );

      // base( list_type const& list,
      //       key_type const& dims = key_type(0),
      //       value_type const& default_v = 0                                   );
      template<std::size_t R = Rank, typename = std::enable_if_t<R==0>>
      base& operator=(value_type const &val);
      base& operator=(list_type const &list);


      std::size_t n_elem() const;
      key_type const& shape() const;
      void shape(key_type const& new_shape);

      value_type const& default_value() const;
      void default_value(value_type const&);

      // struct element_iterator_base;
      struct element_iterator;
      // struct element_const_iterator;

      // element_iterator begin();
      // element_const_iterator begin() const;

      // element_iterator end();
      // element_const_iterator end() const;

      struct element_reference;
      element_reference operator[](key_type const& key);
      value_type const& operator[](key_type const& key) const;
      // std::size_t size() const;

  #   ifdef _IOSTREAM_ // {

      friend std::ostream& operator<< <Value, Rank, Index, Key, Cont>(std::ostream &os, base const &sarr);

  #   endif // } _IOSTREAM_

      template<std::size_t R = Rank, typename = std::enable_if_t<R==0>>
      bool is_list_valid(list_type const &list) const;

      template<std::size_t R = Rank, typename = std::enable_if_t<R==0>>
      void throw_if_list_not_valid(list_type const &list) const;
    };


    template<class V, std::size_t R, class I, class K, class C>
    struct base<V,R,I,K,C>::element_reference {
      key_type m_key;
      base &m_chain;

      explicit element_reference(key_type const &key, base &chain);

      operator value_type() const;
      element_reference& operator=(value_type const& value);
    };


    // template<class V, std::size_t R, class I, class K, class C>
    // struct base<V,R,I,K,C>::element_iterator_base {
    //   typedef radix::unsigned_mixed_system<I, R> iterable_type;

    //   iterable_type m_ikey;
    //   base &m_chain;
    //   bool m_ignore_first_equallity;

    //   explicit element_iterator_base(iterable_type const& ikey, base &chain);
    // };


    template<class V, std::size_t R, class I, class K, class C>
    struct base<V,R,I,K,C>::element_iterator
    : boost::incrementable<element_iterator>,
      boost::equivalent<element_iterator>
    {
      typedef typename radix::unsigned_mixed_system<I, R> iterable_type;
      typedef typename base<V,R,I,K,C>::element_reference element_reference;
      typedef typename base<V,R,I,K,C>::element_reference element_const_reference;

      iterable_type m_ikey;
      base &m_chain;

      explicit element_iterator(iterable_type const& ikey, base &chain);

      element_reference operator*();
      // element_const_reference operator*() const;

      element_iterator& operator++();
      bool operator<(element_iterator const &rhs) const;
    };


    // template<class V, std::size_t R, class I, class K, class C>
    // struct base<V,R,I,K,C>::element_const_iterator {
    //   typedef typename radix::unsigned_mixed_system<I, R> iterable_type;

    //   iterable_type m_ikey;
    //   base &m_chain;
    //   bool m_ignore_first_equallity;

    //   explicit element_const_iterator(iterable_type const& ikey, base &chain);
    // };

    // template<class V, class K, class C>
    // base<V,K,C>::key_type const& base<V,K,C>::shape() const {

    // }




  };

};
