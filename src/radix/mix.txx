#pragma once

#include "mix.hxx"
#include "array/dense/array.txx"

#include <algorithm>

namespace torment {

namespace radix {

#pragma region [con/de]strutors {

template<class T, std::size_t Sz>
unsigned_mixed_system<T,Sz>::unsigned_mixed_system()
: m_radices(nullptr) {}

template<class T, std::size_t Sz>
unsigned_mixed_system<T,Sz>::unsigned_mixed_system(
  list_type const &bases  )
: m_radices(std::make_shared<base_type>(bases))
{
  this->init();
}

template<class T, std::size_t Sz>
unsigned_mixed_system<T,Sz>::unsigned_mixed_system(base_type const &bases)
: m_radices(std::make_shared<base_type>(bases))
{
  this->init();
}


template<class T, std::size_t Sz>
unsigned_mixed_system<T,Sz>::unsigned_mixed_system(
  std::shared_ptr<base_type> bases )
: m_radices(bases)
{
  this->init();
}

// template<class T, std::size_t Sz>
// unsigned_mixed_system<T,Sz>::unsigned_mixed_system(
//   unsigned_mixed_system const &other  )
// : base_type(other), m_radices(other.m_radices)
// {
//   // this->init();
//   // static_cast<base_type&>(*this) = other;
// }

template<class T, std::size_t Sz>
void unsigned_mixed_system<T,Sz>::init() {
  if(!this->m_radices) return;
  // if(!this->m_radices) throw std::logic_error("radices where not initialized!");

  for(auto &e: *this->m_radices) e = e == 0 ? (e-1) : e;
    // throw std::logic_error("one or more radix is set to zero");

  if constexpr(Sz == 0) {
    this->assign(this->m_radices->size(), 0);
  }
}

#pragma endregion } [con/de]strutors

  template<class T, std::size_t Sz>
  unsigned_mixed_system<T, Sz>&
  unsigned_mixed_system<T, Sz>::operator=(value_type const& val) {
    for(auto &e: *this) e = 0;

    (*this) += val;

    return *this;
  }

  template<class T, std::size_t Sz>
  unsigned_mixed_system<T, Sz>&
  unsigned_mixed_system<T, Sz>::operator=(list_type const &list)
  {
    this->base_type::operator=(list);
    return *this;
  }

  template<class T, std::size_t Sz> bool
  unsigned_mixed_system<T, Sz>::overflow() const {
    return this->m_overflow;
  }
  template<class T, std::size_t Sz>
  unsigned_mixed_system<T, Sz>&
  unsigned_mixed_system<T, Sz>::overflow(bool set_unset) {
    this->m_overflow = set_unset;
    return *this;
  }
  // template<class T, std::size_t Sz>
  // unsigned_mixed_system<T, Sz>&
  // unsigned_mixed_system<T, Sz>::operator=(base_type const &arg) {
  //   this->base_type::operator=(arg);
  //   return *this;
  // }

  template<class T, std::size_t Sz>
  typename unsigned_mixed_system<T,Sz>::base_type
  unsigned_mixed_system<T,Sz>::radices() const {
    base_type dst = *this->m_radices;
    return dst;
  }
  template<class T, std::size_t Sz> void
  unsigned_mixed_system<T,Sz>::radices(base_type const& radices) {
    this->m_radices = std::make_shared<base_type>(radices);

    if constexpr(Sz == 0) {
      this->assign(this->m_radices->size(), 0);
    } else {
      for(auto &e: *this) e = 0;
    }
  }
  template<class T, std::size_t Sz>
  std::size_t unsigned_mixed_system<T,Sz>::global_radix() const {
    std::size_t dst = 1;         // this may not be enough

    for(auto &e : *this->m_radices)
        dst *= e;

    dst;

    return dst;
  }


  template<class T, std::size_t Sz> std::size_t
  unsigned_mixed_system<T,Sz>::add(
    std::size_t const &val,
    std::size_t i
  ) {
    // std:size_t  trad = this->global_radix(),
    //             carry = std::abs(val)%trad;
    std::size_t carry = val;

    i %= this->m_radices->size();
    // carry = val >= 0 ? carry : (trad - carry);

    for(; i < this->size(); i++) {
        auto base = (*this->m_radices)[i];
        auto sum = (*this)[i] + carry;

        if(sum <  (*this)[i]) throw std::logic_error("unhandled overflow detected");

        (*this)[i] = sum % base;
        carry = sum / base;
        if(carry == 0) break;
    }

    // this->m_overflow |= carry != 0;
    return carry;
  }

  template<class T, std::size_t Sz>
  unsigned_mixed_system<T, Sz>&
  unsigned_mixed_system<T,Sz>::operator++() {
    this->add(1, 0);
    return *this;
  }

  template<class T, std::size_t Sz>
  unsigned_mixed_system<T, Sz>&
  unsigned_mixed_system<T,Sz>::operator--() {
    this->add(-1, 0);
    return *this;
  }

  template<class T, std::size_t Sz>
  unsigned_mixed_system<T,Sz>&
  unsigned_mixed_system<T,Sz>::operator+=(std::size_t carry) {
    // static_assert(std::is_integral_v<T>, "T is not an integral type" );
    // static_assert(std::is_unsigned_v<T>, "T is not an unsigned integral type");

    // if consexpr(std::is_integral_v<T>)
      this->add(carry, 0);
    // else
    //   for(auto& )
    return *this;
  }
  template<class T, std::size_t Sz>
  unsigned_mixed_system<T,Sz>&
  unsigned_mixed_system<T,Sz>::operator-=(std::size_t carry) {
    this->add(-carry, 0);
    return *this;
  }


  template<class T, std::size_t Sz> bool
  unsigned_mixed_system<T,Sz>::operator<(
    value_type const &val
  ) const {
    unsigned_mixed_system<T,Sz> tmp = *this;
    tmp = val;

    return (*this < tmp);
  }
  template<class T, std::size_t Sz> bool
  unsigned_mixed_system<T,Sz>::operator>(
    value_type const &val
  ) const {
    unsigned_mixed_system<T,Sz> tmp = *this;
    tmp = val;

    return (*this > tmp);
  }

  // template<class T, std::size_t Sz> bool
  // unsigned_mixed_system<T,Sz>::operator<(
  //   unsigned_mixed_system const &rhs
  // ) const {
  //   auto lhs = static_cast<base_type const &>(*this);

  //   auto lt = this->m_overflow < rhs.m_overflow;
  //   auto gt = this->m_overflow > rhs.m_overflow;

  //   return lt ? lt : ( gt ? !gt : lhs < rhs);
  // }

} // namespace radix

} // namespace torment
