#pragma once

#include <numeric>
#include "smart_container.hxx"


namespace torment {

  namespace dense {

    /* @brief Helper function: unsigned array initializer for template arguments
     *
     */
    template<class First, class... Rest>
    constexpr auto urr(First first, Rest... rest) {
      static_assert((std::is_same_v<First, Rest> && ...), "All elements must be the same type");

      return std::array<First, 1 + sizeof...(Rest)>{first, rest...};

      // return std::array<std::size_t, 1 + sizeof...(Rest)>{
      //   static_cast<std::size_t>(first),
      //   static_cast<std::size_t>(rest)...};
    };


    /* @brief Helper boolean: for determining the shape's container type
     *
     */
    template<std::size_t Rank>
    constexpr bool is_multidimensional = (Rank != 1);

    /* @brief Helper template: for determining the shape's container type
     *
     */
    template<class Index, std::size_t Rank>
    using shape_t    = std::conditional_t<  is_multidimensional<Rank>,
                                            ::torment::dense::smart_container<Index, Rank>,
                                            Index  >;

    /*  @brief Helper function: used to find the size of equivanlet-size of flat array.
     *
     *  This is a reduce_multiply transformation with a default value of zero
     *  if the input array is empty.
     *
     */
    template<class Index, std::size_t Rank>
    constexpr std::size_t zredmult (
      std::array<Index, Rank> const &arg
    ) {
      return (arg.size() > 0 ? std::reduce (
          arg.begin(),
          arg.end(),
          std::size_t(1),
          std::multiplies{}) : 0);
    }
    template<class Index>
    std::size_t zredmult (
      std::vector<Index> const &arg
    ) {
      return (arg.size() > 0 ? std::reduce (
          arg.begin(),
          arg.end(),
          std::size_t(1),
          std::multiplies{}) : 0);
    }

    template<
      class Index,
      std::size_t Rank,
      std::array<Index, Rank> Shape = std::array<Index, Rank>{},
      bool Dynamic = is_dynamic<zredmult(Shape)>  >
    struct shape {
      static_assert(
        std::is_integral_v<Index>,
        "template parameter \"Index\" is not of integral type.");

      static_assert(
        !is_dynamic<zredmult(Shape)>,
        "template parameter \"Dynamic\" modified, "
        "not intended to be manually overwritten.");

      typedef shape_t<Index, Rank> array_type;

      static constexpr array_type m_shape = Shape;
    };

    // shape<std::size_t, 0, false>::m_shape = std::vector{1,2,3};
    template<
      class Index,
      std::array<Index, 1> Shape>
    struct shape<Index, 1, Shape, false> {
      typedef shape_t<Index, 1> array_type;

      static_assert(
        !is_dynamic<zredmult(Shape)>,
        "template parameter \"Dynamic\" modified, "
        "not intended to be manually overwritten.");

      static constexpr array_type m_shape = Shape[0];
    };

    template<
      class Index,
      std::size_t Rank,
      std::array<Index, Rank> Shape>
    struct shape<Index, Rank, Shape, true> {
      typedef shape_t<Index, Rank> array_type;

      static_assert(
        is_dynamic<zredmult(Shape)>,
        "template parameter \"Dynamic\" modified, "
        "not intended to be manually overwritten.");

      array_type m_shape;

      // template<typename = std::enable_if_t<is_multidimensional<Rank>>>
      shape() : m_shape{} {}
      explicit shape(array_type const& _shape) : m_shape(_shape) {}

      // template<class S = shape_type, typename = std::void_t<typename S::list_type>>
      // shape(typename S::list_type const& _shape) : m_shape(_shape) {
      //   static_assert(
      //     typeid(S) == typeid(shape_type),
      //     "template parameter S is not intened to be explicitly modified.");
      // }

      // shape(shape_type const& _shape) : m_shape(_shape) {}
    };

    template<
      class T,
      std::size_t Rk,
      class Idx,
      std::array<Idx, Rk> Sp,
      std::size_t Sz> struct base;

    #ifdef _IOSTREAM_ // [

    template<
      class T,
      std::size_t Rk,
      class Idx,
      std::array<Idx, Rk> Sp,
      std::size_t Sz>
    std::ostream&
    operator<<(
      std::ostream &os,
      base<T,Rk,Idx,Sp,Sz> const &arrg  );

    #endif // ] _IOSTREAM_

    template<
      class T,
      std::size_t Rk = 0,
      class Idx = std::size_t,
      std::array<Idx, Rk> Sp
        = std::array<Idx, Rk>{},
      std::size_t Sz = zredmult(Sp) >
    struct base
    : shape<Idx, Rk, Sp, is_dynamic<Sz>>,
      smart_container<T, (is_dynamic<Rk> ? 0 : Sz)>
    {
      static_assert(Sz == zredmult(Sp),
        "Do not modify this value, "
        "not intended to be modified. ");

      typedef shape<Idx, Rk, Sp, is_dynamic<Sz>> shape_type;
      typedef typename shape_type::array_type index_type;
      // typedef base<index_type, 1> indices_type;

      typedef smart_container<T, (is_dynamic<Rk> ? 0 : Sz)> base_type;

      typedef typename base_type::list_type list_type;
      typedef typename base_type::value_type value_type;

      using base_type::base_type;
      using base_type::operator[];
      // using base_type::operator=;


      base(list_type list);
      base& operator=(list_type list);

      template<
        std::size_t _Sz = Sz,
        typename = std::enable_if_t<is_dynamic<_Sz>>>
      base(
        index_type const& shape,
        value_type const& val = value_type{} );

      base& assign(
        index_type const& shape,
        value_type const& val = value_type{} );

      void  resize(
        index_type const& shape,
        value_type const& val = value_type{} );

      index_type shape() const;

      // template<
      //   std::size_t _Rk = Rk,
      //   std::size_t _Sz = Sz,
      //   typename = std::enable_if_t<is_multidimensional<_Rk> && is_dynamic<_Sz>>>
      template<
        std::size_t _Rk = Rk,
        typename = std::enable_if_t<is_multidimensional<_Rk>>  >
      value_type& operator[](index_type const& addr);

      template<
        std::size_t _Rk = Rk,
        typename = std::enable_if_t<is_multidimensional<_Rk>>  >
      value_type const& operator[](index_type const& addr) const;

      template<
        std::size_t _Rk = Rk,
        typename = std::enable_if_t<is_multidimensional<_Rk>>  >
      index_type strides() const;

      template<
        std::size_t _Rk = Rk,
        typename = std::enable_if_t<is_multidimensional<_Rk>>  >
      std::size_t id_from(index_type const& addr) const;

      template<
        std::size_t _Rk = Rk,
        typename = std::enable_if_t<is_multidimensional<_Rk>>  >
      index_type addr_from(std::size_t const& id) const;

      #ifdef _IOSTREAM_ // {

      friend std::ostream& operator<< <T,Rk,Idx,Sp,Sz>(std::ostream &os, base const &arg);

      #endif // } _IOSTREAM_
    };

  };

};
