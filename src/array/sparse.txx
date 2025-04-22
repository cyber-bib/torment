#pragma once

#include "array/sparse.hxx"

namespace torment {

  template<class K, class V>
  sparsed_array<K, V>::sparsed_array(
    key_type const &shape = 0,
    value_type const &default_value = 0  )
  : m_shape(shape),
    m_default_value(default_value)
  {
    if constexpr(!std::is_integral_v<key_type>)
      static_assert(&key_type::size != nullptr, "sparsed_array<>::key_type does not provide a std::uint64_t size() function");
  }

  template<class K, class V> std::size_t
  sparsed_array<K, V>::size() const {
    if constexpr(std::is_integral_v<key_type>) {
      return static_cast<std::uint64_t>(this->m_shape);
    } else {
      std::uint64_t _sz = 1;
      for(auto const &e : this->m_shape) {
        _sz *= e;
      }
      return static_cast<std::size_t>(_sz);
    }
  }

  template<class K, class V>
  template<class T, typename>
  sparsed_array<K, V>::key_type const&
  sparsed_array<K, V>::shape() const {
    return this->m_shape;
  }

  template<class K, class V> sparsed_array<K, V>::container_type const&
  sparsed_array<K, V>::data() const {
    return this->m_data;
  };

  template<class K, class V> sparsed_array<K, V>::element_reference
  sparsed_array<K, V>::operator[](
    key_type const &key
  ) {
    element_reference ref(*this, key);
    return ref;
  }

  template<class K, class V> sparsed_array<K, V>::value_type
  sparsed_array<K, V>::operator[](
    key_type const &key
  ) const {
    auto it = this->m_data.find(key);
    if(it != this->m_data.end()) return it->second;
    else return this->m_default_value;
  }

  template<class K, class V> sparsed_array<K, V>::key_type
  sparsed_array<K, V>::key_from (
    std::size_t val
  ) const {
    key_type key;

    std::size_t stride = this->size();
    val %= stride;
    for(std::size_t i = key.size() - 1; i > 0; i--) {
      stride /= this->m_shape[i];
      key[i] = val / stride;
      val %= stride;
    }
    key[0] = val;
    return key;
  }

  template<class K, class V> sparsed_array<K, V>::element_reference
  sparsed_array<K, V>::operator[](std::size_t val) {
    auto k = this->key_from(val);
    auto dst = this->operator[](k);

    return dst;
  }

  template<class K, class V> sparsed_array<K, V>::value_type
  sparsed_array<K, V>::operator[](std::size_t val) const {
    auto k = this->key_from(val);
    auto dst = this->operator[](k);

    return dst;
  }

  template<class K, class V>
  std::ostream& operator<<(std::ostream &os, sparsed_array<K,V> const & array) {
    auto size = array.size();

    os << "{";
    if(size != 0) {
      for(std::uint64_t i = 0; i < size - 1; i++)
        os << array[i] << ", ";
      os << array[size - 1];
    }
    os << "}";

    return os;
  }

  template<class K, class V>
  sparsed_array<K, V>::element_reference::element_reference(
    sparsed_array &array,
    key_type const &key
  ) : m_array(array), m_key(key) {}


  template<class K, class V> sparsed_array<K, V>::element_reference&
  sparsed_array<K, V>::element_reference::operator=(
    element_reference const &eref
  ) {
    return this->operator=(static_cast<value_type>(eref));
  }

  template<class K, class V> sparsed_array<K, V>::element_reference&
  sparsed_array<K, V>::element_reference::operator=(
    value_type const &value
  ) {
      bool key_in_map, default_value;

      auto it = this->m_array.m_data.find(this->m_key);

      key_in_map = it != this->m_array.m_data.end();
      default_value = this->m_array.m_default_value == value;

      if(!key_in_map && !default_value) {
        // insert
        this->m_array.m_data.insert(std::pair(this->m_key, value));
        goto _return_;
      }
      if( key_in_map && !default_value) {
        // change
        this->m_array.m_data.at(this->m_key) = value;
        goto _return_;
      }
      if(!key_in_map &&  default_value) {
        // do nothing
        goto _return_;
      }
      if( key_in_map &&  default_value) {
        // erase
        this->m_array.m_data.erase(it);
        goto _return_;
      }

      _return_:
        return *this;
    }

    template<class K, class V>
    sparsed_array<K, V>::element_reference::operator value_type() const {
      auto it = this->m_array.m_data.find(this->m_key);
      if(it != this->m_array.m_data.end()) return it->second;
      else return this->m_array.m_default_value;
    }

};
