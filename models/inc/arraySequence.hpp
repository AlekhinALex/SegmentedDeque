#pragma once
#include "sequence.hpp"
#include "dynamicArray.hpp"

template <class T>
class ArraySequence : public Sequence<T>
{
private:
    DynamicArray<T> array;

public:
    ArraySequence();
    ArraySequence(const T *items, int count);
    ArraySequence(const int count);
    ArraySequence(const DynamicArray<T> &array);
    virtual ~ArraySequence() override;

    T &getFirst() override;
    T &getLast() override;
    T &get(const int index) override;

    const T &getFirst() const override;
    const T &getLast() const override;
    const T &get(const int index) const override;

    int getLength() const override;

    void append(const T &item) override;
    void prepend(const T &item) override;
    void insertAt(const T &item, int index) override;
    void set(const int index, const T &data) override;
    void concat(const Sequence<T> *other) override;

    Sequence<T> *getSubsequence(const int startIndex, const int endIndex) const override;
    Sequence<T> *appendImmutable(const T &item) const override;
    Sequence<T> *prependImmutable(const T &item) const override;
    Sequence<T> *insertAtImmutable(const T &item, const int index) const override;
    Sequence<T> *setImmutable(const int index, const T &data) const override;
    Sequence<T> *concatImmutable(const Sequence<T> *other) const override;

    void print() const override;

    void clear();
    ArraySequence<T> &operator=(const ArraySequence<T> &other);
    T &operator[](const int index);
    const T &operator[](const int index) const;
};

#include "../impl/arraySequence.tpp"