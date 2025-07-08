#include <array>
#include <sstream>

#pragma region include-headers [

#include <boost/preprocessor.hpp>

#include <memory>
#include <numeric>
#include <functional>
#include <iostream>

#include "radix/mix.txx"
#include "array/dense/base.txx"
#include "array/sparse/array.txx"

#include "gtest/gtest.h"

#pragma endregion ] include-headers

#define PRINT(os_code) std::cout << #os_code ": " << os_code
#define PRINTLN(os_code) PRINT(os_code) << "\n"

#define PRETTY_PRINT_1(os_code) \
  std::cout << #os_code ":\n" << os_code << "\n"
#define PRETTY_PRINT_2(os_code, os_setw) \
  std::cout << #os_code ":\n" << std::setw(os_setw) << os_code << "\n"

#if !BOOST_PP_VARIADICS_MSVC
# define PRETTY_PRINT(...) BOOST_PP_OVERLOAD(PRETTY_PRINT_,__VA_ARGS__)(__VA_ARGS__)
#else
# define MACRO_ADD_NUMBERS(...) \
  BOOST_PP_CAT(BOOST_PP_OVERLOAD(PRETTY_PRINT_,__VA_ARGS__)(__VA_ARGS__),BOOST_PP_EMPTY())
#endif


#pragma region forward-decls [

// template<class T>
// using dense = torment::dense::smart_container<T, 0>;

template<std::size_t N>
using ivec = torment::dense::base<int, 1, torment::dense::urr(N)>;

template<std::size_t R, std::size_t C>
using imat = torment::dense::base<int, 2, torment::dense::urr(R, C)>;

template<std::size_t N>
using ispec = torment::sparse::array<int, 1>;

template<class T, std::size_t N = 0>
using mrns = torment::radix::unsigned_mixed_system<T, N>;

void* operator new(std::size_t size);

class memory_usage {
  std::size_t m_usage;
  memory_usage() : m_usage(0) {}

  public:

  void reset() { m_usage = 0; }
  std::size_t usage() const { return m_usage; }

  static memory_usage& instance() {
    static memory_usage dst;
    return dst;
  }

  memory_usage(memory_usage && other) = delete;
  memory_usage(memory_usage const& other) = delete;
  memory_usage& operator=(memory_usage const& other) = delete;

  void print() const {
    std::cout << "memory usage: " << *this << " byte"
              << (this->m_usage == 1 ? "" : "s") << "\n";
  };

  friend std::ostream& operator<<(
    std::ostream &os,
    memory_usage const& mem
  ) {
    os << mem.m_usage;
    return os;
  }
  friend void* operator new(std::size_t size);
};

void* operator new(std::size_t size) {
  auto &mem = memory_usage::instance();
  mem.m_usage += size;

  return malloc(size);
}

#pragma endregion ] forward-decls
#pragma region DenseArrayTests [


