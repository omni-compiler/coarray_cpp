#pragma once

#include <valarray>
#include "common_decl.hpp"

template <typename T, std::size_t S>
class coarray<T[S]> {
private:
  T buffer[S];

public:
  coref<T[S]> operator()(std::size_t img) {
    return coref<T[S]>(img, buffer);
  }

  auto &operator[](std::size_t idx) {
    return buffer[idx];
  }

  coref<T[S]> operator[](std::slice &slc) {
    return coref<T[S]>(this_image(), buffer)[slc];
  }
};

template <typename T, std::size_t S0, std::size_t S1>
class coarray<T[S0][S1]> {
private:
  T buffer[S0][S1];

public:
  coref<T[S0][S1]> operator()(std::size_t img) {
    return coref<T[S0][S1]>(img, buffer);
  }

  auto &operator[](std::size_t idx) {
    return buffer[idx];
  }

  coref<T[S0][S1]> operator[](std::slice slc) {
    return coref<T[S0][S1]>(this_image(), buffer)[slc];
  }
};

template <typename T, std::size_t S0, std::size_t S1, std::size_t S2>
class coarray<T[S0][S1][S2]> {
private:
  T buffer[S0][S1][S2];

public:
  coref<T[S0][S1][S2]> operator()(std::size_t img) {
    return coref<T[S0][S1][S2]>(img, buffer);
  }

  auto &operator[](std::size_t idx) {
    return buffer[idx];
  }

  coref<T[S0][S1][S2]> operator[](std::slice slc) {
    return coref<T[S0][S1][S2]>(this_image(), buffer)[slc];
  }
};
