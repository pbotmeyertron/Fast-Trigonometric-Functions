#include <array>
#include <cmath>
#include <cstddef>
#include <type_traits>

template<typename T>
inline constexpr T pi = T(3.14159265358979323846);

template<typename T>
inline constexpr T tau = T(6.28318530717958647693);

// Virtual LUT Implementation for Trigonometric Functions.
template<typename T, std::size_t Bits = 16>
class TrigLUT {
    static_assert(std::is_floating_point_v<T>, "T must be a floating point type");
    
public:
    static constexpr std::size_t sin_bits = Bits;
    static constexpr std::size_t sin_mask = (1 << sin_bits) - 1;
    static constexpr std::size_t sin_count = sin_mask + 1;
    
    static constexpr T radian_to_index = T(sin_count) / tau<T>;
    static constexpr T degree_to_index = T(sin_count) / T(360);
    
private:
    static inline std::array<T, sin_count> sintable = []() {
        std::array<T, sin_count> table{};
        
        for (std::size_t i = 0; i < sin_count; ++i) {
            table[i] = std::sin((i + T(0.5)) / sin_count * tau<T>);
        }
        
        table[0] = T(0);
        table[static_cast<std::size_t>(T(90) * degree_to_index) & sin_mask] = T(1);
        table[static_cast<std::size_t>(T(180) * degree_to_index) & sin_mask] = T(0);
        table[static_cast<std::size_t>(T(270) * degree_to_index) & sin_mask] = T(-1);
        
        return table;
    }();
    
public:
    [[nodiscard]] static constexpr T sin(T radians) noexcept {
        return sintable[static_cast<std::size_t>(radians * radian_to_index) & sin_mask];
    }
    
    [[nodiscard]] static constexpr T cos(T radians) noexcept {
        return sintable[static_cast<std::size_t>((radians + (pi<T> / T(2))) * radian_to_index) & sin_mask];
    }
    
    [[nodiscard]] static constexpr T tan(T radians) noexcept {
        return sin(radians) / cos(radians);
    }
    
    [[nodiscard]] static constexpr T csc(T radians) noexcept {
        return T(1) / sin(radians);
    }
    
    [[nodiscard]] static constexpr T sec(T radians) noexcept {
        return T(1) / cos(radians);
    }
    
    [[nodiscard]] static constexpr T cot(T radians) noexcept {
        return cos(radians) / sin(radians);
    }
};

// Inverse Trigonometric Functions
template<typename T, std::size_t AsinBits = 16, std::size_t AtanBits = 16>
class InverseTrigLUT {
    static_assert(std::is_floating_point_v<T>, "T must be a floating point type");
    
public:
    // Asin table configuration
    static constexpr std::size_t asin_bits = AsinBits;
    static constexpr std::size_t asin_mask = (1 << asin_bits) - 1;
    static constexpr std::size_t asin_count = asin_mask + 1;
    
    // Atan table configuration
    static constexpr std::size_t atan_bits = AtanBits;
    static constexpr std::size_t atan_mask = (1 << atan_bits) - 1;
    static constexpr std::size_t atan_count = atan_mask + 1;
    static constexpr T atan_max_range = T(100);
    
private:
    // Lookup tables
    static inline std::array<T, asin_count> asintable = []() {
        std::array<T, asin_count> table{};
        for (std::size_t i = 0; i < asin_count; ++i) {
            T x = T(-1) + T(2) * i / (asin_count - T(1));
            table[i] = std::asin(x);
        }
        return table;
    }();
    
    static inline std::array<T, atan_count> atantable = []() {
        std::array<T, atan_count> table{};
        for (std::size_t i = 0; i < atan_count; ++i) {
            T x = -atan_max_range + T(2) * atan_max_range * i / (atan_count - T(1));
            table[i] = std::atan(x);
        }
        return table;
    }();
    
public:
    // Fast arcsine using lookup table
    [[nodiscard]] static T asin(T x) noexcept {
        // Clamp input to valid range
        if (x <= T(-1)) return -pi<T> / T(2);
        if (x >= T(1)) return pi<T> / T(2);
        
        // Convert x from [-1, 1] to table index [0, asin_count-1]
        T index_f = (x + T(1)) * T(0.5) * (asin_count - 1);
        std::size_t index = static_cast<std::size_t>(index_f);
        
        // Linear interpolation between table entries
        if (index < asin_count - 1) {
            T frac = index_f - index;
            return asintable[index] * (T(1) - frac) + asintable[index + 1] * frac;
        }
        
        return asintable[asin_count - 1];
    }
    
    // Fast arccosine using the identity: acos(x) = π/2 - asin(x)
    [[nodiscard]] static T acos(T x) noexcept {
        return pi<T> / T(2) - asin(x);
    }
    
    // Fast arctangent using lookup table with range extension
    [[nodiscard]] static T atan(T x) noexcept {
        // For large |x|, use the approximation: atan(x) ≈ sign(x) * π/2 - 1/x
        if (x < -atan_max_range) {
            return -pi<T> / T(2) - T(1) / x;
        }
        if (x > atan_max_range) {
            return pi<T> / T(2) - T(1) / x;
        }
        
        // Convert x from [-atan_max_range, atan_max_range] to table index
        T normalized = (x + atan_max_range) / (T(2) * atan_max_range);
        T index_f = normalized * (atan_count - 1);
        std::size_t index = static_cast<std::size_t>(index_f);
        
        // Linear interpolation
        if (index < atan_count - 1) {
            T frac = index_f - index;
            return atantable[index] * (T(1) - frac) + atantable[index + 1] * frac;
        }
        
        return atantable[atan_count - 1];
    }
};

template<typename T = float>
[[nodiscard]] constexpr inline T sin(T radians) noexcept {
    return TrigLUT<T>::sin(radians);
}

template<typename T = float>
[[nodiscard]] constexpr inline T cos(T radians) noexcept {
    return TrigLUT<T>::cos(radians);
}

template<typename T = float>
[[nodiscard]] constexpr inline T tan(T radians) noexcept {
    return TrigLUT<T>::tan(radians);
}

template<typename T = float>
[[nodiscard]] constexpr inline T asin(T x) noexcept {
    return InverseTrigLUT<T>::asin(x);
}

template<typename T = float>
[[nodiscard]] constexpr inline T acos(T x) noexcept {
    return InverseTrigLUT<T>::acos(x);
}

template<typename T = float>
[[nodiscard]] constexpr inline T atan(T x) noexcept {
    return InverseTrigLUT<T>::atan(x);
}
