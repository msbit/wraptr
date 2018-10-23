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
  wraptr();
  wraptr(T *raw);
  wraptr(const wraptr<T> &other);
  wraptr(wraptr<T> &&other);
  ~wraptr();
  T *operator->() { return this->raw; }
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

template <class T> wraptr<T>::wraptr() : wraptr<T>::wraptr(new T()) {}

template <class T>
wraptr<T>::wraptr(T *raw) : raw(raw), refCount(new RefCount()) {
  this->refCount->Retain();
}

template <class T>
wraptr<T>::wraptr(const wraptr<T> &other)
    : raw(other.raw), refCount(other.refCount) {
  this->refCount->Retain();
}

template <class T>
wraptr<T>::wraptr(wraptr<T> &&other)
    : raw(std::move(other.raw)), refCount(std::move(other.refCount)) {
  other.raw = nullptr;
  other.refCount = nullptr;
}

template <class T> wraptr<T>::~wraptr() {
  if (this->refCount != nullptr && this->raw != nullptr &&
      this->refCount->Release() == 0) {
    delete this->raw;
  }
}
