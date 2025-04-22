#pragma once

#include "pair/pair.hxx"


namespace torment {

    #ifdef _IOSTREAM_ // {

    template<class F, class S>
    std::ostream& operator<< <F,S>(std::ostream& os, pair<F, S> const& arg) {
      os << "\"" << arg.first << "," << arg.second << "\"";
      return os;
    }

    #endif // } _IOSTREAM_
}