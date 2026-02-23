#pragma once

#include "array.hxx"

#include "core.txx"
#include "proxy/core.txx"

#include <algorithm>
#include <functional>
#include <stdexcept>

namespace torment {

  namespace dense {

    template< class T, std::size_t Rk, class Idx, std::array<Idx, Rk> Sp >
    array<T,Rk,Idx,Sp>& array<T,Rk,Idx,Sp>::operator=(value_type const& rhs) {
      for(auto &e : *this) e = rhs;
      return *this;
    }

    template< class T, std::size_t Rk, class Idx, std::array<Idx, Rk> Sp >
    array<T,Rk,Idx,Sp>& array<T,Rk,Idx,Sp>::operator+=(array const& rhs) {
      if constexpr(base_type::m_is_size_dynamic) {
        if(this->size() != rhs.size())
          throw std::logic_error("array sizes do not match.");
      }

      for(std::size_t i = 0; i < this->size(); i++)
        (*this)[i] += rhs[i];

      return *this;
    }

    template< class T, std::size_t Rk, class Idx, std::array<Idx, Rk> Sp >
    array<T,Rk,Idx,Sp>& array<T,Rk,Idx,Sp>::operator-=(array const& rhs) {
      if constexpr(base_type::m_is_size_dynamic) {
        if(this->size() != rhs.size())
          throw std::logic_error("array sizes do not match.");
      }

      for(std::size_t i = 0; i < this->size(); i++)
        (*this)[i] -= rhs[i];

      return *this;
    }

    template< class T, std::size_t Rk, class Idx, std::array<Idx, Rk> Sp >
    array<T,Rk,Idx,Sp>& array<T,Rk,Idx,Sp>::operator*=(array const& rhs) {
      if constexpr(base_type::m_is_size_dynamic) {
        if(this->size() != rhs.size())
          throw std::logic_error("array sizes do not match.");
      }

      for(std::size_t i = 0; i < this->size(); i++)
        (*this)[i] *= rhs[i];

      return *this;
    }

    template< class T, std::size_t Rk, class Idx, std::array<Idx, Rk> Sp >
    array<T,Rk,Idx,Sp>& array<T,Rk,Idx,Sp>::operator/=(array const& rhs) {
      if constexpr(base_type::m_is_size_dynamic) {
        if(this->size() != rhs.size())
          throw std::logic_error("array sizes do not match.");
      }

      for(std::size_t i = 0; i < this->size(); i++)
        (*this)[i] /= rhs[i];

      return *this;
    }

    template< class T, std::size_t Rk, class Idx, std::array<Idx, Rk> Sp >
    array<T,Rk,Idx,Sp>& array<T,Rk,Idx,Sp>::operator%=(array const& rhs) {
      if constexpr(base_type::m_is_size_dynamic) {
        if(this->size() != rhs.size())
          throw std::logic_error("array sizes do not match.");
      }

      for(std::size_t i = 0; i < this->size(); i++)
        (*this)[i] %= rhs[i];

      return *this;
    }

    template< class T, std::size_t Rk, class Idx, std::array<Idx, Rk> Sp >
    array<T,Rk,Idx,Sp>& array<T,Rk,Idx,Sp>::operator+=(value_type const& rhs) {
      for(auto &e : *this) e += rhs;
      return *this;
    }

    template< class T, std::size_t Rk, class Idx, std::array<Idx, Rk> Sp >
    array<T,Rk,Idx,Sp>& array<T,Rk,Idx,Sp>::operator-=(value_type const& rhs) {
      for(auto &e : *this) e -= rhs;
      return *this;
    }

    template< class T, std::size_t Rk, class Idx, std::array<Idx, Rk> Sp >
    array<T,Rk,Idx,Sp>& array<T,Rk,Idx,Sp>::operator*=(value_type const& rhs) {
      for(auto &e : *this) e *= rhs;
      return *this;
    }

    template< class T, std::size_t Rk, class Idx, std::array<Idx, Rk> Sp >
    array<T,Rk,Idx,Sp>& array<T,Rk,Idx,Sp>::operator/=(value_type const& rhs) {
      for(auto &e : *this) e /= rhs;
      return *this;
    }

    template< class T, std::size_t Rk, class Idx, std::array<Idx, Rk> Sp >
    array<T,Rk,Idx,Sp>& array<T,Rk,Idx,Sp>::operator%=(value_type const& rhs) {
      for(auto &e : *this) e %= rhs;
      return *this;
    }

    template< class T, std::size_t Rk, class Idx, std::array<Idx, Rk> Sp >
    bool array<T,Rk,Idx,Sp>::operator==(array const& rhs) const {
      if constexpr(base_type::m_is_size_dynamic) {
        if(this->size() != rhs.size()) return false;
      }
      return std::equal(this->begin(), this->end(), rhs.begin());
    }

    template< class T, std::size_t Rk, class Idx, std::array<Idx, Rk> Sp >
    bool array<T,Rk,Idx,Sp>::operator<(array const& rhs) const {
      return std::lexicographical_compare(
        this->begin(), this->end(), rhs.begin(), rhs.end());
    }

    template< class T, std::size_t Rk, class Idx, std::array<Idx, Rk> Sp >
    bool array<T,Rk,Idx,Sp>::operator==(value_type const& rhs) const {
      return std::all_of(this->begin(), this->end(),
        [&](value_type const& e){ return e == rhs; });
    }

    template< class T, std::size_t Rk, class Idx, std::array<Idx, Rk> Sp >
    bool array<T,Rk,Idx,Sp>::operator<(value_type const& rhs) const {
      for(auto const& e : *this) {
        if(e < rhs) return true;
        if(rhs < e) return false;
      }
      return false;
    }

  };

};
