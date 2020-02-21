#include "util/arrayList.hpp"
#include "runtime/universe.hpp"
#include <stdio.h>

template <typename T>
ArrayList<T>::ArrayList(int n) {
    _length = n;
    _array = new T[_length];
    _size = 0;
}

template <typename T>
void ArrayList<T>::add(T t) {
    if (_size >= _length) {
        expand();
    }
    _array[_size++] = t;
}

template <typename T>
void ArrayList<T>::insert(int index, T t) {
    add(NULL);
    for (int i = _size;i > index;i--) {
        _array[i] = _array[i - 1];
    }
    _array[index] = t;
}

template <typename T>
T ArrayList<T>::get(int index) {
    return _array[index];
}

template <typename T>
void ArrayList<T>::expand() {
    T* new_array = new T[_length << 1];
    for (int i = 0;i < _length;i++) {
        new_array[i] = _array[i];
    }
    delete[] _array;
    _array = new_array;
    _length <<= 1;
    printf("expand an array to %d, size is %d\n",_length, _size);
}

template <typename T>
int ArrayList<T>::size() {
    return _size;
}

template <typename T>
int ArrayList<T>::length() {
    return _length;
}

template <typename T>
void ArrayList<T>::set(int index,T t) {
    if (_size <= index) {
        _size = index + 1;
    }
    while (_size > _length) {
        expand();
    }
    _array[index] = t;
}

template <typename T>
T ArrayList<T>::pop() {
    return _array[--_size];
}

template <typename T>
void ArrayList<T>::delete_index(int index) {
    for (int i = index;i < _size - 1;i++) {
        _array[i] = _array[i + 1];
    }
    _size--;
}

template <typename T>
int ArrayList<T>::index(T t) {
    return 0;
}

/*
template <>
int ArrayList<PyObject*>::index(PyObject* obj) {
    for (int i = 0;i < _size;i++) {
        if (_array[i]->equal(obj) == Universe::ptrue) {
            return i;
        }
    }
    return -1;
}*/


class PyObject;
template class ArrayList<PyObject*>;

class PyString;
template class ArrayList<PyString*>;

class Klass;
template class ArrayList<Klass*>;

class Block;
template class ArrayList<Block*>;
