#pragma once

#include "array/sparse/base.hxx"
#include "map/map.txx"

#include <sstream>
#include <stdexcept>
#include <numeric>

namespace torment {

  namespace sparse {

    // template< class V, std::size_t R, class I, class K, class C>
    // base<V,R,I,K,C>::base(  key_type && dims,
    //                     value_type && default_v                                )
    // {
    //   this->insert_or_assign(dims, default_v);
    // }
    // template< class V, std::size_t R, class I, class K, class C>
    // base<V,R,I,K,C>::base(  key_type const& dims,
    //                     value_type const& default_v                            )
    // {
    //   this->insert_or_assign(dims, default_v);
    // }

    // template< class V, std::size_t R, class I, class K, class C>
    // base<V,R,I,K,C>::base( list_type const& list,
    //       key_type && dims,
    //       value_type && default_v                                              )
    // : map(list)
    // {
    //   this->insert_or_assign(dims, default_v);
    // }

    template< class V, std::size_t R, class I, class K, class C>
    template<typename> base<V,R,I,K,C>::base(
      index_type dims,
      value_type const& default_val,
      list_type const& list  )
    : base(key_type{dims}, default_val, list) {
    }

    template< class V, std::size_t R, class I, class K, class C>
    base<V,R,I,K,C>::base(
          key_type dims,
          value_type const& default_val,
          list_type const& list  )
    : container_type(list), m_dims(dims), m_dval(default_val) {

      if constexpr(R == 0) {
        switch(dims.size()) {
          case 0:
            this->m_dims.clear();
            if(list.size() != 0) {
              std::invalid_argument err("a list was provided to an array with rank 0");
              throw err;
            }
            break;
          default:
            this->throw_if_list_not_valid(list);
            break;
        }
      }
    }

    // template< class V, std::size_t R, class I, class K, class C>
    // base<V,R,I,K,C>::base(elem_type const& default_elem)
    // : base({}, default_elem) {}

    // template< class V, std::size_t R, class I, class K, class C>
    // base<V,R,I,K,C>::base(list_type const& list, elem_type const& default_elem)
    // : map(list)
    // {
    //   this->insert_or_assign(default_elem.first, default_elem.second);
    // }

    template< class V, std::size_t R, class I, class K, class C>
    template<typename> typename base<V,R,I,K,C>::base&
    base<V,R,I,K,C>::operator=(value_type const &val) {
      if(this->m_dims.size() != 0) {
        std::invalid_argument err("expecting a list, a scalar was given (rank != 0)");
        throw err;
      }
      this->m_dval = val;
      return *this;
    }
    template< class V, std::size_t R, class I, class K, class C>
    typename base<V,R,I,K,C>::base&
    base<V,R,I,K,C>::operator=(list_type const &list) {

      // auto[shape, value] = *this->rbegin();

      if constexpr(R == 0) {
        if(this->m_dims.size() == 0) {
          std::invalid_argument err("expecting a scalar, a list was given (rank = 0)");
          throw err;
        }
        this->throw_if_list_not_valid(list);



      }

      static_cast<container_type&>(*this) = list;

      // this->shape(shape);
      // this->default_value(value);
      // this->insert_or_assign(shape, value);

      return *this;
    }

    template< class V, std::size_t R, class I, class K, class C>
    std::size_t base<V,R,I,K,C>::n_elem() const {
      if constexpr(std::is_integral_v<key_type>) {
        return static_cast<std::size_t>(this->shape());
      } else {
        std::size_t _sz = 1;

        for(auto const &e : this->m_dims) {
          _sz *= e;
        }

        return static_cast<std::size_t>(_sz);
      }
    }

    template< class V, std::size_t R, class I, class K, class C>
    typename base<V,R,I,K,C>::key_type const& base<V,R,I,K,C>::shape() const {
      return this->m_dims;
    }

    template< class V, std::size_t R, class I, class K, class C>
    void base<V,R,I,K,C>::shape(key_type const& new_shape) {
      this->m_dims = new_shape;

      if constexpr(!std::is_integral_v<key_type>) {
        // auto ref = static_cast<key_type::base_type&>(this->m_dims);
        // ref = *this->m_dims.m_radices;
        this->m_dims = 0;
      }
    }

    template< class V, std::size_t R, class I, class K, class C>
    typename base<V,R,I,K,C>::value_type const&
    base<V,R,I,K,C>::default_value() const {
      return this->m_dval;
    }

    template< class V, std::size_t R, class I, class K, class C>
    void base<V,R,I,K,C>::default_value(value_type const& new_value) {
      this->m_dval = new_value;
    }

