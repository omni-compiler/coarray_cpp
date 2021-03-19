#include <iostream>
#include "coarray.hpp"

#define M 8
#define N 8

using namespace std;

int main(void) {
  coarray<int[M][N]> a;
  coarray<int[M][N]> b;

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      a[i][j] = i * 10 + j;
      b[i][j] = 1000;
    }
  }

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      cout << a[i][j] << "\t";
    }
    cout << "\n";
  }

  cout << "\n";

  cout << "\n";

  std::slice s0(1, 3, 1);
  std::slice s1(2, 3, 1);
  a(1)[s0][s1] = b(10)[s0][s1];

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      cout << a[i][j] << "\t";
    }
    cout << "\n";
  }

  cout << "\n";

  return 0;
}
