#pragma once

#include <boost/iterator/iterator_facade.hpp>
#include <type_traits>

namespace torment {
  namespace dense {

    template<class Proxy, bool IsConst>
    struct proxy_iterator_traits {
      typedef typename Proxy::value_type value_type;
      typedef std::conditional_t<IsConst, value_type const, value_type> facade_value_type;
      typedef std::conditional_t<IsConst, value_type const&, value_type&> reference;
      typedef std::conditional_t<IsConst, Proxy const, Proxy> proxy_type;
      typedef typename Proxy::proxy_idx_t index_type;
    };

    template<class Proxy, bool IsConst>
    struct proxy_iterator
    : boost::iterator_facade<
        proxy_iterator<Proxy, IsConst>,
        typename proxy_iterator_traits<Proxy, IsConst>::facade_value_type,
        boost::forward_traversal_tag,
        typename proxy_iterator_traits<Proxy, IsConst>::reference>
    {
      typedef proxy_iterator_traits<Proxy, IsConst> traits_type;
      typedef typename traits_type::reference reference;
      typedef typename traits_type::proxy_type proxy_type;
      typedef typename traits_type::index_type index_type;

      proxy_type *m_owner = nullptr;
      std::size_t m_pos = 0;

      proxy_iterator() = default;
      proxy_iterator(proxy_type *owner, std::size_t pos);

      private:
      friend class boost::iterator_core_access;

      void increment();
      bool equal(proxy_iterator const& other) const;
      reference dereference() const;
    };

  };
};

