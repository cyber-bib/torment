#include <array>
#include <cstddef>
#include <sstream>
#include <boost/preprocessor.hpp>

#pragma region include-headers [

#include <boost/preprocessor.hpp>

// #include <memory>
// #include <numeric>
// #include <functional>
#include <iostream>

#include "radix/mix.txx"
#include "array/dense/array.txx"
#include "array/sparse/array.txx"

#include "vector/dense/core.txx"

#include "gtest/gtest.h"

#pragma endregion ] include-headers

#pragma region debug-macros [

#define INIT_CODEVARS                                                           \
  char const  * const __file__ = __FILE__,                                      \
              * const ___fn___ = __func__,                                      \
              * const __line__ = "~" BOOST_PP_STRINGIZE(__LINE__),              \
              * const __date__ = __DATE__,                                      \
              * const __time__ = __TIME__

#define CODEVARS __file__, ___fn___, __line__, __date__, __time__

#define PRINT(os_code) #os_code ": " << os_code
#define PRINTLN(os_code) PRINT(os_code) << "\n"

#define PRINTLN_MACRO(r, n, i, elem)                                            \
  PRINTLN(elem)                                                                 \
  BOOST_PP_IF(BOOST_PP_EQUAL(i, BOOST_PP_DEC(n)), , <<)

#define PRINT_SEQ(seq)                                                          \
  BOOST_PP_SEQ_FOR_EACH_I( PRINTLN_MACRO, BOOST_PP_SEQ_SIZE(seq), seq  )

#define PRINTVARS(...) PRINT_SEQ(BOOST_PP_TUPLE_TO_SEQ((__VA_ARGS__)))

#define PRINT_CODEVARS(os)                                                      \
  INIT_CODEVARS;                                                                \
  os << PRINTVARS(CODEVARS)


#define PRETTY_PRINT_1(os_code)                                                 \
  std::cout << #os_code ":\n" << os_code << "\n"
#define PRETTY_PRINT_2(os_code, os_setw)                                        \
  std::cout << #os_code ":\n" << std::setw(os_setw) << os_code << "\n"

#if !BOOST_PP_VARIADICS_MSVC
# define  PRETTY_PRINT(...)                                                     \
            BOOST_PP_OVERLOAD(PRETTY_PRINT_,__VA_ARGS__)(__VA_ARGS__)
#else
# define  MACRO_ADD_NUMBERS(...)                                                \
            BOOST_PP_CAT( BOOST_PP_OVERLOAD(PRETTY_PRINT_,__VA_ARGS__)          \
                          (__VA_ARGS__),                                        \
                          BOOST_PP_EMPTY()  )
#endif

#pragma endregion ] debug-macros

#pragma region forward-decls [

// template<class T>
// using dense = torment::dense::smart_container<T, 0>;

typedef std::uint8_t  u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

typedef std::int8_t  i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;

constexpr u8 operator "" _u8(unsigned long long val) {
  return val;
};
constexpr u16 operator "" _u16(unsigned long long val) {
  return val;
};
constexpr u32 operator "" _u32(unsigned long long val) {
  return val;
};
constexpr u64 operator "" _u64(unsigned long long val) {
  return val;
};

template< class T,
          std::size_t N = 0,
          std::array<std::uint64_t, N> Sp = std::array<std::uint64_t, N>{}>
using base_u64 = torment::dense::base<T, N, std::uint64_t, Sp>;

template<class T, std::size_t N = 0>
using vec = base_u64<T, 1, torment::dense::urr(N)>;

template<std::size_t N = 0>
using ivec = vec<int, N>;

template<std::uint64_t R, std::uint64_t C>
using imat = base_u64<int, 2, torment::dense::urr(R, C)>;

template<class K>
using iXvec = torment::sparse::array<int, 1, K>;

template<class K = std::size_t>
using iXmat = torment::sparse::array<int, 2, K>;

template<class T, std::size_t N = 0>
using mrns = torment::radix::unsigned_mixed_system<T, N>;

template<std::size_t N = 0>
using size_mrns = torment::radix::unsigned_mixed_system<std::size_t, N>;

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

