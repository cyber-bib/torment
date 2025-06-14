#pragma once

#include <initializer_list>
#include <memory>
#include <boost/operators.hpp>
#include "array/dense/base.hxx"

// #include "iface.hxx"

namespace torment {

  namespace radix {

    template<class T, std::size_t Sz = 0>
    struct unsigned_mixed_system
    : dense::base<T, Sz>,
      boost::incrementable<unsigned_mixed_system<T, Sz>>,
      boost::decrementable<unsigned_mixed_system<T, Sz>>,
      boost::additive<unsigned_mixed_system<T, Sz>, std::size_t>,
      boost::equivalent<unsigned_mixed_system<T, Sz>, std::size_t>
      // arithmetic_interface<unsigned_mixed_system<T, Sz>>
    {
      // static_assert(std::is_integral_v<T>, "T is not an integral type" );
      // static_assert(std::is_unsigned_v<T>, "T is not an unsigned integral type");

      typedef dense::base<T, Sz> base_type;
      typedef base_type::value_type value_type;
      typedef base_type::list_type list_type;

      using base_type::operator=;

      bool m_overflow;

      // unsigned_mixed_system(unsigned_mixed_system const &bases);
      unsigned_mixed_system();

      explicit unsigned_mixed_system(list_type const &bases);
      explicit unsigned_mixed_system(base_type const &bases);
      explicit unsigned_mixed_system(std::shared_ptr<base_type> bases);

      unsigned_mixed_system& operator=(value_type const& val);
      unsigned_mixed_system& operator=(list_type const &list);

      bool overflow() const;
      unsigned_mixed_system& overflow(bool set_unset);
      // unsigned_mixed_system& operator=(base_type const &arg);

      base_type radices() const;
      void radices(base_type const& radices);
      std::size_t global_radix() const;

      std::size_t add(std::size_t const &val, std::size_t i = 0);

      unsigned_mixed_system& operator++();
      unsigned_mixed_system& operator--();


      unsigned_mixed_system& operator+=(std::size_t val);
      unsigned_mixed_system& operator-=(std::size_t val);

      bool operator<(value_type const &val) const;
      bool operator>(value_type const &val) const;

      bool operator<(unsigned_mixed_system const &val) const;


      // protected:
        std::shared_ptr<base_type> m_radices;
      protected:
        void init();
    };

    // mrns<mrns<u64, 2>, 2> k({grid, grid});
    template<class T, std::size_t Sz, std::size_t Sy>
    struct unsigned_mixed_system<unsigned_mixed_system<T, Sy>, Sz>
    : unsigned_mixed_system<T, Sy*Sz>
      // boost::incrementable<unsigned_mixed_system<unsigned_mixed_system<T, Sy>, Sz>>,
      // boost::decrementable<unsigned_mixed_system<unsigned_mixed_system<T, Sy>, Sz>>,
      // boost::additive<unsigned_mixed_system<unsigned_mixed_system<T, Sy>, Sz>>,
      // boost::equivalent<unsigned_mixed_system<unsigned_mixed_system<T, Sy>, Sz>>
    {
      typedef unsigned_mixed_system<T, Sy*Sz> base_type;
      // typedef dense::base<T, Sz> base_type;
      typedef base_type::base_type array_type;
      typedef base_type::value_type value_type;
      typedef base_type::list_type list_type;

      using base_type::base_type;
      unsigned_mixed_system(
        std::initializer_list<unsigned_mixed_system<T, Sy>> const &helper_list);
      // using base_type::operator<;
      using base_type::operator=;
      // typedef base_type::value_type value_type;
      // typedef base_type::list_type list_type;
    };
  } // namespace radix

} // namespace torment
