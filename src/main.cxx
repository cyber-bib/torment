#include <iostream>

#include "array/sparse/2d.txx"

using namespace torment;
using namespace torment::sparse;

template<class V, class K = std::size_t>
struct matrix
: torment::sparse::array2d<V, K> {

};


int main(int argc,char *argv[]) {
  try {
    matrix<float> M;

    // dense::array<unsigned, 3> radices = {2, 1, 4};
    // radix::unsigned_mixed_system<unsigned, 3> num = {10, 10, 10};
    // num = {1, 0, 0};

    // num += 1;
    // num += 101;


    // num++;

    // std::cout << "max val = " << num.maxval() << ": " << num.bases() << "\n";
    // std::cout << num << "\n";

    // array2d<int> b(,);
    // dense::array<std::size_t, 2> dims = {4, 4};
    // array2d<int>::key_type keys = dims;

    // typedef array2d<int>::key_type key_t;

    // key_t k({4, 4});
    // k = 15;
    // std::cout << "k = " << k << "\n";

    // array2d<int> b(key_t{10, 10}, 0);

    // b = { { key_t{0, 0}, 1},
    //       { key_t{1, 1}, 3},
    //       { key_t{2, 2}, 3},
    //       { key_t{3, 3}, 5}  };

    // std::cout << "b.size() = " << b.size() << "\n"
    //           << "b.shape() = " << b.shape() << "\n"
    //           << "b = " << b << "\n";

    // base<int> a =   { 1, 2, 3,
    //                   4, 5, 6,
    //                   7, 8, 9  };


    // dense::array<int, 3> a;
    // a = {1, 3, 4};

    // std::array<int, 3> a = {1, 2, 3, 4};

    // base_array<int, 0> a = {1, 2, 3};
    // dense_array<int, 3> a = {1, 2, 3};

    // array<int> a( 5, 0,
    //               { {1, 2},
    //                 {3, 3},
    //                 {5, 5},
    //                 {7, 7},
    //                 {9, 9}  });
    // a = { {1, 2},
    //       {3, 3},
    //       {5, 5},
    //       {7, 7},
    //       {9, 9}  };

    // std::cout << "a.n_elem() = " << a.n_elem() << "\n"
    //           << "a.array()  = " << a << "\n"
    //           << "a.map()    = " << (array<int>::container_type)a << "\n";


    // map<Dense<int, 2>, int> m =  {  {{0, 0}, 0},
    //                                 {{1, 3}, 1},
    //                                 {{1, 0}, 1},
    //                                 {{2, 0}, 2}  };
    // std::cout << m << "\n";
  } catch(std::exception &ex) {
    std::cerr << ex.what() << "\n";
    throw ex;
  }
  return 0;
}
