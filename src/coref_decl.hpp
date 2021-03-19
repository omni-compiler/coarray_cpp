#pragma once

#include <cassert>
#include "common_decl.hpp"

template <typename T>
class coref {
private:
  std::size_t image;
  T &ref_val;

public:
  coref<T>(std::size_t img, T &v)
    : image(img), ref_val(v) {
  }

  operator T() const {
    if (image == this_image()) {
      return ref_val;
    } else {
      T buf;
      get(&buf, sizeof(T), image);
      return buf;
    }
  }

  coref<T> &operator=(T v) {
    if (image == this_image()) {
      ref_val = v;
    } else {
      put(&v, sizeof(T), image);
    }

    return *this;
  }

  coref<T> &operator=(coref<T> &r) {
    T buf = r;
    *this = buf;
    return *this;
  }
};

template <typename T, std::size_t S>
class coref<T[S]> {
private:
  std::size_t image;

  unsigned dim;
  size_t num_elmts;

  void RefSlice(std::slice &slc) {
    assert(dim != 0);

    lower  = slc.start();
    upper  = slc.start() + (slc.size() * slc.stride());
    stride = slc.stride();

    num_elmts = slc.size();

    dim--;
  }

public:
  T (&ref_val)[S];

  std::size_t lower;
  std::size_t upper;
  std::size_t stride;

  size_t getImage() const {
    return image;
  }

  bool isCompleteRef() const {
    return dim == 0;
  }

  size_t countNumElmts() const {
    assert(isCompleteRef());

    return num_elmts;
  }

  coref<T[S]>(std::size_t img, T (&v)[S])
    : image(img), ref_val(v), dim(1) {
  }

  coref<T> operator[](std::size_t idx) {
    assert (dim == 1);

    return coref<T>(image, ref_val[idx]);
  }

  coref<T[S]> &operator[](std::slice &slc) {
    RefSlice(slc);
    return *this;
  }

  coref<T[S]> &operator=(T v) {
    assert(isCompleteRef());

    if (image == this_image()) {
      for (size_t i = lower; i < upper; i += stride) {
        ref_val[i] = v;
      }
    } else {
      put(&v, sizeof(T), image);
    }

    return *this;
  }

  coref<T[S]> &operator=(const coref<T[S]> &that){
    assert(isCompleteRef() && that.isCompleteRef());

    if (image == that.getImage()) {
      assert(this->countNumElmts() == that.countNumElmts());

      T *buf_start_ptr = new T[that.countNumElmts()];

      T *pack_ptr = buf_start_ptr;
      for (size_t i = that.lower; i < that.upper; i += that.stride) {
        *pack_ptr = that.ref_val[i];
        pack_ptr++;
      }

      pack_ptr = buf_start_ptr;
      for (size_t i = lower; i < upper; i += stride) {
        ref_val[i] = *pack_ptr;
        pack_ptr++;
      }

      delete[] buf_start_ptr;
    } else {
      get(&ref_val[0], sizeof(T), that.getImage());
      put(&ref_val[0], sizeof(T), image);
    }

    return *this;
  }
};

template <typename T, std::size_t S0, std::size_t S1>
class coref<T[S0][S1]> {
private:
  std::size_t image;

  unsigned dim;
  size_t num_elmts;

  void RefSlice(std::slice &slc) {
    assert(dim != 0);

    lower[2 - dim]  = slc.start();
    upper[2 - dim]  = slc.start() + (slc.size() * slc.stride());
    stride[2 - dim] = slc.stride();

    num_elmts *= slc.size();

    dim--;
  }

public:
  T (&ref_val)[S0][S1];

  std::size_t lower[2];
  std::size_t upper[2];
  std::size_t stride[2];

  size_t getImage() const {
    return image;
  }

  bool isCompleteRef() const {
    return dim == 0;
  }

  size_t countNumElmts() const {
    assert(isCompleteRef());

    return num_elmts;
  }

  coref<T[S0][S1]>(std::size_t img, T (&v)[S0][S1])
    : image(img), ref_val(v), dim(2), num_elmts(1) {
  }

  coref<T[S1]> operator[](std::size_t idx) {
    assert(dim == 2);

    return coref<T[S1]>(image, ref_val[idx]);
  }

  coref<T[S0][S1]> &operator[](std::slice &slc) {
    RefSlice(slc);
    return *this;
  }

  coref<T[S0][S1]> &operator=(T v) {
    assert(isCompleteRef());

    if (image == this_image()) {
      for (size_t i = lower[0]; i < upper[0]; i += stride[0]) {
        for (size_t j = lower[1]; j < upper[1]; j += stride[1]) {
          ref_val[i][j] = v;
        }
      }
    } else {
      put(&v, sizeof(T), image);
    }

    return *this;
  }

