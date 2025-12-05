
#include "DynamicArray.hpp"

#include <iostream>

template <typename T>
void DynamicArray<T>::resize(int newCapacity) {
    T* newData = new T[newCapacity];
    for (int i = 0; i < size; i++) {
        newData[i] = data[i];  // copy
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

template <typename T>
DynamicArray<T>::DynamicArray() {
    size = 0;
    capacity = 2;  // small but expandable
    data = new T[capacity];
}

template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}
template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& other) {
    size = other.size;
    capacity = other.capacity;
    data = new T[capacity];
    for (int i = 0; i < size; i++) {
        data[i] = other.data[i];
    }
}

// Assignment operator
template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& other) {
    if (this != &other) {
        delete[] data;

        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];

        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

template <typename T>
void DynamicArray<T>::pushBack(const T& value) {
    if (size >= capacity) {
        resize(capacity * 2);
    }
    data[size] = value;
    size++;
}

template <typename T>
void DynamicArray<T>::removeAt(int index) {
    if (index < 0 || index >= size) return;

    for (int i = index; i < size - 1; i++) {
        data[i] = data[i + 1];
    }
    size--;
}

template <typename T>
T& DynamicArray<T>::operator[](int index) {
    // No bounds error printing (cleaner)
    return data[index];
}

template <typename T>
const T& DynamicArray<T>::operator[](int index) const {
    // Optional: Add debug check
    // assert(index >= 0 && index < this->size);
    return data[index];
}
template <typename T>
int DynamicArray<T>::getSize() const {
    return size;
}

template <typename T>
bool DynamicArray<T>::empty() const {
    return size == 0;
}