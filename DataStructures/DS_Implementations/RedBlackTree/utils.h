#ifndef UTILS_H
#define UTILS_H

#include <cstddef> // std::size_t

// ===================== Defines ====================== //
#define DEBUG_MODE
//#define HAS_COLOR_MEMBER_DATA

#define RED true
#define BLACK false

// ================= Helper Functions ================= //
template <class T>
void resize_array(T**& arr, std::size_t& size) {
  T** newArr = new T*[size *= 2];
  for (std::size_t i = 0; i < size; ++i) {
    newArr[i] = arr[i];
  }
  delete [] arr;
  arr = newArr;
}

#endif /* UTILS_H */
