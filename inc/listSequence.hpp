#pragma once
#include "sequence.hpp"
#include "linkedList.hpp"

template <class T>
class ListSequence : public Sequence<T>
{
private:
    LinkedList<T> list;

public:
    using Iterator = typename LinkedList<T>::Iterator;
    using ConstIterator = typename LinkedList<T>::ConstIterator;

    Iterator begin() { return list.begin(); }
    Iterator end() { return list.end(); }

    ConstIterator begin() const { return list.begin(); }
    ConstIterator end() const { return list.end(); }

public:
    ListSequence();
    ListSequence(const T *items, const int count);
    ListSequence(const int count);
    ListSequence(const LinkedList<T> &list);
    virtual ~ListSequence() override;

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
    Sequence<T> *insertAtImmutable(const T &item, int index) const override;
    Sequence<T> *setImmutable(const int index, const T &data) const override;
    Sequence<T> *concatImmutable(const Sequence<T> *other) const override;

    void print() const override;

    void clear();
    ListSequence<T> &operator=(const ListSequence<T> &other);
};

#include "../impl/listSequence.tpp"