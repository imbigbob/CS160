#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H
template <typename T>
class DynamicArray {
   private:
    T* data;
    int size;
    int capacity;

    void resize(int newCapacity);

   public:
    DynamicArray();
    ~DynamicArray();
    DynamicArray(const DynamicArray& other);
    DynamicArray& operator=(const DynamicArray& other);

    void pushBack(const T& value);
    void removeAt(int index);
    T& operator[](int index);
    const T& operator[](int index) const;
    int getSize() const;
    bool empty() const;
};

#endif