#pragma once

#include <vector>
#include <functional>

#include <iostream>

namespace tormentor {
  struct vector;
  struct vector_proxy;

  struct vector {
    typedef std::size_t key_type;
    typedef double scalar;
    typedef std:: vector<scalar> container_type;
    typedef std::function<std::size_t(std::size_t)> functor_type;
    container_type m_data;

    vector(key_type const &size, scalar const &value = 0.0);

    std::size_t size() const;

    scalar operator*(vector const &rhs) const;
    vector operator*(scalar const &rhs) const;

    scalar& operator()(key_type const &key);
    scalar const& operator()(key_type const &key) const;
    vector_proxy operator()(functor_type const &functor);

    friend std::ostream& operator<<(std::ostream &os, vector const vec);
  };
  vector operator*(vector::scalar const &lhs, vector const &rhs);

  struct vector_proxy {
    typedef vector::key_type key_type;
    typedef vector::scalar scalar;
    typedef vector::container_type container_type;

    vector &m_vector;
    std::size_t m_size;
    std::function<std::size_t(std::size_t)> m_functor;

    vector_proxy(vector &vec, std::function<std::size_t(std::size_t)> const &functor);

    std::size_t size() const;

    scalar operator*(vector_proxy const &rhs) const;
    vector operator*(scalar const &rhs) const;

    scalar& operator()(key_type const &key);
    scalar const& operator()(key_type const &key) const;

    friend std::ostream& operator<<(std::ostream &os, vector_proxy const vec);
  };
};