    template< class V, std::size_t R, class I, class K, class C>
    typename base<V,R,I,K,C>::element_iterator
    base<V,R,I,K,C>::begin() {
      assert("TODO: Implementation");
      typedef typename element_iterator::iterable_type iterable_type;

      iterable_type ikey(this->m_dims);
      element_iterator dst(ikey, *this);

      return dst;
    }
    // template< class V, std::size_t R, class I, class K, class C>
    // typename base<V,R,I,K,C>::element_const_iterator
    // base<V,R,I,K,C>::begin() const {
    //   assert("TODO: Implementation");
    //   typedef typename element_const_iterator::iterable_type iterable_type;

    //   iterable_type ikey(this->m_dims);
    //   element_const_iterator dst(ikey, *this);

    //   return dst;
    // }

    template< class V, std::size_t R, class I, class K, class C>
    typename base<V,R,I,K,C>::element_iterator
    base<V,R,I,K,C>::end() {
      assert("TODO: Implementation");
      typedef typename element_iterator::iterable_type iterable_type;

      iterable_type ikey(this->m_dims);
      ikey.m_overflow = true;
      element_iterator dst(ikey, *this);

      return dst;
    }
    // template< class V, std::size_t R, class I, class K, class C>
    // typename base<V,R,I,K,C>::element_const_iterator
    // base<V,R,I,K,C>::end() const {
    //   assert("TODO: Implementation");
    //   typedef typename element_const_iterator::iterable_type iterable_type;

    //   iterable_type ikey(this->m_dims);
    //   element_const_iterator dst(ikey, *this);

    //   return dst;
    // }

    template< class V, std::size_t R, class I, class K, class C>
    typename base<V,R,I,K,C>::element_reference
    base<V,R,I,K,C>::operator[](key_type const& key) {
      element_reference dst(key, *this);
      return dst;
    }
    template< class V, std::size_t R, class I, class K, class C>
    typename base<V,R,I,K,C>::value_type const&
    base<V,R,I,K,C>::operator[](key_type const& key) const {
      if constexpr(R == 1) {
        if(!(key < this->shape())) {
          std::stringstream ss;
          ss  << "key is out of range ("
              << "key = " << key << ", "
              << "shape = " << this->shape() << ")";
          throw std::out_of_range(ss.str());
        }
      } else {
        auto shape = this->m_dims;

        if(!(key < shape)) {
          std::stringstream ss;
          ss  << "key is out of range ("
              << "key = " << key << ", "
              << "shape = " << this->shape() << ")";
          throw std::out_of_range(ss.str());
        }
      }

      auto data = static_cast<container_type const&>(*this);
      if(auto it = data.find(key); it != data.end()) {
        return it->second;
      } else {
        return this->m_dval;
      }
    }

    template< class V, std::size_t R, class I, class K, class C>
    template<typename> bool
    base<V,R,I,K,C>::is_list_valid(list_type const &list) const {
      bool dst = true;

      for(auto &elem : list) {
        auto &[shape, value] = elem;

        if(shape.size() != this->m_dims.size()) {
          dst = false;
          break;
        }
      }

      return dst;
    }

    template< class V, std::size_t R, class I, class K, class C>
    template<typename> void
    base<V,R,I,K,C>::throw_if_list_not_valid(list_type const &list) const {
      if(!is_list_valid(list)) {
        std::invalid_argument err("list key's rank(s) do not match the array's dimensionality");
        throw err;
      }
    }

    template< class V, std::size_t R, class I, class K, class C>
    base<V,R,I,K,C>::element_reference::element_reference(key_type const &key, base &chain)
    : m_key(key), m_chain(chain) {}

    template< class V, std::size_t R, class I, class K, class C>
    base<V,R,I,K,C>::element_reference::operator base<V,R,I,K,C>::value_type() const {
      auto it = this->m_chain.find(this->m_key);
      auto dst = it != this->m_chain.end() ? it->second : this->m_chain.dval;
      return dst;
    }

    template< class V, std::size_t R, class I, class K, class C>
    typename base<V,R,I,K,C>::element_reference&
    base<V,R,I,K,C>::element_reference::operator=(value_type const &value) {
      // std::cout << "TODO: operator= not implemented!!!";
      if(value != this->m_chain.m_dval) {
        this->m_chain.insert_or_assign(this->m_key, value);
      } else {
        this->m_chain.erase(this->m_key);
      }

      return *this;
    }

    template< class V, std::size_t R, class I, class K, class C>
    base<V,R,I,K,C>::element_iterator::element_iterator(
      iterable_type const& ikey,
      base &chain  )
    : m_ikey(ikey), m_chain(chain) {}

