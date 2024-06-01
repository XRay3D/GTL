#pragma once
#include <inttypes.h>
#include <stddef.h>

namespace gtl {

// External Static Stack
template <typename T>
class stack_ext {
   public:
    stack_ext() {}
    stack_ext(T* arr, size_t capacity, size_t length = 0) {
        setBuffer(arr, capacity, length);
    }

    // подключить буфер
    void setBuffer(T* arr, size_t capacity, size_t length = 0) {
        _buf = arr;
        _cap = capacity;
        _len = min(length, capacity);
    }

    // добавить в конец
    bool push(const T& val) {
        if (!_fit(_len + 1)) return 0;
        _buf[_len++] = val;
        return 1;
    }

    // добавить в конец
    bool operator+=(const T& val) {
        return push(val);
    }

    // получить с конца и удалить
    T pop() {
        return length() ? _buf[(_len--) - 1] : T();
    }

    // прочитать с конца не удаляя
    T& peek() const {
        return _buf[_len - 1];
    }

    // добавить в начало
    bool shift(const T& val) {
        if (!_fit(_len + 1)) return 0;
        memmove((void*)(_buf + 1), (void*)(_buf), _len * sizeof(T));
        _buf[0] = val;
        _len++;
        return 1;
    }

    // получить с начала и удалить
    T unshift() {
        if (!length()) return T();
        T t = _buf[0];
        _len--;
        memmove((void*)(_buf), (void*)(_buf + 1), _len * sizeof(T));
        return t;
    }

    // прочитать с начала не удаляя
    T& unpeek() const {
        return _buf[0];
    }

    // удалить элемент. Отрицательный - с конца
    bool remove(int idx) {
        if (!length() || idx >= (int)_len || idx < -(int)_len) return 0;

        if (idx < 0) idx += _len;
        memcpy((void*)(_buf + idx), (void*)(_buf + idx + 1), (_len - idx - 1) * sizeof(T));
        _len--;
        return 1;
    }

    // вставить элемент на индекс (допускается индекс length())
    bool insert(int idx, const T& val) {
        if (idx > (int)_len || idx < -(int)_len || !_fit(_len + 1)) return 0;

        if (idx < 0) idx += _len;
        if (idx == 0) return shift(val);
        else if (idx == (int)_len) return push(val);

        memmove((void*)(_buf + idx + 1), (void*)(_buf + idx), (_len - idx) * sizeof(T));
        _buf[idx] = val;
        _len++;
        return 1;
    }

    // прибавить другой массив в конец
    bool concat(const stack_ext& st) {
        if (!st.length()) return 1;
        if (!_fit(_len + st._len)) return 0;

        memcpy((void*)(_buf + _len), (void*)(st._buf), st._len * sizeof(T));
        _len += st._len;
        return 1;
    }

    // прибавить другой массив в конец
    bool operator+=(const stack_ext& st) {
        return concat(st);
    }

    // заполнить значением (на capacity)
    void fill(const T& val) {
        for (size_t i = 0; i < capacity(); i++) _buf[i] = val;
        _len = _cap;
    }

    // инициализировать, вызвать конструкторы (на capacity)
    void init() {
        fill(T());
    }

    // очистить (установить длину 0)
    void clear() {
        _len = 0;
    }

    // количество элементов
    size_t length() const {
        return _buf ? _len : 0;
    }

    // установить количество элементов
    void setLength(size_t newlen) {
        if (_buf) _len = newlen;
    }

    // есть место для добавления
    bool canAdd() const {
        return _buf ? (_len < _cap) : 0;
    }

    // вместимость, элементов
    size_t capacity() const {
        return _buf ? _cap : 0;
    }

    // позиция элемента (-1 если не найден)
    int indexOf(const T& val) const {
        if (_buf) {
            for (size_t i = 0; i < _len; i++) {
                if (_buf[i] == val) return i;
            }
        }
        return -1;
    }

    // содержит элемент
    bool includes(const T& val) const {
        return indexOf(val) != -1;
    }

    // удалить по значению (true если элемента нет)
    bool removeByVal(const T& val) {
        int i = indexOf(val);
        return (i >= 0) ? remove(i) : true;
    }

    // получить элемент под индексом. Отрицательный - с конца
    T& get(int idx) const {
        if (idx < 0) return _buf[(idx + _len >= 0) ? (idx + _len) : 0];
        return _buf[idx];
    }

    // получить элемент под индексом. Отрицательный - с конца
    T& operator[](int idx) const {
        return get(idx);
    }

    // доступ к буферу
    inline T* buf() const {
        return _buf;
    }

    // буфер существует
    bool valid() const {
        return _buf;
    }

    // буфер существует
    explicit operator bool() const {
        return _buf;
    }

   protected:
    T* _buf = nullptr;
    size_t _len = 0;
    size_t _cap = 0;

    inline bool _fit(size_t size) {
        return (size <= _cap || reserve(size)) && _buf;
    }
    virtual bool reserve(size_t size) {
        return 0;
    }
};

}  // namespace gtl