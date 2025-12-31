#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <utility> // std::move
#include <cassert>

template <typename T>
class DynamicArray
{
private:
    T *data;
    int size;
    int capacity;

    void resize(int newCapacity)
    {
        T *newData = new T[newCapacity];

        // MOVE elements (like std::vector does)
        for (int i = 0; i < size; ++i)
            newData[i] = std::move(data[i]);

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    // ===== Constructors / Destructor =====

    DynamicArray()
        : data(new T[2]), size(0), capacity(2) {}

    ~DynamicArray()
    {
        delete[] data;
    }

    // COPY constructor (only works if T is copyable)
    DynamicArray(const DynamicArray &other)
        : data(new T[other.capacity]),
          size(other.size),
          capacity(other.capacity)
    {
        for (int i = 0; i < size; ++i)
            data[i] = other.data[i];
    }

    // COPY assignment
    DynamicArray &operator=(const DynamicArray &other)
    {
        if (this != &other)
        {
            T *newData = new T[other.capacity];

            for (int i = 0; i < other.size; ++i)
                newData[i] = other.data[i];

            delete[] data;
            data = newData;
            size = other.size;
            capacity = other.capacity;
        }
        return *this;
    }

    // MOVE constructor
    DynamicArray(DynamicArray &&other)
        : data(other.data),
          size(other.size),
          capacity(other.capacity)
    {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    // MOVE assignment
    DynamicArray &operator=(DynamicArray &&other)
    {
        if (this != &other)
        {
            delete[] data;

            data = other.data;
            size = other.size;
            capacity = other.capacity;

            other.data = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }

    // ===== Capacity =====

    int getSize() const { return size; }
    bool empty() const { return size == 0; }

    void clear()
    {
        delete[] data;
        capacity = 2;
        size = 0;
        data = new T[capacity];
    }

    // ===== Element access =====

    T &operator[](int index)
    {
        assert(index >= 0 && index < size);
        return data[index];
    }

    const T &operator[](int index) const
    {
        assert(index >= 0 && index < size);
        return data[index];
    }

    // ===== Modifiers =====

    // Pass-by-value enables move OR copy automatically
    void pushBack(T value)
    {
        if (size >= capacity)
            resize(capacity * 2);

        data[size++] = std::move(value);
    }

    void popBack()
    {
        if (size > 0)
            --size;
    }

    void removeAt(int index)
    {
        if (index < 0 || index >= size)
            return;

        for (int i = index; i < size - 1; ++i)
            data[i] = std::move(data[i + 1]);

        --size;
    }

    void setValue(int index, T value)
    {
        if (index < 0 || index >= size)
            return;

        data[index] = std::move(value);
    }

    void getValue(int index) const
    {
        if (index < 0 || index >= size)
            return;

        return data[index];
    }
};

#endif