  coref<T[S0][S1]> &operator=(const coref<T[S0][S1]> &that){
    assert(isCompleteRef() && that.isCompleteRef());

    if (image == that.getImage()) {
      assert(this->countNumElmts() == that.countNumElmts());

      T *buf_start_ptr = new T[that.countNumElmts()];

      T *pack_ptr = buf_start_ptr;
      for (size_t i = that.lower[0]; i < that.upper[0]; i += that.stride[0]) {
        for (size_t j = that.lower[1]; j < that.upper[1]; j += that.stride[1]) {
          *pack_ptr = that.ref_val[i][j];
          pack_ptr++;
        }
      }

      pack_ptr = buf_start_ptr;
      for (size_t i = lower[0]; i < upper[0]; i += stride[0]) {
        for (size_t j = lower[1]; j < upper[1]; j += stride[1]) {
          ref_val[i][j] = *pack_ptr;
          pack_ptr++;
        }
      }

      delete[] buf_start_ptr;
    } else {
      get(&ref_val[0][0], sizeof(T), that.getImage());
      put(&ref_val[0][0], sizeof(T), image);
    }

    return *this;
  }
};

template <typename T, std::size_t S0, std::size_t S1, std::size_t S2>
class coref<T[S0][S1][S2]> {
private:
  std::size_t image;

  unsigned dim;
  size_t num_elmts;

  void RefSlice(std::slice &slc) {
    assert(dim != 0);

    lower[3 - dim]  = slc.start();
    upper[3 - dim]  = slc.start() + (slc.size() * slc.stride());
    stride[3 - dim] = slc.stride();

    num_elmts *= slc.size();

    dim--;
  }

public:
  T (&ref_val)[S0][S1][S2];

  std::size_t lower[3];
  std::size_t upper[3];
  std::size_t stride[3];

  size_t getImage() const {
    return image;
  }

  bool isCompleteRef() const {
    return dim == 0;
  }

  size_t countNumElmts() const {
    assert(isCompleteRef());

    return num_elmts;
  }

  coref<T[S0][S1][S2]>(std::size_t img, T (&v)[S0][S1][S2])
    : image(img), ref_val(v), dim(3), num_elmts(1) {
  }

  coref<T[S1][S2]> operator[](std::size_t idx) {
    assert(dim == 3);

    return coref<T[S1][S2]>(image, ref_val[idx]);
  }

  coref<T[S0][S1][S2]> &operator[](std::slice &slc) {
    RefSlice(slc);
    return *this;
  }

  coref<T[S0][S1][S2]> &operator=(T v) {
    assert(isCompleteRef());

    if (image == this_image()) {
      for (size_t i = lower[0]; i < upper[0]; i += stride[0]) {
        for (size_t j = lower[1]; j < upper[1]; j += stride[1]) {
          for (size_t k = lower[2]; k < upper[2]; k += stride[2]) {
            ref_val[i][j][k] = v;
          }
        }
      }
    } else {
      put(&v, sizeof(T), image);
    }

    return *this;
  }

  coref<T[S0][S1][S2]> &operator=(const coref<T[S0][S1][S2]> &that){
    assert(isCompleteRef() && that.isCompleteRef());

    if (image == that.getImage()) {
      assert(this->countNumElmts() == that.countNumElmts());

      T *buf_start_ptr = new T[that.countNumElmts()];

      T *pack_ptr = buf_start_ptr;
      for (size_t i = that.lower[0]; i < that.upper[0]; i += that.stride[0]) {
        for (size_t j = that.lower[1]; j < that.upper[1]; j += that.stride[1]) {
          for (size_t k = that.lower[2]; k < that.upper[2]; k += that.stride[2]) {
            *pack_ptr = that.ref_val[i][j][k];
            pack_ptr++;
          }
        }
      }

      pack_ptr = buf_start_ptr;
      for (size_t i = lower[0]; i < upper[0]; i += stride[0]) {
        for (size_t j = lower[1]; j < upper[1]; j += stride[1]) {
          for (size_t k = lower[2]; k < upper[2]; k += stride[2]) {
            ref_val[i][j][k] = *pack_ptr;
            pack_ptr++;
          }
        }
      }

      delete[] buf_start_ptr;
    } else {
      get(&ref_val[0][0][0], sizeof(T), that.getImage());
      put(&ref_val[0][0][0], sizeof(T), image);
    }

    return *this;
  }
};
