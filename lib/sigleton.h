#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <memory>

// Singleton pattern encapsulation class.
// @param T Type
// @param X In order to create tags corresponding to multiple instances
// @param N Create multiple instance indexes with the same Tag
template <class T, class X = void, int N = 0> class Singleton {
public:
  // Return singleton raw pointer.
  static T *GetInstance() {
    static T v;
    return &v;
  }
};

// Singleton pattern smart pointer encapsulation class.
// @param T Type
// @param X In order to create tags corresponding to multiple instances
// @param N Create multiple instance indexes with the same Tag
template <class T, class X = void, int N = 0> class SingletonPtr {
public:
  // Return singleton smart pointer.
  static std::shared_ptr<T> GetInstance() {
    static std::shared_ptr<T> v(new T);
    return v;
  }
};

#endif