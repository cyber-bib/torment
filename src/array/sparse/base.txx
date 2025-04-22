#pragma once

#include "array/sparse/base.hxx"
#include "map/map.txx"

#include <stdexcept>
#include <numeric>

namespace torment {

  namespace sparse {

    // template<class V, class K, class C>
    // base<V,K,C>::base(  key_type && dims,
    //                     value_type && default_v                                )
    // {
    //   this->insert_or_assign(dims, default_v);
    // }
    // template<class V, class K, class C>
    // base<V,K,C>::base(  key_type const& dims,
    //                     value_type const& default_v                            )
    // {
    //   this->insert_or_assign(dims, default_v);
    // }

    // template<class V, class K, class C>
    // base<V,K,C>::base( list_type const& list,
    //       key_type && dims,
    //       value_type && default_v                                              )
    // : map(list)
    // {
    //   this->insert_or_assign(dims, default_v);
    // }

    template<class V, class K, class C>
    base<V,K,C>::base(
          key_type const& dims,
          value_type const& default_v,
          list_type const& list                                                )
    : map(list)
    {
      this->shape(dims);
      this->default_value(default_v);
      // this->insert_or_assign(dims, default_v);
    }

    // template<class V, class K, class C>
    // base<V,K,C>::base(elem_type const& default_elem)
    // : base({}, default_elem) {}

    // template<class V, class K, class C>
    // base<V,K,C>::base(list_type const& list, elem_type const& default_elem)
    // : map(list)
    // {
    //   this->insert_or_assign(default_elem.first, default_elem.second);
    // }

    template<class V, class K, class C>
    typename base<V,K,C>::base&
    base<V,K,C>::operator=(list_type &&list) {

      auto[shape, value] = *this->rbegin();

      static_cast<container_type&>(*this) = list;

      this->shape(shape);
      this->default_value(value);
      // this->insert_or_assign(shape, value);

      return *this;
    }

    template<class V, class K, class C>
    std::size_t base<V,K,C>::n_elem() const {
      if constexpr(std::is_integral_v<key_type>) {
        return static_cast<std::size_t>(this->shape());
      } else {
        std::size_t _sz = 1;

        for(auto const &e : this->shape()) {
          _sz *= e;
        }

        return static_cast<std::size_t>(_sz);
      }
    }

    template<class V, class K, class C>
    typename base<V,K,C>::key_type const& base<V,K,C>::shape() const {
      return this->rbegin()->first;
    }

    template<class V, class K, class C>
    void base<V,K,C>::shape(key_type const& new_shape) {
      if(this->size() == 0) {
        this->insert_or_assign(new_shape, value_type(0));
      }

      auto[shape, value] = *this->rbegin();

      for(auto it = this->lower_bound(new_shape); it != this->end();) {
        it = this->erase(it);
      }

      this->insert_or_assign(new_shape, value);
    }

    template<class V, class K, class C>
    typename base<V,K,C>::value_type const&
    base<V,K,C>::default_value() const {
      return this->rbegin()->second;
    }

    template<class V, class K, class C>
    void base<V,K,C>::default_value(value_type const& new_value) {
      this->rbegin()->second = new_value;
    }
    // std::size_t size() const;
    template<class V, class K, class C>
    typename base<V,K,C>::value_type const&
    base<V,K,C>::operator()(key_type const& key) const {
      if(!(key < this->shape())) throw std::out_of_range("key is out of range");

      if(auto it = this->find(key); it != this->end()) {
        return it->second;
      } else {
        return this->rbegin()->second;
      }
    }

    template<class V, class K, class C>
    std::ostream& operator<< (
      std::ostream &os,
      base<V,K,C> const &sarr
    ) {

      os << "[ ";

      auto i = sarr.shape();
      i = 0;

      if(sarr.n_elem() != 0) os << sarr(i);

      for(i = 1; i < sarr.shape(); ++i) {
        os << ", " << sarr(i);
      }

      os << " ]";

      return os;
    }

  };

};