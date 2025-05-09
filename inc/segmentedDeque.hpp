#pragma once

#include <memory>

#include "sequence.hpp"
#include "arraySequence.hpp"
#include "listSequence.hpp"

template <typename T>
class SegmentedDeque : public Sequence<T>
{
private:
    ListSequence<std::unique_ptr<ArraySequence<T>>> *segments;
    int segmentSize;
    int totalSize;

public:
    SegmentedDeque(int segmentSize = 32);
    SegmentedDeque(const SegmentedDeque<T> &other);
    ~SegmentedDeque();

    T &getFirst();
    T &getLast();
    T &get(const int index);

    const T &getFirst() const;
    const T &getLast() const;
    const T &get(const int index) const;

    void append(const T &item);
    void prepend(const T &item);
    void insertAt(const T &item, const int index);
    void set(const int index, const T &data);
    void concat(const SegmentedDeque<T> *other);

    int getLength() const;

    SegmentedDeque<T> *getSubsequence(const int startIndex, const int endIndex) const;
    SegmentedDeque<T> *appendImmutable(const T &item) const;
    SegmentedDeque<T> *prependImmutable(const T &item) const;
    SegmentedDeque<T> *insertAtImmutable(const T &item, const int index) const;
    SegmentedDeque<T> *setImmutable(const int index, const T &data) const;
    SegmentedDeque<T> *concatImmutable(const SegmentedDeque<T> *other) const;

    void print() const;
};

#include "../impl/segmentedDeque.tpp"