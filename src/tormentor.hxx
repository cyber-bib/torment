
// #include "vector.hxx"
// #include "matrix.hxx"

#include <map>
#include <vector>
#include <array>

namespace tormentor {


  struct tensor : std::map<std::vector<std::size_t>, double> {
  };

  // namespace sparsed
  // {
  //   struct vector {
  //     typedef float value_type;
  //     typedef std::size_t integral_type;
  //     typedef std::vector<integral_type> key_type;
  //     typedef std::map<key_type, value_type> container_type;

  //     container_type m_data;
  //   };
  // } // namespace sparse

} // namespace tormentor
