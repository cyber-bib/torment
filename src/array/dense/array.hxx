#include "core.hxx"
#include "boost/operators.hpp"

namespace torment {

  template<class LHS, class RHS = LHS>
  struct multiplicable {

    friend LHS& operator*=(LHS& lhs, RHS const &rhs) {
      lhs.rhs_arg_operator_worker(  rhs,
        std::multiplies<typename LHS::value_type>()  );

      return lhs;
    }

  };

  namespace dense {



    template< class T,
              std::size_t Rk = 0,
              std::array<std::size_t, Rk> Sp
                = std::array<std::size_t, Rk>{}  >
    struct array 
    : base<T, Rk, Sp>,
      ::torment::multiplicable< array<T,Rk,Sp> >,
      ::torment::multiplicable< array<T,Rk,Sp>, T>
    {
      typedef base<T, Rk, Sp> base_type;

      using base_type::base_type;
        
        
      template<class Arg, class BinaryOp>
      void rhs_arg_operator_worker(Arg const& arg, BinaryOp const& binary_op);

      // template<class Arg, class BinaryOp>
      // void lhs_arg_operator_worker(Arg const& arg, BinaryOp const& binary_op);

    };

  };

};
