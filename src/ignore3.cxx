#include <array>
#include <stdexcept>
#include <iostream>
#include <boost/operators.hpp>

struct mix_radix_system;
std::ostream& operator<<(std::ostream &os, mix_radix_system const& num);

struct mix_radix_system
: std::array<std::size_t, 4>,
  boost::ordered_euclidean_ring_operators<mix_radix_system>,
  boost::ordered_euclidean_ring_operators<mix_radix_system, std::int64_t>
{
  static std::size_t const Sz = 4;
  static std::array<std::size_t, 4> const radices;

  mix_radix_system(int val)
  : std::array<std::size_t, 4>{0, 0, 0, 0}
  {
    (*this) += val;
  }

  std::size_t total_radix() {
    std::size_t dst = 1;         // this may not be enough
    for (std::size_t i = 0; i < radices.size(); ++i)
        dst *= (radices)[i];

    return dst;
  }
  void add(std::int64_t const &val, std::size_t i = 0) {
    std::size_t trad = total_radix(),
                carry = std::abs(val)%trad;

    i %= radices.size();
    carry = val >= 0 ? carry : (trad - carry);

    while (carry != 0) {
        auto base = radices[i];
        auto sum = (*this)[i] + carry;

        if(sum <  (*this)[i]) throw std::logic_error("unhandled overflow detected");

        (*this)[i] = sum % base;
        carry = sum / base;

        i = (i + 1) % radices.size(); // wrap around
    }
  }
  void propagate_carry() {
    for(std::size_t i = 0; i < radices.size(); i++) {
      auto tmp = (*this)[i];
      (*this)[i] = 0;
      this->add(tmp, i);
    }
  }

  void mul(std::int64_t const &val, std::size_t j = 0) {
    j %= radices.size();

    for(auto &e: *this) e *= val;
    this->propagate_carry();

    for(std::size_t i = 0; i < j; i++) {
      for(auto &e: *this) e *= radices[i];
      this->propagate_carry();
    }
  }

  mix_radix_system& operator+=(std::int64_t const &val) {
    this->add(val);

    return *this;
  }
  mix_radix_system& operator-=(std::int64_t const &val) {
    (*this) += -val;
    return *this;
  }

  mix_radix_system& operator+=(mix_radix_system const &rhs) {

    for(std::size_t i = 0; i < radices.size(); i++) {
      this->add(rhs[i], i);
    }

    return *this;
  }

  mix_radix_system& operator*=(std::int64_t const &val) {
    this->mul(val);
    return *this;
  }

  mix_radix_system& operator*=(mix_radix_system const &rhs) {
    auto lhs = *this;
    *this = 0;

    for(std::size_t i = 0; i < radices.size(); i++) {
      auto tmp = lhs;
      tmp.mul(rhs[i], i);
      (*this) += tmp;
    }

    return *this;
  }
  std::size_t& operator[](std::size_t idx) {
    auto &ref = static_cast<std::array<std::size_t, 4>&>(*this);
    return ref[idx%ref.size()];
  }
  std::size_t const& operator[](std::size_t idx) const {
    auto &ref = static_cast<std::array<std::size_t, 4> const&>(*this);
    return ref[idx%ref.size()];
  }
};

std::ostream& operator<<(std::ostream &os, mix_radix_system const& num) {
  os << "[" << num[0];
  for(auto i = 1; i < num.size(); ++i)
    os << ", " << num[i];
  os << "]";
  return os;
}
std::array<std::size_t, 4> const mix_radix_system::radices = {3, 5, 7, 11};

int main(int argc, char *argv[])
{
  mix_radix_system b = 49, c = 8, rst = 49*8;

  auto _b52 = b * c;
  auto _52b = c * b;

  std::cout << "b * c = " << _b52 << "\n";
  std::cout << "c * b = " << _52b << "\n";
  std::cout << "\n";
  std::cout << "b = " << b << "\n";
  std::cout << "c = " << c << "\n";
  std::cout << "rst = " << rst << "\n";

  return 0;
}