#include "matrix.hxx"

namespace tormentor {
  matrix::matrix() {};
  matrix::matrix(key_type const &shape, scalar const &value)
  : m_shape(shape), m_data(shape[0]*shape[1], value) {};

  matrix::matrix(std::size_t const &rows, std::size_t const &cols, scalar const &value)
  : matrix({rows, cols}, value) {};

  matrix::key_type matrix::shape() const {
    key_type dst = this->m_shape;
    return dst;
  }
  void matrix::reshape(std::size_t const &rows, std::size_t const &cols) {
    this->m_shape[0] = rows;
    this->m_shape[1] = cols;

    this->m_data.resize(rows*cols);
  }
  std::size_t matrix::rows() const {
    return this->m_shape[0];
  }
  std::size_t matrix::cols() const {
    return this->m_shape[1];
  }
  matrix_proxy matrix::row(std::size_t const &r) {
    matrix_proxy dst(*this, [&](std::size_t i, std::size_t j) {
      key_type rst = {r + (i == 0 ? 0 : this->m_shape[0]), j};
      return rst;
    });

    return dst;
  }

  // matrix_proxy col(std::size_t const &j);
  matrix_proxy matrix::col(std::size_t const &c) {
    matrix_proxy dst(*this, [&](std::size_t i, std::size_t j) {
      key_type rst = {i, c + (j == 0 ? 0 : this->m_shape[1])};
      return rst;
    });

    return dst;
  }

  matrix matrix::operator*(matrix const &rhs) const {
    matrix const &lhs = *this;
    matrix dst({this->m_shape[0], rhs.m_shape[1]}, 0.0);


    if(lhs.m_shape[1] != rhs.m_shape[0])
      throw std::exception("matrix product error");

    for(std::size_t i = 0; i < lhs.m_shape[0]; i++) {
      for(std::size_t j = 0; j < rhs.m_shape[1]; j++) {
        for(std::size_t k = 0; k < lhs.m_shape[1]; k++) {
          dst(i, j) += lhs(i, k) * rhs(k, j);
        }
      }
    }

    return dst;
  }
  matrix matrix::operator*(scalar const &rhs) const {
    matrix dst(this->m_shape, 0.0);

    for(std::size_t i = 0; i < this->m_data.size(); i++)
      dst.m_data[i] = this->m_data[i]*rhs;

    return dst;
  }
  matrix matrix::inv() const {
    matrix dst(this->shape()), tmp = (*this);


    for(std::size_t j = 0; j < this->rows(); j++) {
      dst(j, j) = 1;
      for(std::size_t i = j + 1; i < this->rows(); i++) {
        // auto scale = iM(i, j)/iM(j,j);
        // iM.row(i) -= scale*iM.row(j);
        //
        auto scale = tmp(i, j)/tmp(j,j);
        tmp.row(i) -= scale*tmp.row(j);
        dst.row(i) -= scale*dst.row(j);
        // tmp(i, j) = 0.0;
      }
    }

    for(std::size_t j = this->rows() - 1; j < this->rows(); j--) {
      dst.row(j) *= (1.0/tmp(j, j));
      for(std::size_t i = j - 1; i < this->rows(); i--) {
        auto scale = tmp(i, j);
        dst.row(i) -= scale*dst.row(j);
        // tmp(i,j) = 0;
      }
    }

    return dst;
  }
  matrix::scalar& matrix::operator()(key_type const &key) {
    return this->m_data[key[1]*this->m_shape[0] + key[0]];
  }
  matrix::scalar& matrix::operator()(
    std::size_t const &row,
    std::size_t const &col
  ) {
    return this->m_data[col*this->m_shape[0] + row];
  }
  matrix::scalar const& matrix::operator()(key_type const &key) const {
    return this->m_data[key[1]*this->m_shape[0] + key[0]];
  }
  matrix::scalar const& matrix::operator()(
    std::size_t const &row,
    std::size_t const &col
  ) const {
    return this->m_data[col*this->m_shape[0] + row];
  }
  matrix_proxy matrix::operator()(functor_type const &functor) {
    matrix_proxy dst(*this, functor);
    return dst;
  }
  matrix operator*(matrix::scalar const &lhs, matrix const &rhs) {
    return rhs*lhs;
  }
  std::ostream& operator<<(std::ostream &os, matrix const &mat) {
    os << "{\n";
    for(std::size_t i = 0; i < mat.m_shape[0]; i++) {
      os << "\t";
      for(std::size_t j = 0; j < mat.m_shape[1]; j++) {
        os << mat(i, j) << "  ";
      }
      os << "\n";
    }
    os << "}";

    return os;
  }







  matrix_proxy::matrix_proxy(matrix &mat, functor_type const &functor)
  : m_matrix(mat), m_functor(functor)
  {
    this->m_shape = {0, 0};

    bool empty_shape = false;
    for(auto s: this->m_matrix.m_shape)
      empty_shape |= s == 0;

    if(empty_shape) return;

    //could change this a binary search algorithm
    for(std::size_t i = 0; i < this->m_matrix.m_shape[0]; i++) {
      this->m_shape[0] += this->m_functor(i, 0)[0] < this->m_matrix.m_shape[0];
    }

    for(std::size_t j = 0; j < this->m_matrix.m_shape[1]; j++) {
      this->m_shape[1] += this->m_functor(0, j)[1] < this->m_matrix.m_shape[1];
    }
  }

