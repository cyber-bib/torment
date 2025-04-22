#pragma once

#include <memory>
#include <boost/operators.hpp>
#include "array/dense/array.hxx"

// #include "iface.hxx"

namespace torment {

  namespace radix {

    template<class T, std::size_t Sz = 0>
    struct unsigned_mixed_system
    : dense::array<T, Sz>
      // arithmetic_interface<unsigned_mixed_system<T, Sz>>
    {
      static_assert(std::is_integral_v<T>, "T is not an integral type" );
      static_assert(std::is_unsigned_v<T>, "T is not an unsigned integral type");

      typedef dense::array<T, Sz> base_type;
      typedef base_type::list_type list_type;

      std::size_t maxval() const;
      base_type bases() const;

      unsigned_mixed_system& operator++();
      unsigned_mixed_system& operator+=(value_type val);

      bool operator<(unsigned_mixed_system const &val) const;

      unsigned_mixed_system(list_type const &bases);
      unsigned_mixed_system(std::shared_ptr<base_type const> bases);

      unsigned_mixed_system& operator=(value_type const& val);
      unsigned_mixed_system& operator=(list_type const &bases);

      // protected:
        std::shared_ptr<base_type const> m_bases;
      private:
        void init();
    };

  } // namespace radix

} // namespace torment
