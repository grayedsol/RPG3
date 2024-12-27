#pragma once
#include <cmath>

#ifndef uint
#define uint unsigned int
#endif

/**
 * @brief Vector type with tag dispatching.
 * 
 * @details
 * Allows for multiple types of vectors within a tuple
 * that have the same T (type) and N (size).
 * 
 * Example:
 * @code{.cpp}
 * struct Position2Tag{};
 * struct Velocity2Tag{};
 * 
 * using Position2 = GRY_VecTD<float, 2, Position2Tag>;
 * using Velocity2 = GRY_VecTD<float, 2, Velocity2Tag>;
 * 
 * std::tuple<Position2, Velocity2> components;
 * 
 * void process(std::tuple<Position2, Velocity2> components, double deltaTime) {
 * 	std::get<Position2>(components) += std::get<Velocity2>(components) * deltaTime;
 * }
 * @endcode
 * 
 * Thus, `Position2` and `Velocity2` can be used in a tuple
 * and accessed with `std::get<T>`, even though they are essentially
 * the same type underneath.
 * 
 * @tparam T Data type for the vector
 * @tparam N Number of elements
 * @tparam Tag Distinguishing type for tag dispatching
 */
template<typename T, uint N, typename Tag>
struct GRY_VecTD {
    T data[N];
    T& operator[](const uint i) { return data[i]; }
    const T& operator[](const uint i) const { return data[i]; }
};

template<typename T, typename Tag>
struct GRY_VecTD<T,3,Tag> {
    T data[3];
    GRY_VecTD() {}
    GRY_VecTD(T x, T y, T z) {
        data[0] = x; data[1] = y; data[2] = z;
    }
    T& operator[](const uint i) { return data[i]; }
    const T& operator[](const uint i) const { return data[i]; }
};

template<typename T, typename Tag>
struct GRY_VecTD<T,2,Tag> {
    T data[2];
    GRY_VecTD() {}
    GRY_VecTD(T x, T y) {
        data[0] = x; data[1] = y;
    }
    T& operator[](const uint i) { return data[i]; }
    const T& operator[](const uint i) const { return data[i]; }
};

template<typename Tag>
using Vec3f = GRY_VecTD<float,3,Tag>;

template<typename Tag>
using Vec2f = GRY_VecTD<float,2,Tag>;

template<typename T, uint N, typename Tag1, typename Tag2>
GRY_VecTD<T,N,Tag1> operator+(const GRY_VecTD<T,N,Tag1>& lhs, const GRY_VecTD<T,N,Tag2>& rhs) {
    GRY_VecTD<T,N,Tag1> ret;
    for (uint i = 0; i < N; i++) { ret[i] = lhs[i] + rhs[i]; }
    return ret;
}

template<typename T, uint N, typename Tag1, typename Tag2>
void operator+=(GRY_VecTD<T,N,Tag1>& lhs, const GRY_VecTD<T,N,Tag2>& rhs) {
    for (uint i = 0; i < N; i++) { lhs[i] += rhs[i]; }
}

template<typename T, uint N, typename Tag1, typename Tag2>
GRY_VecTD<T,N,Tag1> operator-(const GRY_VecTD<T,N,Tag1>& lhs, const GRY_VecTD<T,N,Tag2>& rhs) {
    GRY_VecTD<T,N,Tag1> ret;
    for (uint i = 0; i < N; i++) { ret[i] = lhs[i] - rhs[i]; }
    return ret;
}

template<typename T, uint N, typename Tag1, typename Tag2>
void operator-=(GRY_VecTD<T,N,Tag1>& lhs, const GRY_VecTD<T,N,Tag2>& rhs) {
    for (uint i = 0; i < N; i++) { lhs[i] -= rhs[i]; }
}

template<typename T, typename K, uint N, typename Tag>
GRY_VecTD<T,N,Tag> operator*(const GRY_VecTD<T,N,Tag>& vec, const K& scalar) {
    GRY_VecTD<T,N,Tag> ret;
    for (uint i = 0; i < N; i++) { ret[i] = vec[i] * scalar; }
    return ret;
}

template<typename T, typename K, uint N, typename Tag>
void operator*=(GRY_VecTD<T,N,Tag>& vec, const K& scalar) {
    for (uint i = 0; i < N; i++) { vec[i] *= scalar; }
}

template<typename T, uint N, typename Tag>
T GRY_VecLengthSq(const GRY_VecTD<T,N,Tag>& vec) {
    T sum = 0;
    for (uint i = 0; i < N; i++) { sum += vec[i] * vec[i]; }
    return sum;
}

template<typename T, uint N, typename Tag1, typename Tag2>
T GRY_VecDistanceSq(const GRY_VecTD<T,N,Tag1>& lhs, const GRY_VecTD<T,N,Tag2>& rhs) {
    T sum = 0;
    for (uint i = 0; i < N; i++) {
        sum += (lhs[i] - rhs[i]) * (lhs[i] - rhs[i]);
    }
    return sum;
}

template<typename T, uint N, typename Tag1, typename Tag2>
T GRY_VecDot(const GRY_VecTD<T,N,Tag1>& lhs, const GRY_VecTD<T,N,Tag2>& rhs) {
    T sum = 0;
    for (uint i = 0; i < N; i++) { sum += lhs[i] * rhs[i]; }
    return sum;
}

template<typename T, typename Tag1, typename Tag2>
GRY_VecTD<T,3,Tag1> GRY_VecCross(const GRY_VecTD<T,3,Tag1>& lhs, const GRY_VecTD<T,3,Tag2>& rhs) {
    GRY_VecTD<T,3,Tag1> ret;
    ret[0] = lhs[1]*rhs[2] - lhs[2]*rhs[1];
    ret[1] = lhs[2]*rhs[0] - lhs[0]*rhs[2];
    ret[2] = lhs[0]*rhs[1] - lhs[1]*rhs[0];
    return ret;
}

template<uint N, typename Tag>
GRY_VecTD<float,N,Tag> GRY_VecNormalize(const GRY_VecTD<float,N,Tag>& vec) {
    return vec * (1.0f / sqrtf(GRY_VecLengthSq(vec)));
}

#undef uint
