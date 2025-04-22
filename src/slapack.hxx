#include <cstddef>
// #include <limits>
#include <string>
#include <utility>
#include <climits>
#include <cstdint>
#include <type_traits>
#include <initializer_list>

#include <map>
#include <array>
#include <stdexcept>
#include <iostream>

typedef std::uint8_t u8;

template<
  class F, class S,
  std::uint64_t Fz = sizeof(F)*CHAR_BIT,
  std::uint64_t Sz = sizeof(S)*CHAR_BIT,
  bool = (Fz + Sz <= (sizeof(F) + sizeof(S) - 1)*CHAR_BIT)>
struct spair {
  typedef F first_type;
  typedef S second_type;
  first_type first : Fz;
  second_type second : Sz;
};

template<class F, class S, std::uint64_t Fz, std::uint64_t Sz>
struct spair<F, S, Fz, Sz, false> {
  typedef F first_type;
  typedef S second_type;

  first_type first;
  second_type second;
};


template<class T, std::size_t Sz = 0>
struct dense_array : std::array<T, Sz> {
  typedef std::initializer_list<T> list;

  explicit dense_array(int val = 0) {
    for(auto &e : *this) e = val;
  }
  // dense_array(std::initializer_list<T> &&list) {
  //   this->operator=(list);
  // }
  dense_array(std::initializer_list<T> const &list) {
    this->operator=(list);
  }
  dense_array& operator=(std::initializer_list<T> const &list) {
    if (list.size() != Sz)
      throw std::invalid_argument("list size != " + std::to_string(Sz));

    auto it = list.begin();
    for(auto &e : *this) e = *(it++);

    return *this;
  }

  friend std::ostream& operator<<(std::ostream &os, dense_array const & array) {
    auto size = array.size();

    os << "{";
    if(size != 0) {
      for(std::uint64_t i = 0; i < size - 1; i++)
        os << static_cast<std::size_t>(array[i]) << ", ";
      os << static_cast<std::size_t>(array[size - 1]);
    }
    os << "}";

    return os;
  }
};

template<
  class Key,
  class Value,
  class Compare = std::less<Key>,
  class Alloc = std::allocator<std::pair<const Key, Value  >>>
class data_map : public std::map<Key, Value, Compare, Alloc> {
  public:
    typedef std::map<Key, Value, Compare, Alloc> container_type;

  friend std::ostream& operator<<(std::ostream &os, data_map const & cont) {
    for(auto it = cont.begin(); it != cont.end(); it++)
      os << it->first << ": " << it->second << "\n";
    return os;
  }
};

template<class K, class V>
class sparsed_array {
  public:
    typedef K key_type;
    typedef V value_type;
    typedef data_map<key_type, value_type> container_type;

    class element_reference {
      key_type m_key;
      sparsed_array &m_array;
      element_reference(sparsed_array &array, key_type const &key) : m_array(array), m_key(key) {}

      public:
        element_reference& operator=(element_reference const &eref) {
          return this->operator=(static_cast<value_type>(eref));
        }
        element_reference& operator=(value_type const &value) {
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
            this->m_array.m_data.at(this->m_key) = value;
            goto _return_;
          }
          if(!key_in_map &&  default_value) {
            // return
            goto _return_;
          }
          if( key_in_map &&  default_value) {
            this->m_array.m_data.erase(it);
            goto _return_;
          }

          _return_:
            return *this;
        }
        operator value_type() const {
          auto it = this->m_array.m_data.find(this->m_key);
          if(it != this->m_array.m_data.end()) return it->second;
          else return this->m_array.m_default_value;
        }

        friend sparsed_array;
    };
  private:
    template<class T>
    using enable_if_integral_t = typename std::enable_if_t<std::is_integral_v<T>, bool>;
    template<class T>
    using enable_if_rank_gt_1 = typename std::enable_if_t<
      !std::is_integral_v<T> && std::is_same_v<T, K>, bool>;
    template<class T>
    using enable_if_rank_1 = typename std::enable_if_t<
      std::is_integral_v<T> && std::is_same_v<T, K>, bool>;

  protected:
    key_type m_shape;
    value_type m_default_value;
    data_map<key_type, value_type> m_data;
  // std::map<key_type, value_type> m_data;

  public:
    sparsed_array(key_type const &shape = 0, value_type const &default_value = 0) : m_shape(shape), m_default_value(default_value) {
      // static_assert(std::is_same_v<decltype(&key_type::operator std::uint64_t), decltype(&key_type::operator std::uint64_t)>, "");
      if constexpr(!std::is_integral_v<key_type>)
        static_assert(&key_type::size != nullptr, "sparsed_array<>::key_type does not provide a std::uint64_t size() function");
    }
    std::size_t size() const {
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
    template<class T = key_type, enable_if_rank_gt_1<T> = true>
    key_type const& shape() const {
      return this->m_shape;
    }

    container_type const& data() const {
      return this->m_data;
    };
    // template<
    //   class T = key_type,
    //   typename std::enable_if_t<std::is_integral_v<T>, bool> = false>
    element_reference operator[](key_type const &key) {
      element_reference ref(*this, key);
      return ref;
    }
    // template<
    //   class T = key_type,
    //   typename std::enable_if_t<std::is_integral_v<T>, bool> = false>
    value_type operator[](key_type const &key) const {
      auto it = this->m_data.find(key);
      if(it != this->m_data.end()) return it->second;
      else return this->m_default_value;
    }
    // template<
    //   class T = key_type,
    //   typename std::enable_if_t<std::is_integral_v<T>, bool>>
    key_type key_from(std::size_t val) const {
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
    element_reference operator[](std::size_t val) {
      auto k = this->key_from(val);
      auto dst = this->operator[](k);

      return dst;
    }
    value_type operator[](std::size_t val) const {
      auto k = this->key_from(val);
      auto dst = this->operator[](k);

      return dst;
    }

  // template<class T = K, enable_if_rank_1<T> = true>
  friend std::ostream& operator<<(std::ostream &os, sparsed_array const & array) {
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
};

template<class K, class V>
struct sparsed_2d_array : sparsed_array<dense_array<K, 2>, V> {
  typedef sparsed_array<dense_array<K, 2>, V> base_type;

  using base_type::base_type;
  using typename base_type::key_type;
  using typename base_type::value_type;

  // using sparsed_array<dense_array<K, 2>, V>::sparsed_array;
  // using sparsed_array<dense_array<K, 2>, V>::sparsed_array;

  friend std::ostream& operator<<(std::ostream &os, sparsed_2d_array const & array) {
    auto sz = array.size();
    auto sp = array.shape();

    key_type k;
    os << "{ ";
    if(sz != 0) {
      for(std::size_t i = 0; i < sp[0]; i++) {
        os << (i == 0 ? "{" : "  {");
        k[0] = i;
        for(std::size_t j = 0; j < sp[1] - 1; j++) {
          k[1] = j;
          os << array[k] << ",";
        }
        k[1] = sp[1] - 1;
        os << array[k];
        // os << "},\n";
        os << (i != sp[0] - 1 ? "},\n" : "} }");
      }
    }

    return os;
  }
};