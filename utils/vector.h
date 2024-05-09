#include "utils/exceptions.h"
#include <algorithm>
#include <cstdlib>
#include <initializer_list>
#include <utility>

namespace utils {
template <typename T> class vector {
    public:
    // Constructors
    vector ();
    explicit vector (int);
    vector (std::initializer_list<T>);

    // Copy Constructorcsvcsv
    vector (vector<T>&);

    // Move Constructor
    vector (vector<T>&&);

    // Destructor
    ~vector ();

    // Getters
    auto size () const {
        return _size;
    }
    auto capacity () const {
        return _capacity;
    }

    // Functionalities
    T& at (int) const;
    void push_back (T value);
    void pop_back ();
    void resize (int);
    void reserve (int);
    void clear ();
    void erase (int);
    void erase (int, int);
    void insert (int, T);
    T& front () const;
    T& back () const;

    // Operators
    T& operator[] (int);
    T& operator= (vector<T>&);
    T& operator= (vector<T>&&);

    // Iterators
    T* begin () const;
    T* end () const;

    private:
    std::size_t _capacity, _size;
    T* _elems;

    // Helpers
    void reallocate ();
    void reallocate (int);

    bool valid_index (int) const;
};

// Implementation
// Constructors

template <typename T>
vector<T>::vector () : _capacity (0), _size (0), _elems{ nullptr } {
}

template <typename T>
vector<T>::vector (std::initializer_list<T> elems)
: _capacity (elems.size ()), _size (elems.size ()), _elems{ new T[_capacity] } {
    std::copy (elems.begin (), elems.end (), _elems);
}

template <typename T>
vector<T>::vector (int capacity) : _capacity (capacity), _size (0) {
    _elems = new T[_capacity];
}

// Copy Constructor
template <typename T>
vector<T>::vector (vector<T>& copy)
: _capacity (copy.size ()), _size (copy.size ()), _elems (new T[copy.size ()]) {
    for (int i = 0; i < _size; i++)
        _elems[i] = copy[i];
}

// Move Constructor
template <typename T>
vector<T>::vector (vector<T>&& move)
: _capacity (move.size ()), _size (move.size ()), _elems{ move._elems } {
    move._capacity = move._size = 0;
    move._elems                 = 0;
}

// Destructor
template <typename T> vector<T>::~vector () {
    delete[] _elems;
}

// Helpers
template <typename t> void vector<t>::reallocate () {
    reallocate (1);
}

template <typename T> void vector<T>::reallocate (int offset) {
    if (offset == 0)
        return;
    T* temp = new T[_size + offset];
    for (int i = 0; i < _size; i++)
        temp[i] = _elems[i];
    delete[] _elems;
    _elems    = temp;
    _capacity = _size + offset;
}

template <typename T> bool vector<T>::valid_index (int index) const {
    return index >= 0 && index < _size;
}
// Functionalities
template <typename T> T& vector<T>::at (int index) const {
    if (!valid_index (index))
        throw utils::custom_exception{ "Invalid index" };

    return _elems[index];
}
template <typename T> void vector<T>::push_back (T value) {
    if (_size >= _capacity)
        reallocate ();
    _elems[_size] = value;
    _size++;
    return;
}

template <typename T> void vector<T>::pop_back () {
    if (_size == 0)
        throw utils::custom_exception{ "You cannot pop up an empty array" };

    _size--;
}

template <typename T> void vector<T>::resize (int new_size) {
    if (new_size < _size) {
        _size = new_size;
        return;
    }

    reallocate (new_size - _size);
}

template <typename T> void vector<T>::reserve (int offset) {
    reallocate (offset);
}

template <typename T> T& vector<T>::front () const {
    return at (0);
}

template <typename T> T& vector<T>::back () const {
    return at (_size - 1);
}

template <typename T> void vector<T>::clear () {
    delete[] _elems;
    _size     = 0;
    _capacity = 0;
}
template <typename T> void vector<T>::erase (int index, int count) {
    if (count < 1)
        throw utils::custom_exception{
            "Invalid count option to utils::vector::erase"
        };
    for (int i = index; i + count < _size; i++)
        _elems[i] = _elems[i + count];
    _size -= count;
}
template <typename T> void vector<T>::erase (int index) {
    erase (index, 1);
}
template <typename T> void vector<T>::insert (int index, T value) {
    if (index > _size)
        throw utils::custom_exception{
            "You cannot insert to an index > size()"
        };
    push_back (value);
    for (int i = _size - 1; i < index; i--)
        std::swap (_elems[i], _elems[i - 1]);
}
// Operators Overloading
template <typename T> T& vector<T>::operator[] (int value) {
    return at (value);
}
template <typename T> T& vector<T>::operator= (vector<T>& other) {
    _capacity = _size = other._size;
    for (int i = 0; i < _size; i++)
        _elems[i] = other[i];
}

template <typename T> T& vector<T>::operator= (vector<T>&& other) {
    _capacity = _size = other._size;
    _elems            = other._elems;
    other._elems      = nullptr;
    other._size = other._capacity = 0;
}

// Iterators
template <typename T> T* utils::vector<T>::begin () const {
    return &at (0);
};

template <typename T> T* utils::vector<T>::end () const {
    return &at (_size - 1);
};
} // namespace utils
