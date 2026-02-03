#include "../../array/dense/core.hxx"
#include "boost/operators.hpp"

namespace torment {

namespace dense {

  template<class T, std::size_t S = 0>
  struct vector 
  : base<T, 1, urr(S)>,
    boost::ring_operators<base<T, 1, urr(S)>>
  {
    typedef base<T, 1, urr(S)> base_type;

    using base_type::base_type;
    
  };

  // template<class T, std::size_t R = 0, std::size_t C = 0>
  // struct matrix : base<T, 2, urr(R, C)> {
  //
  // };

};

};
