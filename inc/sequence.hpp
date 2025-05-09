#pragma once

template <class T>
class Sequence
{
public:
    virtual ~Sequence() = default;

    virtual T &getFirst() = 0;
    virtual T &getLast() = 0;
    virtual T &get(const int index) = 0;

    virtual const T &getFirst() const = 0;
    virtual const T &getLast() const = 0;
    virtual const T &get(const int index) const = 0;

    virtual void append(const T &item) = 0;
    virtual void prepend(const T &item) = 0;
    virtual void insertAt(const T &item, const int index) = 0;
    virtual void set(const int index, const T &data) = 0;
    virtual void concat(const Sequence<T> *list) = 0;

    virtual int getLength() const = 0;

    virtual Sequence<T> *getSubsequence(const int startIndex, const int endIndex) const = 0;
    virtual Sequence<T> *appendImmutable(const T &item) const = 0;
    virtual Sequence<T> *prependImmutable(const T &item) const = 0;
    virtual Sequence<T> *insertAtImmutable(const T &item, const int index) const = 0;
    virtual Sequence<T> *setImmutable(const int index, const T &data) const = 0;
    virtual Sequence<T> *concatImmutable(const Sequence<T> *list) const = 0;

    virtual void print() const = 0;
};