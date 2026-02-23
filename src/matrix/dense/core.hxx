#include "../../array/dense/array.hxx"

namespace torment {

namespace dense {

  template<class T, ::std::size_t R = 0, std::size_t C = 0>
  struct matrix
  : array<T, 2, std::size_t, urr(R, C)>
  {
    // typedef base<T, 2, ::std::size_t, urr(R, C)> base_type;
    // using base_type::base_type;

  };


};

};