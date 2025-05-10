#pragma once

#include "sequence.hpp"
#include "arraySequence.hpp"
#include "listSequence.hpp"

template <typename T>
class SegmentedDeque : public Sequence<T>
{
private:
    // Using a ListSequence of ArraySequence pointers instead of unique_ptr
    ListSequence<ArraySequence<T>*> *segments;
    int segmentSize;
    int totalSize;

public:
    SegmentedDeque(int segmentSize = 32);
    SegmentedDeque(const SegmentedDeque<T> &other);
    ~SegmentedDeque();

    T &getFirst() override;
    T &getLast() override;
    T &get(const int index) override;

    const T &getFirst() const override;
    const T &getLast() const override;
    const T &get(const int index) const override;

    void append(const T &item) override;
    void prepend(const T &item) override;
    void insertAt(const T &item, const int index) override;
    void set(const int index, const T &data) override;
    void concat(const Sequence<T> *other) override;

    int getLength() const override;

    Sequence<T> *getSubsequence(const int startIndex, const int endIndex) const override;
    Sequence<T> *appendImmutable(const T &item) const override;
    Sequence<T> *prependImmutable(const T &item) const override;
    Sequence<T> *insertAtImmutable(const T &item, const int index) const override;
    Sequence<T> *setImmutable(const int index, const T &data) const override;
    Sequence<T> *concatImmutable(const Sequence<T> *other) const override;

    void print() const override;
};

#include "../impl/segmentedDeque.tpp"