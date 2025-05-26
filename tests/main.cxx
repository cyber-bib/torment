#include <memory>
#include <numeric>
#include <functional>
#include <iostream>

#include "array/sparse/array.txx"
#include "array/dense/array.txx"
#include "gtest/gtest.h"

#pragma region forward-decls {{{


// template<class T>
// using dense = torment::dense::base<T, 0>;

template<std::size_t N>
using ivec = torment::dense::array<int, 1, torment::dense::urr(N)>;

template<std::size_t R, std::size_t C>
using imat = torment::dense::array<int, 2, torment::dense::urr(R, C)>;

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

#pragma endregion }}} forward-decls
#pragma region DenseArrayTests {{{

TEST(DenseBaseTests, ShapeStructTest) {
  using namespace torment::dense;

}
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
  std::string _1_elem_base_tn = typeid(base<std::size_t, 1>).name();
  std::string _2_elem_base_tn = typeid(base<std::size_t, 2>).name();
  std::string _dynamic_base_tn = typeid(base<std::size_t, 0>).name();

  std::string _ivec0_stn = typeid(ivec<0>::shape_array_type).name();
  std::string _ivec3_stn = typeid(ivec<3>::shape_array_type).name();
  std::string _imat00_stn = typeid(imat<0,0>::shape_array_type).name();
  std::string _imat33_stn = typeid(imat<3,3>::shape_array_type).name();
  std::string _itensor_stn = typeid(array<int>::shape_array_type).name();

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

  array<int, 2, _33> v_frfs = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1  }; // static array

  array<int, 2>::shape_array_type _24 = {2, 4};
  array<int, 2> v_frds(_24, 1);

  array<int>::shape_array_type _44 = {4, 4};
  array<int> v_heap(_44);

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

  array<int> tensor(base<std::size_t>(4, 2));
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

TEST(SparseArrayTests, Dummy) {
  // using namespace torment::dense;

}

#pragma endregion }}} DenseArrayTests
