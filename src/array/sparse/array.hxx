#pragma once

    template<class V, class K = std::size_t>
    struct array : base<V, K> {
      typedef base<V, K> base_type;

      using base_type::base_type;
      using base_type::operator=;

    };