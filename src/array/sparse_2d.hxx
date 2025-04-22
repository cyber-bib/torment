
template<class K, class V>
struct sparsed_2d_array : sparsed_array<dense_array<K, 2>, V> {
  typedef sparsed_array<dense_array<K, 2>, V> base_type;

  using base_type::base_type;
  using typename base_type::key_type;
  using typename base_type::value_type;

  friend std::ostream& operator<<(std::ostream &os, sparsed_2d_array const & array);
};