TEST(DenseArrayTests, ShapeStructTest) {
  using namespace torment::dense;

  constexpr auto _ = std::array<std::size_t, 0>{};
  constexpr auto _0 = urr(0);
  constexpr auto _3 = urr(3);
  constexpr auto _00 = urr(0, 0);
  constexpr auto _33 = urr(3, 3);
  auto _123 = std::vector<std::size_t>{1, 2, 3};

  shape<std::size_t, 0>       d0({1, 2, 3});

  shape<std::size_t, 1>       d1(3);
  shape<std::size_t, 1, _3>   s1;

  shape<std::size_t, 2>       d2({3, 3});
  shape<std::size_t, 2, _33>  s2;

  // shape<std::size_t, 0, _, false>   e1;        // expected to failed
  // shape<std::size_t, 1, _0, false>   e1;       // expected to failed
  // shape<std::size_t, 1, _3, true>    e10({2}); // expected to failed
  // shape<std::size_t, 2, _00, false>  e2;       // expected to failed


  EXPECT_EQ(d0.m_shape, _123);
  EXPECT_EQ(d1.m_shape, 3);
  EXPECT_EQ(s1.m_shape, 3);
  EXPECT_EQ(d2.m_shape, _33);
  EXPECT_EQ(s2.m_shape, _33);

  EXPECT_EQ(sizeof(d0), sizeof(std::vector<std::size_t>));
  EXPECT_EQ(sizeof(d1), sizeof(std::size_t));
  EXPECT_EQ(sizeof(s1), 1);
  EXPECT_EQ(sizeof(d2), sizeof(std::array<std::size_t, 2>));
  EXPECT_EQ(sizeof(s2), 1);

}
TEST(DenseArrayTests, ShapeTypeTest) {
  using namespace torment::dense;

  // typedef array<int, 1, urr(3)> ivec3;

  std::string _size_tn = typeid(std::size_t).name();
  std::string _1_elem_base_tn = typeid(smart_container<std::size_t, 1>).name();
  std::string _2_elem_base_tn = typeid(smart_container<std::size_t, 2>).name();
  std::string _dynamic_base_tn = typeid(smart_container<std::size_t, 0>).name();

  std::string _ivec0_stn = typeid(ivec<0>::shape_array_type).name();
  std::string _ivec3_stn = typeid(ivec<3>::shape_array_type).name();
  std::string _imat00_stn = typeid(imat<0,0>::shape_array_type).name();
  std::string _imat33_stn = typeid(imat<3,3>::shape_array_type).name();
  std::string _itensor_stn = typeid(base<int>::shape_array_type).name();

  EXPECT_EQ(_ivec0_stn, _size_tn);
  EXPECT_EQ(_ivec3_stn, _size_tn);
  EXPECT_EQ(_imat00_stn, _2_elem_base_tn);
  EXPECT_EQ(_imat33_stn, _2_elem_base_tn);
  EXPECT_EQ(_itensor_stn, _dynamic_base_tn);
}
TEST(DenseArrayTests, ShapeValueTest) {
  using namespace torment::dense;
  // auto &mem = memory_usage::instance();
  // mem.reset();

  constexpr auto _33 = urr(3, 3);

  base<int, 2, _33> v_frfs = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1  }; // static array

  base<int, 2>::shape_array_type _24 = {2, 4};
  base<int, 2> v_frds(_24, 1);

  base<int>::shape_array_type _44 = {4, 4};
  base<int> v_heap(_44);

  // mem.print();


  // std::cout << v_frfs << "\n";
  // base<std::size_t, _33.size()> b33 = _33;
  // std::cout << typeid(array<int, 2>::shape_type).name() << "\n";

  EXPECT_EQ(_33, v_frfs.shape());
  EXPECT_EQ(_24, v_frds.shape());
  EXPECT_EQ(_44, v_heap.shape());
}
TEST(DenseArrayTests, StreamTest) {
  using namespace torment::dense;

  std::stringstream ss;
  std::string str;

  ivec<4> vec = {1, 2, 3, 4};
  imat<4, 4> mat = {
    1, 0, 0, 0,
    0, 2, 0, 0,
    0, 0, 3, 0,
    0, 0, 0, 4
  };

  ss.str("");
  ss << vec;
  str = "[1, 2, 3, 4]";
  EXPECT_EQ(ss.str(), str);

  ss.str("");
  ss << mat;
  str =
    "[[1, 0, 0, 0], \n"
    " [0, 2, 0, 0], \n"
    " [0, 0, 3, 0], \n"
    " [0, 0, 0, 4]]";
  EXPECT_EQ(ss.str(), str);

  base<int> tensor(smart_container<std::size_t>(4, 2));
  for(int counter = 0; auto &e: tensor)
    e = counter++ % 2;
  ss.str("");
  ss << tensor;
  str =
    "[[[[0, 1], \n"
    "   [0, 1]], \n"
    "  [[0, 1], \n"
    "   [0, 1]]], \n"
    " [[[0, 1], \n"
    "   [0, 1]], \n"
    "  [[0, 1], \n"
    "   [0, 1]]]]";
  EXPECT_EQ(ss.str(), str);
}

#pragma endregion ] DenseArrayTests

TEST(MixedRadixSystemTests, Subtract) {
  using namespace std;
  using namespace torment::dense;

  typedef std::uint64_t u64;

  mrns<u64, 4> num({3, 3, 3, 3});

  auto res = num;

  num = {0, 0, 0, 1};
  res = {2, 2, 2, 0};
  num -= 1;

  EXPECT_EQ(num, res);
}
TEST(MixedRadixSystemTests, SimpleTest) {
  using namespace std;
  using namespace torment::dense;

  typedef std::uint64_t u64;

  mrns<u64, 2> grid({3, 3});
  smart_container<u64, 4> grid_squared_size = {3, 3, 3, 3};

  // mrns<mrns<u64, 2>, 2> k({3, 3, 3, 3});
  mrns<mrns<u64, 2>, 2> k({grid, grid});

  // std::cout << "grid^2: " << *k.m_radices << "\n";
  EXPECT_EQ(*k.m_radices, grid_squared_size);

  for(auto i = k = 0; i < k.overflow(true); i++) {
    EXPECT_LT(i, k);
  }

    // std::cout << i << "\n";

  // while(k+1)
  // k = k + 1;
  // std::cout << k << "\n";
  // u2rad<std::size_t> grid({3, 3});
  // u2rad<u2rad<std::size_t>> k({grid, grid});
  //
  // for(auto i = k = {0, 0}; i < k.overflow(true); i++) {
  //   std::cout << i << "\n";
  // }
  //
  // typedef dense::array<std::uint16_t, 2> index_type;
  //
  // base<float, 2, index_type> m({}, 0);
  //
  // int cnt = 0;
  // for(auto it = m.begin(); it != m.end(); it++) {
  //   *it = cnt++;
  // }
  // v = { {{2, 0}, 1.0f},
  //       {{2, 1}, 2.0f},
  //       {{2, 2}, 3.0f} };
  //
  // std::cout << m << "\n";
  //
  // base<float> v(3, 0);
  // std::cout << v << "\n";
}