    template< class V, std::size_t R, class I, class K, class C>
    typename base<V,R,I,K,C>::element_reference
    base<V,R,I,K,C>::element_iterator::operator*() {
      element_reference dst(this->m_ikey, this->m_chain);
      return dst;
    }
    // element_const_reference operator*() const;

    template< class V, std::size_t R, class I, class K, class C>
    typename base<V,R,I,K,C>::element_iterator&
    base<V,R,I,K,C>::element_iterator::operator++() {
      ++this->m_ikey;

      return *this;
    }

    template< class V, std::size_t R, class I, class K, class C> bool
    base<V,R,I,K,C>::element_iterator::operator<(
      element_iterator const &rhs
    ) const {
      bool dst  = this->m_ikey.m_overflow < rhs.m_ikey.m_overflow ? true :
                    this->m_ikey < rhs.m_ikey;

      // std::cout << "lhs overflow: " << this->m_ikey.m_overflow << ", "
      //           << "rhs overflow: " << rhs.m_ikey.m_overflow << "\n"
      //           << "lhs key: "      << this->m_ikey << ", "
      //           << "rhs key: "      << rhs.m_ikey << "\n"
      //           << "result: "       << dst << "\n";

      return dst;
    }

    template< class V, std::size_t R, class I, class K, class C>
    std::ostream& operator<< (
      std::ostream &os,
      base<V,R,I,K,C> const &arr
    ) {
      typedef radix::unsigned_mixed_system<I, R> ums;


      if constexpr(R == 0) {
        auto &s = arr.shape();

        switch(s.size()) {
          case 0:
            // os << "[\"TODO: implement function 'std::ostream& operator<< (...)'\n"
            //       "        for dynamic scalar array.                          \"]";
            os << arr.m_dval;
            break;
          case 1: {
            os << "[";

            if(s[0] != 0) os << arr[{0}];
            for(I i = 1; i < s[0]; i++) {
              os << ", " << arr[{i}];
            }

            os << "]";
          } break;
          case 2: {
            os << "[";

            if(arr.n_elem() != 0) {
              for(I i = 0; i < s[0]; i++) {
                os  << (i == 0 ? " " : "  ")
                    << "[" << arr[{i, 0}];
                for(I j = 1; j < s[1]; j++) {
                  os << ", " << arr[{i, j}];
                }
                os << "]" << (i == (s[0]-1) ? " " : "\n");
              }
            }

            os << "]";
          } break;
          default: {
            os << "[\n  ";
            ums k(s); k--;

            for(ums i(k.m_radices); i != k; i++) {
              bool comma = true;
              os << arr[i];
              for(I j = 0; j != k.size()-1; j++) {
                if(i[j] != k[j]) break;
                comma = false;
                os << "\n";
              }
              os << (comma ? ", " : "  ");
              comma = true;
            };
            os << arr[k];

            os << "\n]";
          } break;
        }

      } else if constexpr(R == 1) {
        os << "[";

        if(arr.shape() != 0) os << arr[0];
        for(I i = 1; i < arr.shape(); i++) {
          os << ", " << arr[i];
        }

        os << "]";
      } else if constexpr(R == 2) { // matrix form array
        auto s = arr.shape();

        os << "[";

        if(arr.n_elem() != 0) {
          for(I i = 0; i < s[0]; i++) {
            os  << (i == 0 ? " " : "  ")
                << "[" << arr[{i, 0}];
            for(I j = 1; j < s[1]; j++) {
              os << ", " << arr[{i, j}];
            }
            os << "]" << (i == (s[0]-1) ? " " : "\n");
          }
        }

        os << "]";
      } else {
        // os << "[\"TODO: implement function 'std::ostream& operator<< (...)'\n"
        //       "         for multidimensiinal rank.                         \"]";
        auto &s = arr.shape();

        os << "[\n  ";
        ums k(s); k--;

        for(ums i(k.m_radices); i != k; i++) {
          bool comma = true;
          os << arr[i];
          for(I j = 0; j != k.size()-1; j++) {
            if(i[j] != k[j]) break;
            comma = false;
            os << "\n";
          }
          os << (comma ? ", " : "  ");
          comma = true;
        };
        os << arr[k];

        os << "\n]";
      }
      // typedef radix::unsigned_mixed_system<std::size_t> ums;

      // auto n_elem = sarr.n_elem();

      // if (sarr.n_elem() != 0)
      // {

      // auto k = sarr.shape();
      // ums i(k);

      // // for(auto i = k--; i < k; i++) {
      // //   // os << "{" << i << "}: " << sarr(i);
      // // }
      // // os << sarr(k);

      // }


      return os;
    }


  };


};