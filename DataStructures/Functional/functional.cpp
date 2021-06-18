#ifndef FUNCTIONAL_CPP
#define FUNCTIONAL_CPP

#include <memory>
#include <utility>

namespace Functional {
  
  template <class ContainerType, class Lambda>
  void map(ContainerType& c, Lambda&& f) {
    for(class ContainerType::iterator it = c.begin(); it != c.end(); ++it) {
      std::forward<Lambda>(f)(*it);
    }
  }

  template <class T,
            template <class, class = std::allocator<T>> class Container,
            class Lambda>
  T reduce(const Container<T>& c, T nv, Lambda&& f) {
    T result = nv;
    Lambda&& func = std::forward<Lambda>(f);
    for(auto it = c.cbegin(); it != c.cend(); ++it) {
      result = func (result, *it);
    }
    return result;
  }
  
  template <class T, class ConstIt, class Lambda>
  T reduce(ConstIt beg, ConstIt end, T nv, Lambda&& f) {
    if(beg == end) {
      return nv;
    }
    const T& curr = *beg;
    return std::forward<Lambda>(f)(curr, reduce(++beg, end, nv, f));
  }

}
#endif // FUNCTIONAL_CPP
