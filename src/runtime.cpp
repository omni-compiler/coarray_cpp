#include "common_decl.hpp"
#include <iostream>

size_t this_image() {
  return 0;
}

void get(void *ptr, size_t size, size_t img) {
  std::cout << "get from " << img << std::endl;
}

void put(void *ptr, size_t size, size_t img) {
  std::cout << "put to " << img << std::endl;
}
