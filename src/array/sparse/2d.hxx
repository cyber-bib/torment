#pragma once

#include "radix/mix.hxx"
#include "array/sparse/base.hxx"

namespace torment {

  namespace sparse {

    // template<class V, class K = std::size_t>
    // struct array2d : base<V, radix::unsigned_mixed_system<K, 2>> {
    //   // typedef radix::unsigned_mixed_system<K, 2> key_type;
    //   typedef base<V, radix::unsigned_mixed_system<K, 2>> base_type;
    //   typedef typename base_type::value_type value_type;

    //   using base_type::base_type;
    //   using base_type::operator=;

    //   // value_type const& operator=(K const& id) const {
    //   //   return 0;
    //   // }
    // };

    template<class V, class K = std::size_t>
    struct array2d : base<V, dense::array<K, 2>> {
      // typedef radix::unsigned_mixed_system<K, 2> key_type;
      typedef base<V, dense::array<K, 2>> base_type;
      typedef typename base_type::value_type value_type;

      using base_type::base_type;
      using base_type::operator=;

      // value_type const& operator=(K const& id) const {
      //   return 0;
      // }
    };

  };

};