#pragma once

#include "map/map.hxx"

namespace torment {
  template<class V, class K > struct sparsed_array;

  template<class K, class V>
  struct sparsed_array {
    typedef K key_type;
    typedef V value_type;
    typedef map<key_type, value_type> container_type;

    struct element_reference;

    private:
      template<class T>
      using enable_if_integral_t = typename std::enable_if_t<
        std::is_integral_v<T>, bool>;

      template<class T>
      using enable_if_rank_gt_1 = typename std::enable_if_t<
        !std::is_integral_v<T> && std::is_same_v<T, K>, bool>;

      template<class T>
      using enable_if_rank_1 = typename std::enable_if_t<
        std::is_integral_v<T> && std::is_same_v<T, K>, bool>;

    protected:
      key_type        m_shape;
      value_type      m_default_value;
      container_type  m_data;

    public:
                                  sparsed_array(key_type const &shape = 0,
                                                value_type const &default_value = 0 );

            std::size_t           size() const;

      template<class T = key_type, enable_if_rank_gt_1<T> = true>
            key_type const&       shape() const;

            container_type const& data() const;

            key_type              key_from(std::size_t val) const;

            element_reference     operator[](key_type const &key);
            value_type            operator[](key_type const &key) const;

            element_reference     operator[](std::size_t val);
            value_type            operator[](std::size_t val) const;

    friend  std::ostream&         operator<<( std::ostream &os,
                                              sparsed_array const & array  );
  };

  template<class K, class V>
  struct sparsed_array<K,V>::element_reference {
    element_reference& operator=(element_reference const &eref);
    element_reference& operator=(value_type const &value);

    operator value_type() const;

    friend sparsed_array;

    private:
      key_type m_key;
      sparsed_array &m_array;
      element_reference(sparsed_array &array, key_type const &key);
  };
};