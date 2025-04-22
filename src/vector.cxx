#include "vector.hxx"

namespace tormentor {
  vector::vector(key_type const &size, scalar const &value)
  : m_data(size, value) {};

  std::size_t vector::size() const {
    return this->m_data.size();
  }
  vector::scalar vector::operator*(vector const &rhs) const {
    scalar dst;

    if(this->m_data.size() != rhs.m_data.size())
      throw std::exception("dot product error");

    dst = 0.0;
    for(std::size_t i = 0; i < this->m_data.size(); i++)
      dst += this->m_data[i]*rhs.m_data[i];

    return dst;
  }
  vector vector::operator*(scalar const &rhs) const {
    vector dst(this->m_data.size(), 0.0);

    for(std::size_t i = 0; i < this->m_data.size(); i++)
      dst.m_data[i] = this->m_data[i]*rhs;

    return dst;
  }
  vector operator*(vector::scalar const &lhs, vector const &rhs) {
    return rhs*lhs;
  }
  vector::scalar& vector::operator()(key_type const &key) {
    return this->m_data[key];
  }
  vector::scalar const& vector::operator()(key_type const &key) const {
    return this->m_data[key];
  }

  vector_proxy vector::operator()(functor_type const &functor)
  {
    vector_proxy dst(*this, functor);
    return dst;
  }

  std::ostream& operator<<(std::ostream &os, vector const vec) {
    if(vec.size() == 0) {
      os << "{ }";
      return os;
    }

    os << "{ " << vec(0);
    for(std::size_t i = 1; i < vec.size(); i++) {
      os << ", " << vec(i);
    }
    os << " }";
    return os;
  }

  vector_proxy::vector_proxy(vector &vec, std::function<std::size_t(std::size_t)> const &functor)
  : m_vector(vec), m_functor(functor) {
    this->m_size = 0;

    //could change this a binary search algorithm
    for(std::size_t i = 0; i < this->m_vector.size(); i++) {
      this->m_size += this->m_functor(i) < this->m_vector.size();
    }
  }

  std::size_t vector_proxy::size() const {
    return this->m_size;
  }

  vector_proxy::scalar vector_proxy::operator* (
    vector_proxy const &rhs
  ) const {
    vector_proxy const &lhs = *this;
    scalar dst;

    if(lhs.size() != rhs.size())
      throw std::exception("dot product error");

    dst = 0.0;
    for(std::size_t i = 0; i < rhs.size(); i++)
      dst += lhs(i)*rhs(i);

    return dst;
  }
  vector vector_proxy::operator*(scalar const &rhs) const {
    vector dst(this->size(), 0.0);

    for(std::size_t i = 0; i < this->size(); i++)
      dst.m_data[i] = (*this)(i)*rhs;

    return dst;
  }

  vector_proxy::scalar&
  vector_proxy::operator()(key_type const &key) {
    return this->m_vector(this->m_functor(key));
  }

  vector_proxy::scalar const&
  vector_proxy::operator()(key_type const &key) const {
    return this->m_vector(this->m_functor(key));
  }

  std::ostream& operator<<(std::ostream &os, vector_proxy const vec) {
    if(vec.size() == 0) {
      os << "{ }";
      return os;
    }

    os << "{ " << vec(0);
    for(std::size_t i = 1; i < vec.size(); i++) {
      os << ", " << vec(i);
    }
    os << " }";

    return os;
  }
};