#pragma region SparseArrayTests [

TEST(SparseArrayTests, StreamTest) {

  using namespace torment;
  using namespace torment::sparse;

  typedef std::uint64_t u64;
  typedef radix::unsigned_mixed_system<std::size_t, 2> ums2;
  typedef radix::unsigned_mixed_system<std::size_t, 3> ums3;
  typedef radix::unsigned_mixed_system<std::size_t, 4> ums4;


  base<int, 1> b1(3);
  base<int, 2> b2({3, 3});
  base<int, 3> b3({3, 3, 3});
  base<int, 4> b4({2, 2, 2, 2});

  { int counter = 0;
    auto k = b1.shape();
    for(auto i = k = 0; i < k; i++)
      b1[i] = counter++;
  }

  { int counter = 0;
    ums2 k(b2.shape());
    for(auto i = k = 0; i < k.overflow(true); i++)
      b2[i] = counter++;
  }

  { int counter = 0;
    ums3 k(b3.shape());
    for(auto i = k = 0; i < k.overflow(true); i++)
      b3[i] = counter++;
  }

  { int counter = 0;
    ums4 k(b4.shape());
    for(auto i = k = 0; i < k.overflow(true); i++)
      b4[i] = counter++;
  }

  std::stringstream bx_ss;
  std::string
    b1_str =  "[0,  0,  0]",
    b2_str =  "[[ 0,  1,  2],\n"
              " [ 3,  4,  5],\n"
              " [ 6,  7,  8]]",
    b3_str =  "[[[ 0,  1,  2],\n"
              "  [ 3,  4,  5],\n"
              "  [ 6,  7,  8]],\n"
              " [[ 9, 10, 11],\n"
              "  [12, 13, 14],\n"
              "  [15, 16, 17]],\n"
              " [[18, 19, 20],\n"
              "  [21, 22, 23],\n"
              "  [24, 25, 26]]]",
    b4_str =  "[[[[ 0,  1],\n"
              "   [ 2,  3]],\n"
              "  [[ 4,  5],\n"
              "   [ 6,  7]]],\n"
              " [[[ 8,  9],\n"
              "   [10, 11]],\n"
              "  [[12, 13],\n"
              "   [14, 15]]]]";

  bx_ss.str("");
  bx_ss << std::setw(2) << b1;
  EXPECT_EQ(bx_ss.str(), b1_str);

  bx_ss.str("");
  bx_ss << std::setw(2) << b2;
  EXPECT_EQ(bx_ss.str(), b2_str);

  bx_ss.str("");
  bx_ss << std::setw(2) << b3;
  EXPECT_EQ(bx_ss.str(), b3_str);

  bx_ss.str("");
  bx_ss << std::setw(2) << b4;
  EXPECT_EQ(bx_ss.str(), b4_str);

}
TEST(SparseArrayTests, IteratorTest) {
  using namespace torment;
  using namespace torment::sparse;

  array<int, 4> v1({3, 3, 3, 3});

  int count = 0;

  for(auto it = v1.begin(); it != v1.end(); it++) {
    ASSERT_LT(count++, v1.n_elem());
  }

  EXPECT_EQ(count, v1.n_elem());
}
TEST(SparseArrayTests, ElementReferenceTest) {
  using namespace torment;
  using namespace torment::sparse;

  array<int, 4> v1({3,3,3,3});

  int count = 0;
  for(auto it = v1.begin(); it != v1.end(); it++) {
    ASSERT_LT(count, v1.n_elem());
    *it = count % 2 == 0 ? count : 0;
    count++;
  }
  EXPECT_EQ(v1.size(), 40);

  std::string str =   "[[[[ 0,  0,  2],\n"
                      "   [ 0,  4,  0],\n"
                      "   [ 6,  0,  8]],\n"
                      "  [[ 0, 10,  0],\n"
                      "   [12,  0, 14],\n"
                      "   [ 0, 16,  0]],\n"
                      "  [[18,  0, 20],\n"
                      "   [ 0, 22,  0],\n"
                      "   [24,  0, 26]]],\n"
                      " [[[ 0, 28,  0],\n"
                      "   [30,  0, 32],\n"
                      "   [ 0, 34,  0]],\n"
                      "  [[36,  0, 38],\n"
                      "   [ 0, 40,  0],\n"
                      "   [42,  0, 44]],\n"
                      "  [[ 0, 46,  0],\n"
                      "   [48,  0, 50],\n"
                      "   [ 0, 52,  0]]],\n"
                      " [[[54,  0, 56],\n"
                      "   [ 0, 58,  0],\n"
                      "   [60,  0, 62]],\n"
                      "  [[ 0, 64,  0],\n"
                      "   [66,  0, 68],\n"
                      "   [ 0, 70,  0]],\n"
                      "  [[72,  0, 74],\n"
                      "   [ 0, 76,  0],\n"
                      "   [78,  0, 80]]]]";
  std::stringstream ss;
  ss << std::setw(2) << v1;

  EXPECT_EQ(ss.str(), str);
}
TEST(SparseArrayTests, RangedForLoopTest) {
  using namespace torment;
  using namespace torment::sparse;

  typedef std::uint64_t u64;

  array<int, 4> v1({3, 3, 3, 3});

  for(int cnt = 0; auto e : v1)
    e = cnt++;

  PRETTY_PRINT(v1, 2);
}

