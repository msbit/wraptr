#include <utility>

class RefCount {
private:
  int count;

public:
  int Retain() { return ++this->count; }
  int Release() { return --this->count; }
};

template <class T> class wraptr {
private:
  T *raw;
  RefCount *refCount;

public:
  // default constructor declaration
  wraptr();

  // wrapping constructor declaration
  wraptr(T *raw);

  // copy constructor declaration
  wraptr(const wraptr<T> &other);

  // move constructor declaration
  wraptr(wraptr<T> &&other);

  // destructor declaration
  ~wraptr();

  // class member access operator overload
  T *operator->() { return this->raw; }

  // copy assignment operator overload
  wraptr<T> &operator=(const wraptr<T> &other) {
    if (this != &other) {
      if (this->refCount->Release() == 0) {
        delete this->raw;
        delete this->refCount;
      }
      this->raw = other.raw;
      this->refCount = other.refCount;
      this->refCount->Retain();
    }
    return *this;
  }

  // move assignment operator overload
  wraptr<T> &operator=(wraptr<T> &&other) {
    if (this != &other) {
      if (this->refCount->Release() == 0) {
        delete this->raw;
        delete this->refCount;
      }
      this->raw = other.raw;
      this->refCount = other.refCount;
      other.raw = nullptr;
      other.refCount = nullptr;
    }
    return *this;
  }
};

// default constructor definition
template <class T> wraptr<T>::wraptr() : wraptr<T>::wraptr(new T()) {}

// wrapping constructor definition
template <class T>
wraptr<T>::wraptr(T *raw) : raw(raw), refCount(new RefCount()) {
  this->refCount->Retain();
}

// copy constructor definition
template <class T>
wraptr<T>::wraptr(const wraptr<T> &other)
    : raw(other.raw), refCount(other.refCount) {
  this->refCount->Retain();
}

// move constructor definition
template <class T>
wraptr<T>::wraptr(wraptr<T> &&other)
    : raw(std::move(other.raw)), refCount(std::move(other.refCount)) {
  other.raw = nullptr;
  other.refCount = nullptr;
}

// destructor definition
template <class T> wraptr<T>::~wraptr() {
  if (this->refCount != nullptr && this->raw != nullptr &&
      this->refCount->Release() == 0) {
    delete this->raw;
  }
}
