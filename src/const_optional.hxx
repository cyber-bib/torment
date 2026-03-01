#pragma once

namespace torment {

  struct const_nullopt_t {
    explicit constexpr const_nullopt_t(int) {}
  };
  inline constexpr const_nullopt_t const_nullopt{0};

  template<class T>
  struct const_optional {
    const bool engaged;
    const T value;

    constexpr const_optional() : engaged(false), value{} {}
    constexpr const_optional(const_nullopt_t) : engaged(false), value{} {}
    constexpr const_optional(T v) : engaged(true), value(v) {}

    // observers
    constexpr bool has_value() const noexcept { return engaged; }
    constexpr explicit operator bool() const noexcept { return engaged; }

    constexpr T const& operator*() const noexcept { return value; }
    constexpr T const* operator->() const noexcept { return &value; }

    constexpr T const& value_ref() const {
      if (!engaged) throw "bad const_optional access";
      return value;
    }

    template<class U>
    constexpr T value_or(U&& fallback) const {
      return engaged ? value : static_cast<T>(fallback);
    }
  };


};