  matrix_proxy::key_type matrix_proxy::shape() const {
    key_type dst = this->m_shape;
    return dst;
  }

  matrix_proxy matrix_proxy::row(std::size_t const &r) {
    matrix_proxy dst(this->m_matrix, [&](std::size_t i, std::size_t j) {
      key_type rst = {
        r + (i == 0 ? 0 : this->m_matrix.m_shape[0]),
        (j < this->m_shape[1] ? j : this->m_matrix.m_shape[1])
      };
      return rst;
    });

    return dst;
  }
  matrix_proxy matrix_proxy::col(std::size_t const &c) {
    matrix_proxy dst(this->m_matrix, [&](std::size_t i, std::size_t j) {
      key_type rst = {
        (i < this->m_shape[0] ? i : this->m_matrix.m_shape[0]),
        c + (j == 0 ? 0 : this->m_matrix.m_shape[1])
      };
      return rst;
    });

    return dst;
  }

  matrix_proxy& matrix_proxy::operator-=(matrix const &rhs) {
    matrix_proxy const &lhs = *this;

    if(lhs.m_shape[0] != rhs.m_shape[0] || lhs.m_shape[1] != rhs.m_shape[1])
      throw std::exception("matrix product error");

    for(std::size_t i = 0; i < lhs.m_shape[0]; i++) {
      for(std::size_t j = 0; j < lhs.m_shape[1]; j++) {
          (*this)(i, j) -= rhs(i, j);
      }
    }
    return *this;
  }
  matrix_proxy& matrix_proxy::operator-=(matrix_proxy const &rhs) {
    matrix_proxy const &lhs = *this;

    if(lhs.m_shape[0] != rhs.m_shape[0] || lhs.m_shape[1] != rhs.m_shape[1])
      throw std::exception("matrix product error");

    for(std::size_t i = 0; i < lhs.m_shape[0]; i++) {
      for(std::size_t j = 0; j < lhs.m_shape[1]; j++) {
          (*this)(i, j) -= rhs(i, j);
      }
    }
    return *this;
  }
  matrix_proxy&
  matrix_proxy::operator*=(
    scalar const &rhs
  ) {
    matrix_proxy const &lhs = *this;

    for(std::size_t i = 0; i < lhs.m_shape[0]; i++) {
      for(std::size_t j = 0; j < lhs.m_shape[1]; j++) {
          (*this)(i, j) *= rhs;
      }
    }
    return *this;
  }

  matrix matrix_proxy::operator*(matrix_proxy const &rhs) const {
    matrix_proxy const &lhs = *this;
    matrix dst({this->m_shape[0], rhs.m_shape[1]}, 0.0);

    if(lhs.m_shape[1] != rhs.m_shape[0])
      throw std::exception("matrix product error");

    for(std::size_t i = 0; i < lhs.m_shape[0]; i++) {
      for(std::size_t j = 0; j < rhs.m_shape[1]; j++) {
        for(std::size_t k = 0; k < lhs.m_shape[1]; k++) {
          dst(i, j) += lhs(i, k) * rhs(k, j);
        }
      }
    }

    return dst;
  }
  matrix matrix_proxy::operator*(scalar const &rhs) const {
    matrix dst(this->m_shape);

    for(std::size_t i = 0; i < this->m_shape[0]; i++) {
      for(std::size_t j = 0; j < this->m_shape[1]; j++) {
        dst(i, j) = (*this)(i, j) * rhs;
      }
    }

    return dst;
  }

  matrix_proxy::scalar&
  matrix_proxy::operator()(
    std::size_t const &key
  ) {
    auto row = this->m_shape[0] == 1;
    auto col = this->m_shape[1] == 1;

    return this->m_matrix(this->m_functor(key*row, key*col));
  }

  matrix_proxy::scalar&
  matrix_proxy::operator()(
    std::size_t const &row,
    std::size_t const &col
  ) {
    return this->m_matrix(this->m_functor(row, col));
  }

  matrix_proxy::scalar&
  matrix_proxy::operator()(
    key_type const &key
  ) {
    return this->m_matrix(this->m_functor(key[0], key[1]));
  }


  matrix_proxy::scalar const&
  matrix_proxy::operator()(
    std::size_t const &key
  ) const {
    auto row = this->m_shape[0] == 1;
    auto col = this->m_shape[1] == 1;

    return this->m_matrix(this->m_functor(key*row, key*col));
  }

  matrix_proxy::scalar const&
  matrix_proxy::operator()(
    std::size_t const &row,
    std::size_t const &col
  ) const {
    return this->m_matrix(this->m_functor(row, col));
  }

  matrix_proxy::scalar const&
  matrix_proxy::operator()(
    key_type const &key
  ) const {
    return this->m_matrix(this->m_functor(key[0], key[1]));
  }


  std::ostream& operator<<(std::ostream &os, matrix_proxy const &mat)
  {
    os << "{\n";
    for(std::size_t i = 0; i < mat.m_shape[0]; i++) {
      os << "\t";
      for(std::size_t j = 0; j < mat.m_shape[1]; j++) {
        os << mat(i, j) << "  ";
      }
      os << "\n";
    }
    os << "}";

    return os;
  }
  matrix operator*(matrix_proxy::scalar const &lhs, matrix_proxy const &rhs) {
    auto dst = rhs*lhs;
    return dst;
  }
};