#pragma once

template <typename T>
class DynamicArray
{
private:
    T *data;
    int size;
    int capacity;

public:
    DynamicArray();
    DynamicArray(const int size);
    DynamicArray(const T *items, const int count);
    DynamicArray(const DynamicArray<T> &dynamicArray);
    ~DynamicArray();

    T &getFirst();
    T &getLast();
    T &get(const int index);

    const T &getFirst() const;
    const T &getLast() const;
    const T &get(const int index) const;

    int getSize() const;

    void append(const T &item);
    void prepend(const T &item);
    void set(const int index, const T &value);
    void insertAt(const T &item, const int index);
    void resize(const int newSize);
    void print() const;
    void clear();

    void concat(DynamicArray<T> *dynamicArray);
    DynamicArray<T> *concatImmutable(DynamicArray<T> *dynamicArray);
    DynamicArray<T> *getSubArray(const int startIndex, const int endIndex);

    T &operator[](int index);
    const T &operator[](int index) const;
    DynamicArray<T> &operator=(const DynamicArray<T> &other);
};

#include "../impl/dynamicArray.tpp"