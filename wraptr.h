template <class T> class wraptr {
private:
  T *raw;

public:
  wraptr();
  wraptr(T *raw);
  wraptr(const wraptr<T> &other);
  ~wraptr();
  T *operator->() { return this->raw; }
};

template <class T> wraptr<T>::wraptr() : wraptr<T>::wraptr(new T()) {}

template <class T> wraptr<T>::wraptr(T *raw) : raw(raw) {}

template <class T> wraptr<T>::wraptr(const wraptr<T> &other) : raw(other.raw) {}

template <class T> wraptr<T>::~wraptr() { delete this->raw; }