TEST(SparseArrayTests, Dummy) {
  typedef std::uint64_t u64;

  using namespace torment;
  using namespace torment::sparse;

  auto &cout = std::cout;

  mrns<u64, 2> grid({3, 3});

  // cout << "grid: " << *grid.m_radices << "\n";
  PRINTLN(*grid.m_radices);

  // array<int, 2, mrns<u64, 2>>::base_type::index_type idx({1, 2});
  array<int, 2, dense::smart_container<u64, 2>> v1({*grid.m_radices, *grid.m_radices}, 0);



  // decltype(v1)::index_type idx;
  // decltype(v1)::key_type idk;

  // cout << "typeid(v1.m_dims): " << typeid(v1.m_dims).name() << "\n";
  // cout << "v1.m_dims: " << v1.shape() << "\n";
  // PRINTLN(v1.shape());

  // cout << v1[{{0,0}, {0,0}}] << "\n";

  // array<int, 2, mrns<u64, 2>> v1(k, 0);
  // array<int, 2, mrns<u64, 2>>::base_type::index_type idx;
  // array<int, 2, mrns<u64, 2>>::base_type::key_type key;

            //  cout << v1 << "\n"; //this should print a 9x9 grid
  //                  ^
  //                  |
  //                  |
  // this -+ is that -+
  //       |
  //       |
  //       v
  //

  // auto width = cout.width();

  // // if(std::is_integral_v<decltype(v1)::index_type>)
  // mrns<mrns<u64, 2>, 2> k({grid, grid});

  // // cout << "k.size(): " << k.size() << "\n";
  // PRINTLN(k.size());

  // cout << std::setfill('[') << std::setw(2) << ""
  //           << std::setfill(' ');
  // for(auto i = k = 0; i < k.overflow(true); i++) {
  //   auto j = i-1;

  //   for(std::size_t n = k.size() - 1; n < k.size(); n--) {
  //     // std::cout << (j[n] < i[n] ? left_delimeters[n] : "");
  //     if(j[n] < i[n]) {
  //       if(n != 0) cout
  //         << std::setfill(' ') << std::setw(k.size()-n) << ""
  //         << std::setfill('[') << std::setw(n) << ""
  //         << std::setfill(' ');
  //       break;
  //     }
  //   }

  //   cout << 1;
  //   // cout << std::setw(width) << v1[i];

  //   j = i+1;
  //   for(std::size_t n = 0; n < k.size(); n++) {
  //     // std::cout << (i[n] < j[n] ? right_delimeters[n] : "");

  //     if(i[n] < j[n]) {
  //       if(n == 0)
  //         cout << ", ";
  //       else cout
  //         << std::setfill(']') << std::setw(n+2) << ",\n"
  //         << std::setfill(' ');
  //     }
  //   }
  // };

  // cout << std::setfill(']') << std::setw(2) << ""
  //           << std::setfill(' ');

  // cout << "\n";
  FAIL();
}

#pragma endregion ] SparseArrayTests