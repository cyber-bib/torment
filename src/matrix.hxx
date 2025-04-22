#pragma once

#include <array>
#include <vector>
#include <functional>

#include <iostream>

namespace tormentor {
  struct matrix;
  struct matrix_proxy;

  struct matrix {
    typedef std::array<std::size_t, 2> key_type;
    typedef double scalar;
    typedef std::vector<scalar> container_type;
    typedef std::function<key_type(std::size_t, std::size_t)> functor_type;

    key_type m_shape;
    container_type m_data;

    matrix();
    matrix(key_type const &shape, scalar const &value = 0.0);
    matrix(std::size_t const &rows, std::size_t const &cols, scalar const &value = 0.0);

    key_type shape() const;
    void reshape(std::size_t const &rows, std::size_t const &cols);

    std::size_t rows() const;
    std::size_t cols() const;

    matrix_proxy row(std::size_t const &i);
    matrix_proxy col(std::size_t const &j);

    matrix operator*(matrix const &rhs) const;
    matrix operator*(scalar const &rhs) const;

    matrix inv() const;

    scalar& operator()(std::size_t const &row, std::size_t const &col);
    scalar& operator()(key_type const &key);
    matrix_proxy operator()(functor_type const &functor);

    scalar const& operator()(std::size_t const &row, std::size_t const &col) const;
    scalar const& operator()(key_type const &size) const;
    // matrix_proxy operator()(functor_type const &functor) const;

    friend std::ostream& operator<<(std::ostream &os, matrix const &mat);
  };
  matrix operator*(matrix::scalar const &lhs, matrix const &rhs);



  struct matrix_proxy {
    typedef matrix::key_type key_type;
    typedef matrix::scalar scalar;
    typedef matrix::container_type container_type;
    typedef matrix::functor_type functor_type;

    matrix &m_matrix;
    key_type m_shape;
    functor_type m_functor;

    matrix_proxy(matrix &mat, functor_type const &functor);

    key_type shape() const;


    matrix_proxy row(std::size_t const &i);
    matrix_proxy col(std::size_t const &j);

    matrix_proxy& operator-=(matrix const &rhs);
    matrix_proxy& operator-=(matrix_proxy const &rhs);
    matrix_proxy& operator*=(scalar const &rhs);

    matrix operator*(matrix_proxy const &rhs) const;
    matrix operator*(scalar const &rhs) const;


    scalar& operator()(std::size_t const &key);
    scalar& operator()(std::size_t const &row, std::size_t const &col);
    scalar& operator()(key_type const &size);

    scalar const& operator()(std::size_t const &key) const;
    scalar const& operator()(std::size_t const &row, std::size_t const &col) const;
    scalar const& operator()(key_type const &size) const;

    friend std::ostream& operator<<(std::ostream &os, matrix_proxy const &mat);
  };
  matrix operator*(matrix::scalar const &lhs, matrix_proxy const &rhs);

} // namespace tormentor