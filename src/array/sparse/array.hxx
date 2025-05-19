#pragma once

#include "radix/mix.hxx"
#include "array/sparse/base.hxx"

namespace torment {

namespace sparse {
template<class V, std::size_t N, class K> struct array;

#ifdef _IOSTREAM_ // {

template< class V, std::size_t N, class K>
std::ostream& operator<<(std::ostream &os, array<V, N, K> const &arr);

#endif // } _IOSTREAM_

template<class V, std::size_t R = 1, class K = std::size_t>
struct array : base<V,R,K> {

typedef base<V,R,K> base_type;
typedef typename base_type::value_type value_type;

using base_type::base_type;
using base_type::operator=;

#ifdef _IOSTREAM_ // {

friend std::ostream& operator<< <V,R,K>(std::ostream &os, array const &sarr);

#endif // } _IOSTREAM_

};

} // namespace sparse

} // namespace torment