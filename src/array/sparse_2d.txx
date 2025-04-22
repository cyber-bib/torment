
  template<class K, class V>
  struct sparsed_2d_array : sparsed_array<dense_array<K, 2>, V> {
    typedef sparsed_array<dense_array<K, 2>, V> base_type;

    using base_type::base_type;
    using typename base_type::key_type;
    using typename base_type::value_type;

    // using sparsed_array<dense_array<K, 2>, V>::sparsed_array;
    // using sparsed_array<dense_array<K, 2>, V>::sparsed_array;

    friend std::ostream& operator<<(std::ostream &os, sparsed_2d_array const & array) {
      auto sz = array.size();
      auto sp = array.shape();

      key_type k;
      os << "{ ";
      if(sz != 0) {
        for(std::size_t i = 0; i < sp[0]; i++) {
          os << (i == 0 ? "{" : "  {");
          k[0] = i;
          for(std::size_t j = 0; j < sp[1] - 1; j++) {
            k[1] = j;
            os << array[k] << ",";
          }
          k[1] = sp[1] - 1;
          os << array[k];
          // os << "},\n";
          os << (i != sp[0] - 1 ? "},\n" : "} }");
        }
      }

      return os;
    }
  };