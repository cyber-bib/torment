#pragma once

#include <utility>

namespace torment {
  template<class F, class S> struct pair;

  #ifdef _IOSTREAM_ // {

  template<class F, class S>
  std::ostream& operator<< (std::ostream& os, pair<F, S> const& arg);

  #endif // } _IOSTREAM_

  template<class F, class S>
  struct pair : std::pair<F, S> {
    typedef std::pair<F, S> base_type;

    using base_type::base_type;


    #ifdef _IOSTREAM_ // {

    friend std::ostream& operator<< <F,S>(std::ostream& os, pair<F, S> const& arg);

    #endif // } _IOSTREAM_
  };
}