#pragma once

#include "base.hxx"

namespace torment {

  namespace dense {

    template<class T, std::size_t Sz = 0>
    struct array : base<T, Sz> {
      typedef base<T,Sz> base_type;
      typedef base_type::value_type value_type;

      using base_type::base_type;
    };

  };

};