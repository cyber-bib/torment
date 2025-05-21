#pragma once

#include "base.hxx"

#include <numeric>

namespace torment {

  namespace dense {

    template<class First, class... Rest>
    constexpr auto urr(First first, Rest... rest) {
      static_assert((std::is_same_v<First, Rest> && ...), "All elements must be the same type");

      return std::array<std::size_t, 1 + sizeof...(Rest)>{
        static_cast<std::size_t>(first),
        static_cast<std::size_t>(rest)...};
    }

    template<std::size_t Rank>
    constexpr bool is_multidimensional = (Rank != 1);

    template<class Index, std::size_t Rank>
    using shape_t    = std::conditional_t<  is_multidimensional<Rank>,
                                            ::torment::dense::base<Index, Rank>,
                                            Index  >;

    template<class Index, std::size_t Rank>
    constexpr std::size_t zredmult(
      std::array<Index, Rank> const &arg
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
      bool Dynamic = is_dynamic<zredmult(Shape)>>
    struct shape {
      typedef shape_t<Index, Rank> array_type;

      static_assert(
        !is_dynamic<zredmult(Shape)>,
        "template parameter \"Dynamic\" modified, "
        "not intended to be manually overwritten.");

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
      std::array<std::size_t, Rk> Sp,
      std::size_t Sz> struct array;

    template<
      class T,
      std::size_t Rk,
      std::array<std::size_t, Rk> Sp,
      std::size_t Sz>
    std::ostream&
    operator<<(
      std::ostream &os,
      array<T,Rk,Sp,Sz> const &arrg  );

    template<
      class T,
      std::size_t Rk = 0,
      std::array<std::size_t, Rk> Sp
        = std::array<std::size_t, Rk>{},
      std::size_t Sz = zredmult(Sp) >
    struct array
    : shape<std::size_t, Rk, Sp, is_dynamic<Sz>>,
      base<T, (is_dynamic<Rk> ? 0 : Sz)>
    {
      static_assert(Sz == zredmult(Sp),
        "Do not modify this value, "
        "not intended to be modified. ");

      typedef shape<std::size_t, Rk, Sp, is_dynamic<Sz>> shape_type;
      typedef shape_type::array_type shape_array_type;

      typedef base<T, (is_dynamic<Rk> ? 0 : Sz)> base_type;
      typedef base_type::value_type value_type;

      using base_type::base_type;
      using base_type::operator=;

      // template<typename
      //   = std::enable_if_t<is_multidimensional<Rk> &&
      //     is_dynamic<Sz>>>
      array(shape_array_type const& shape, value_type const& val = 0);

      shape_array_type shape() const;

      // value_type& operator[]();

      #ifdef _IOSTREAM_ // {

      friend std::ostream& operator<< <T,Rk,Sp,Sz>(std::ostream &os, array const &arr);

      #endif // } _IOSTREAM_
    };

  };

};
