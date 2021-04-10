#include <utility>

class RefCount {
private:
  int count;

public:
  int Retain() { return ++count; }
  int Release() { return --count; }
};

template <class T> class wraptr {
private:
  T *raw;
  RefCount *refCount;

public:
  // default constructor
  wraptr() : wraptr(new T()) {}

  // wrapping constructor
  wraptr(T *raw) : raw(raw), refCount(new RefCount()) { refCount->Retain(); }

  // copy constructor
  wraptr(const wraptr<T> &other) : raw(other.raw), refCount(other.refCount) {
    refCount->Retain();
  }

  // move constructor
  wraptr(wraptr<T> &&other)
      : raw(std::move(other.raw)), refCount(std::move(other.refCount)) {
    other.raw = nullptr;
    other.refCount = nullptr;
  }

  // destructor
  ~wraptr() {
    if (refCount != nullptr && raw != nullptr && refCount->Release() == 0) {
      delete raw;
    }
  }

  // class member access operator overload
  T *operator->() { return raw; }

  // copy assignment operator overload
  wraptr<T> &operator=(const wraptr<T> &other) {
    if (this != &other) {
      if (refCount->Release() == 0) {
        delete raw;
        delete refCount;
      }
      raw = other.raw;
      refCount = other.refCount;
      refCount->Retain();
    }
    return *this;
  }

  // move assignment operator overload
  wraptr<T> &operator=(wraptr<T> &&other) {
    if (this != &other) {
      if (refCount->Release() == 0) {
        delete raw;
        delete refCount;
      }
      raw = other.raw;
      refCount = other.refCount;
      other.raw = nullptr;
      other.refCount = nullptr;
    }
    return *this;
  }
};
