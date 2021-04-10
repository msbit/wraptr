#include <utility>

class ref_count {
private:
  int count;

public:
  int retain() { return ++count; }
  int release() { return --count; }
};

template <class T> class wraptr {
private:
  T *raw;
  ref_count *rc;

public:
  // default constructor
  wraptr() : wraptr(new T()) {}

  // wrapping constructor
  wraptr(T *raw) : raw(raw), rc(new ref_count()) { rc->retain(); }

  // copy constructor
  wraptr(const wraptr<T> &other) : raw(other.raw), rc(other.rc) {
    rc->retain();
  }

  // move constructor
  wraptr(wraptr<T> &&other)
      : raw(std::exchange(other.raw, nullptr)),
        rc(std::exchange(other.rc, nullptr)) {}

  // destructor
  ~wraptr() {
    if (rc != nullptr && raw != nullptr && rc->release() == 0) {
      delete raw;
    }
  }

  // class member access operator overload
  T *operator->() { return raw; }

  // copy assignment operator overload
  wraptr<T> &operator=(const wraptr<T> &other) {
    if (this != &other) {
      if (rc->release() == 0) {
        delete raw;
        delete rc;
      }
      raw = other.raw;
      rc = other.rc;
      rc->retain();
    }
    return *this;
  }

  // move assignment operator overload
  wraptr<T> &operator=(wraptr<T> &&other) {
    if (this != &other) {
      if (rc->release() == 0) {
        delete raw;
        delete rc;
      }
      std::swap(raw, other.raw);
      std::swap(rc, other.rc);
    }
    return *this;
  }
};
