#pragma once

#include "mix.hxx"
#include "array/dense/array.txx"

#include <algorithm>

namespace torment {
  namespace radix {

    // template<class T, std::size_t Sz>
    // unsigned_mixed_system<T,Sz>::unsigned_mixed_system(base_type const &bases)
    // : m_bases(std::make_shared<base_type>(bases))
    // {
    //   this->init();
    // }

    template<class T, std::size_t Sz>
    unsigned_mixed_system<T,Sz>::unsigned_mixed_system(list_type const &bases)
    : m_bases(std::make_shared<base_type>(bases))
    {
      this->init();
    }

    template<class T, std::size_t Sz>
    unsigned_mixed_system<T,Sz>::unsigned_mixed_system(std::shared_ptr<base_type const> bases)
    : m_bases(bases)
    {
      this->init();
    }
      // unsigned_mixed_system(list_type const &bases);
    template<class T, std::size_t Sz>
    void unsigned_mixed_system<T,Sz>::init() {
      if(!this->m_bases) return;
      // if(!this->m_bases) throw std::logic_error("radices where not initialized!");

      for(auto const &e: *this->m_bases) if(e == 0)
        throw std::logic_error("one or more radix is set to zero");


      if constexpr(Sz == 0) {
        this->resize(this->m_bases->size());
      }
    }

    template<class T, std::size_t Sz>
    unsigned_mixed_system<T, Sz>&
    unsigned_mixed_system<T, Sz>::operator=(value_type const& val) {
      for(auto &e: *this) e = 0;

      (*this) += val;

      return *this;
    }

    template<class T, std::size_t Sz>
    unsigned_mixed_system<T, Sz>&
    unsigned_mixed_system<T, Sz>::operator=(list_type const &bases)
    {
      this->base_type::operator=(bases);

      return *this;
    }

    template<class T, std::size_t Sz>
    std::size_t unsigned_mixed_system<T,Sz>::maxval() const {
      std::size_t total_base = 1;         // this may not be enough

      for (std::size_t i = 0; i < Sz; ++i)
          total_base *= (*this->m_bases)[i];
      total_base--;

      return total_base;
    }

    template<class T, std::size_t Sz>
    typename unsigned_mixed_system<T,Sz>::base_type
    unsigned_mixed_system<T,Sz>::bases() const {
      base_type dst = *this->m_bases;

      return dst;
    }

    template<class T, std::size_t Sz>
    unsigned_mixed_system<T,Sz>&
    unsigned_mixed_system<T,Sz>::operator+=(value_type carry) {

      std::size_t total_base = 1;         // this may not be enough
      for (std::size_t i = 0; i < Sz; ++i)
          total_base *= (*this->m_bases)[i];
      carry %= total_base;


      std::size_t i = 0;
      while (carry != 0) {
          auto base = (*this->m_bases)[i];
          auto sum = (*this)[i] + carry;

          if(sum <  (*this)[i]) throw std::logic_error("unhandled overflow detected");

          (*this)[i] = sum % base;
          carry = sum / base;

          i = (i + 1) % Sz; // wrap around
      }

      return *this;
    }

    template<class T, std::size_t Sz>
    unsigned_mixed_system<T, Sz>&
    unsigned_mixed_system<T,Sz>::operator++() {
      this->operator+=(1);

      return *this;
    }

    template<class T, std::size_t Sz>
    bool unsigned_mixed_system<T,Sz>::operator<(
      unsigned_mixed_system const &rhs
    ) const {
      bool dst = std::lexicographical_compare(
        this->begin(),
        this->end(),
        rhs.begin(),
        rhs.end()                            );

      return dst;
    }


  } // namespace radix

} // namespace torment
