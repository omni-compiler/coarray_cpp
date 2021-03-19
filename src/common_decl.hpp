#pragma once

#include <valarray>

template <typename T>
class coarray;

template <typename T>
class coref;

extern size_t this_image();
extern void get(void *ptr, std::size_t size, std::size_t img);
extern void put(void *ptr, std::size_t size, std::size_t img);