TEST(DenseArrayCoreTests, ShapeStructTest) {
  using namespace torment::dense;

  constexpr auto _ = std::array<std::size_t, 0>{};
  constexpr auto _0 = urr(0ULL);
  constexpr auto _3 = urr(3ULL);
  constexpr auto _00 = urr(0ULL, 0ULL);
  constexpr auto _33 = urr(3ULL, 3ULL);
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
TEST(DenseArrayCoreTests, ShapeTypeTest) {
  using namespace torment::dense;

  // typedef array<int, 1, urr(3)> ivec3;

  std::string _size_tn = typeid(std::size_t).name();
  std::string _1_elem_base_tn = typeid(smart_container<std::size_t, 1>).name();
  std::string _2_elem_base_tn = typeid(smart_container<std::size_t, 2>).name();
  std::string _dynamic_base_tn = typeid(smart_container<std::size_t, 0>).name();

  std::string _ivec0_stn = typeid(ivec<0>::index_type).name();
  std::string _ivec3_stn = typeid(ivec<3>::index_type).name();
  std::string _imat00_stn = typeid(imat<0,0>::index_type).name();
  std::string _imat33_stn = typeid(imat<3,3>::index_type).name();
  std::string _itensor_stn = typeid(base<int>::index_type).name();

  EXPECT_EQ(_ivec0_stn, _size_tn);
  EXPECT_EQ(_ivec3_stn, _size_tn);
  EXPECT_EQ(_imat00_stn, _2_elem_base_tn);
  EXPECT_EQ(_imat33_stn, _2_elem_base_tn);
  EXPECT_EQ(_itensor_stn, _dynamic_base_tn);
}
TEST(DenseArrayCoreTests, ShapeValueTest) {
  using namespace torment::dense;
  // auto &mem = memory_usage::instance();
  // mem.reset();

  constexpr auto _33 = urr(3ULL, 3ULL);

  base_u64<int, 2, _33> v_frfs = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1  }; // static array

  base_u64<int, 2>::index_type _24 = {2, 4};
  base_u64<int, 2> v_frds(_24, 1);

  base_u64<int>::index_type _44 = {4, 4};
  base_u64<int> v_heap(_44);


  // mem.print();


  // std::cout << v_frfs << "\n";
  // base<std::size_t, _33.size()> b33 = _33;
  // std::cout << typeid(array<int, 2>::shape_type).name() << "\n";

  EXPECT_EQ(_33, v_frfs.shape());
  EXPECT_EQ(_24, v_frds.shape());
  EXPECT_EQ(_44, v_heap.shape());
}
TEST(DenseArrayCoreTests, StreamTest) {
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
TEST(DenseArrayCoreTests, RangedForLoopTest) {
  typedef std::uint64_t u64;
  using namespace torment::dense;

  std::stringstream ss;
  std::string str;

  base_u64<int, 3, urr(5ULL, 3ULL, 2ULL)> T;

  for(u64 cnt = 0; auto &e : T) e = cnt++;

  ss << std::setw(3) << T;
  str = "[[[  0,   1,   2,   3,   4], \n"
        "  [  5,   6,   7,   8,   9], \n"
        "  [ 10,  11,  12,  13,  14]], \n"
        " [[ 15,  16,  17,  18,  19], \n"
        "  [ 20,  21,  22,  23,  24], \n"
        "  [ 25,  26,  27,  28,  29]]]";

  EXPECT_EQ(ss.str(), str);
  // std::cout << "T:\n" << std::setw(3) << T << "\n";
}
TEST(DenseArrayCoreTests, SubscriptAccessTest) {
  typedef std::uint64_t u64;
  using namespace torment::dense;

  constexpr auto shape = urr(5ULL, 3ULL, 2ULL);
  base_u64<int, shape.size(), shape> T;
  T.fill(0);

  T[{1, 0 , 0}] = 10;
  T[{0, 1 , 0}] = 20;
  T[{0, 0 , 1}] = 30;

  EXPECT_EQ(T[1], 10);
  EXPECT_EQ(T[5], 20);
  EXPECT_EQ(T[15], 30);

  // std::cout << "T:\n" << std::setw(4) << T << "\n";
}
TEST(DenseArrayCoreTests, LowRankInitialization) {
  using namespace torment::dense;

  typedef base<int, 1, u16> u16vec;


  base<int, 1, u16> v(4, 1);
  u16vec t = {1, 1, 1, 1};

  EXPECT_EQ(t.shape(), 4);
  EXPECT_EQ(v, t);

  v.assign(3, 2);
  t = {2, 2, 2};

  EXPECT_EQ(t.shape(), 3);
  EXPECT_EQ(v, t);

  base<int, 2, u16> m({2, 2}, 1);
  base<int, 2, u16> w({2, 2}, 0);

  w = { 1, 1,
        1 ,1 };


  EXPECT_EQ(w.shape(), urr(2_u16, 2_u16));
  EXPECT_EQ(m, w);

  // base<int, 2, u16, urr(2_u16, 2_u16)> w = {1, 1, 1, 1};
  // std::cout << "w:\t" << w << "\n";

  // w = { 1, 1,
  //       1, 1  };
  // m.assign({3, 3}, 2);
  // w = { 2, 2, 2,
  //       2, 2, 2,
  //       2, 2, 2 };

  // EXPECT_EQ(w33.shape(), _3x3_);
  // EXPECT_EQ(m, w33);

  // std::cout << "v.shape():  " << v.shape() << "\n";
  // std::cout << "v: " << v << "\n";
  // std::cout << "m.shape():  " << m.shape() << "\n";
  // std::cout << "m: " << m << "\n";
}
TEST(DenseArrayProxyTests, StaticOneToOneTest) {
  using namespace torment::dense;

  constexpr auto _4x4_ = urr(4_u16, 4_u16);
  base<int, 2, u16, _4x4_> a;
  for(int i = 0; auto &e: a) e = i++;

  proxy<int, 2, u16, _4x4_, 2, _4x4_> p(a);
  for(u8 i = 0; i < 4; i++) {
    for(u8 j = 0; j < 4; j++) {
      p({i, j}) = {i, j};
    }
  }

  std::stringstream as;
  std::stringstream ps;
  as << std::setw(2) << a;
  ps << std::setw(2) << p;

  EXPECT_EQ(as.str(), ps.str());

  for(int i = 0; auto &e: a) e = 1 + i++;

  as.str("");
  ps.str("");

  as << std::setw(2) << a;
  ps << std::setw(2) << p;

  EXPECT_EQ(as.str(), ps.str());

  // std::cout << std::setw(2) << a << "\n";
  // std::cout << std::setw(2) << a << "\n";
}

TEST(DenseArrayProxyTests, ProjectionsTest) {
  using namespace torment::dense;

  constexpr auto _3x3_ = urr(3_u16, 3_u16);
  constexpr auto _3v3_ = urr(3_u16, 3_u16, 3_u16);

  typedef base<   int,
                  _3v3_.size(),
                  decltype(_3v3_)::value_type,
                  _3v3_                        >                        itsr163;
  typedef proxy<  int,
                  _3v3_.size(),
                  decltype(_3v3_)::value_type,
                  _3v3_,
                  _3x3_.size(),
                  _3x3_                        >                      ipxy163_2;

  itsr163 a;
  for(int i = 0; auto &e: a) e = 0;

  // ipxy163_2 p(a);
  // for(u16 i = 0; i < 3; i++) {
  //   for(u16 j = 0; j < 3; j++) {
  //     p({i, j}) = {i, j, 1};
  //   }
  // }
  // auto p = make_slice<2>(a, 1_u16);

  // auto p = make_slice<1>(a, 2_u16);
  // for(int i = 0; auto &e: p) e = 0;

  auto p = make_view< int,
                      _3v3_.size(),
                      decltype(_3v3_)::value_type,
                      _3v3_,
                      1,
                      urr(0_u16),
                      2,
                      urr(1_u16, 2_u16)>
                                                      (a, {0_u16, 0_u16});

  for(int i = 0; auto &e: p) e = 1;


  // proxy<int, 3, u16, _3v3_, 2, _3x3_> p(a);
  // proxy<int, 2, u16, urr(4_u16, 4_u16), > p(a);

  std::cout << std::setw(2) << a << "\n\n";
  // std::cout << std::setw(2) << p << "\n";

}

// TEST(DenseArrayTests, MultiSubscriptAcessTest) {
// }

// TEST(DenseArrayTests, RingOperators) {
//   using namespace torment::dense;

//   auto &cout = std::cout;

//   // array<float, 3> a;
//   //

//   array<float, 2, urr(3, 3)>
//     a = { 1, 0, 1,
//           0, 1, 0,
//           1, 0, 1  },
//     b = { 3, 0, 3,
//           0, 3, 0,
//           3, 0, 3  };
//     // b = { 0, 3, 0,
//     //       3, 0, 3,
//     //       0, 3, 0  };

//     a*=3;

//   EXPECT_EQ(a, b);
//   // cout << PRINTVARS(a);
// }

TEST(DenseArrayTests, RingOperators) {
}
#pragma endregion ] DenseArrayTests
#pragma region DenseVectorTests [

// TEST(DenseVectorTests, RingOperators) {
//   using namespace torment::dense;
//
//   vector<double, 3> v1 = {1, 2, 3},
//                     v2 = {4, 5, 6};
//
//
// }


#pragma endregion ] DenseVectorTests
#pragma region MixedRadixSystemTests [

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
TEST(MixedRadixSystemTests, RangedForLoop_1DTest) {
  using namespace std;
  using namespace torment::dense;

  typedef std::uint64_t u64;

  mrns<u64, 4> num({3, 4, 6, 8});

  num = {2, 3, 5, 7};

  std::size_t tot = 1, rst = 2*3*5*7;
  for(auto e: num) {
    tot *= e;
  }

  EXPECT_EQ(tot, rst);
}
TEST(MixedRadixSystemTests, RangedForLoop_2DTest) {
  using namespace std;
  using namespace torment::dense;

  typedef std::uint64_t u64;

  mrns<mrns<u64, 2>, 2> num({3, 4, 6, 8});

  num = {2, 3, 5, 7};

  std::size_t tot = 1, rst = 2*3*5*7;
  for(auto e: num) {
    tot *= e;
  }

  EXPECT_EQ(tot, rst);
}
TEST(MixedRadixSystemTests, RangedForLoop_3DTest) {
  using namespace std;
  using namespace torment::dense;

  typedef std::uint64_t u64;

  auto &cout = std::cout;

  mrns<mrns<mrns<u64, 2>, 2>, 2> num({
    3, 4, 3, 12,
    8, 6, 4, 6  });

  num = {
    2, 3, 2, 11,
    7, 5, 3, 5  };

  std::size_t tot = 1, rst = 2*3*2*11*7*5*3*5;
  for(auto e: num) {
    tot *= e;
  }

  EXPECT_EQ(tot, rst);
}
TEST(MixedRadixSystemTests, DecayTest) {
  using namespace std;
  using namespace torment::dense;

  typedef std::uint64_t u64;

  mrns<mrns<u64,2>,2> num({3, 3, 3, 3});

  auto res = num;

  num = {0, 0, 0, 0};

  stringstream ss;
  ss << num.decay_cast();

  string str = "[ [ 0, 0 ], [ 0, 0 ] ]";
  EXPECT_EQ(ss.str(), str);
  // ss << static_cast<decltype(num)::decay_type>(num);
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

#pragma endregion ] MixedRadixSystemTests

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

  for(int cnt = 0; auto e : v1) {
    e = cnt % 2 == 0 ? cnt : 0;
    cnt++;
  }

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
TEST(SparseArrayTests, MixedRadixNumberSystemTest) {
  typedef std::uint64_t u64;
  typedef mrns<u64, 2> index_type;

  index_type R({4, 6});
  index_type C({8, 3});

  R = {3, 5};
  C = {7, 2};


  iXmat<index_type> mat({R, C});

  auto shape = mat.shape();

  std::cout << PRINTVARS(
    R.size(),
    C.size(),
    mat.shape(),
    mat.size()
  );
  // PRINTLN(R.size());
  // PRINTLN(C.size());

  // PRINTLN(mat.shape());
  // PRINTLN(mat.size());
}
TEST(SparseArrayTests, Dummy) {
  typedef std::uint64_t u64;

  using namespace torment;
  using namespace torment::sparse;

  auto &cout = std::cout;

  mrns<u64, 2> grid({2, 2});

  // cout << "grid: " << *grid.m_radices << "\n";
  // PRINTLN(*grid.m_radices);

  // array<int, 2, mrns<u64, 2>>::base_type::index_type idx({1, 2});
  // array<int, 2, dense::smart_container<u64, 2>> v1({*grid.m_radices, *grid.m_radices}, 0);



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

  auto width = cout.width();

  // if(std::is_integral_v<decltype(v1)::index_type>)
  mrns<mrns<u64, 2>, 2> k({grid, grid});

  // cout << "k.size(): " << k.size() << "\n";
  // PRINTLN(k.size());


  // cout << std::setfill('[') << std::setw(4) << ""
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

  // cout << std::setfill(']') << std::setw(4) << ""
  //           << std::setfill(' ');

  // cout << "\n";
  FAIL();
}

#pragma endregion ] SparseArrayTests
