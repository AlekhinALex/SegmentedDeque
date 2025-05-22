#pragma once

#include "sequence.hpp"
#include "arraySequence.hpp"
#include "listSequence.hpp"

template <typename T>
class SegmentedDeque : public Sequence<T>
{
private:
    ListSequence<ArraySequence<T> *> *segments;
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
    int getSegmentSize() const;
    void rebalanceSegments();

    Sequence<T> *getSubsequence(const int startIndex, const int endIndex) const override;
    Sequence<T> *appendImmutable(const T &item) const override;
    Sequence<T> *prependImmutable(const T &item) const override;
    Sequence<T> *insertAtImmutable(const T &item, const int index) const override;
    Sequence<T> *setImmutable(const int index, const T &data) const override;
    Sequence<T> *concatImmutable(const Sequence<T> *other) const override;

    void print() const override;

public:
    //* { Sort
    template <class RandomIt>
    void sort(RandomIt first, RandomIt last);

    template <class RandomIt>
    SegmentedDeque<T> *sortImmutable(RandomIt first, RandomIt last);

    template <class RandomIt, class Compare>
    void sort(RandomIt first, RandomIt last, Compare compare);

    template <class RandomIt, class Compare>
    SegmentedDeque<T> *sortImmutable(RandomIt first, RandomIt last, Compare compare);

    template <class Compare>
    void mergeSort(int left, int right, Compare compare);
    //* } Sort

    //* { Map
    template <class InputIt, class OutputIt, class UnaryOp>
    OutputIt apply(InputIt first1, InputIt last1, OutputIt destFirst, UnaryOp unaryOp);

    template <class InputIt1, class InputIt2, class OutputIt, class BinaryOp>
    OutputIt apply(InputIt1 first1, InputIt1 last1, InputIt2 first2, OutputIt destFirst, BinaryOp binaryOp);
    //* { Map

    template <typename Predicate>
    SegmentedDeque<T> *where(const Predicate &pred) const;

    template <typename R, typename BinaryOp>
    R reduce(const BinaryOp &op, R init) const;

    template <class ForwardIt1, class ForwardIt2>
    bool searchSubsequence(ForwardIt1 first, ForwardIt1 last, ForwardIt2 searchFirst, ForwardIt2 searchLast) const;

public:
    class Iterator
    {
    private:
        SegmentedDeque<T> *deque;
        int index;

    public:
        Iterator(SegmentedDeque<T> *deque, const int index);
        T &operator*();
        Iterator &operator++();
        Iterator operator++(int);
        bool operator==(const Iterator &other) const;
        bool operator!=(const Iterator &other) const;
        bool notEnd() const;
    };

    class ConstIterator
    {
    private:
        const SegmentedDeque<T> *deque;
        int index;

    public:
        ConstIterator(const SegmentedDeque<T> *deque, const int index);
        const T &operator*() const;
        ConstIterator &operator++();
        ConstIterator operator++(int);
        bool operator==(const ConstIterator &other) const;
        bool operator!=(const ConstIterator &other) const;
        bool notEnd() const;
    };

    Iterator begin();
    Iterator end();
    ConstIterator cbegin() const;
    ConstIterator cend() const;
};

#include "../impl/segmentedDeque.tpp"
