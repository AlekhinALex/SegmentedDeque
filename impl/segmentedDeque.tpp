#include <iostream>

#include "../inc/segmentedDeque.hpp"

template <typename T>
SegmentedDeque<T>::SegmentedDeque(int segmentSize)
    : segmentSize(segmentSize), totalSize(0)
{
    if (segmentSize <= 0)
    {
        throw std::invalid_argument("Segment size must be positive");
    }
    segments = new ListSequence<ArraySequence<T> *>();
}

template <typename T>
SegmentedDeque<T>::SegmentedDeque(const SegmentedDeque<T> &other)
    : segmentSize(other.segmentSize), totalSize(other.totalSize)
{
    segments = new ListSequence<ArraySequence<T> *>();

    for (int i = 0; i < other.segments->getLength(); i++)
    {
        ArraySequence<T> *newSegment = new ArraySequence<T>();
        ArraySequence<T> *otherSegment = other.segments->get(i);

        for (int j = 0; j < otherSegment->getLength(); j++)
        {
            newSegment->append(otherSegment->get(j));
        }

        segments->append(newSegment);
    }
}

template <typename T>
SegmentedDeque<T>::~SegmentedDeque()
{
    for (int i = 0; i < segments->getLength(); i++)
    {
        delete segments->get(i);
    }
    delete segments;
}

template <typename T>
T &SegmentedDeque<T>::getFirst()
{
    if (totalSize == 0)
    {
        throw std::out_of_range("Deque is empty");
    }
    return segments->getFirst()->getFirst();
}

template <typename T>
const T &SegmentedDeque<T>::getFirst() const
{
    if (totalSize == 0)
    {
        throw std::out_of_range("Deque is empty");
    }
    return segments->getFirst()->getFirst();
}

template <typename T>
T &SegmentedDeque<T>::getLast()
{
    if (totalSize == 0)
    {
        throw std::out_of_range("Deque is empty");
    }
    return segments->getLast()->getLast();
}

template <typename T>
const T &SegmentedDeque<T>::getLast() const
{
    if (totalSize == 0)
    {
        throw std::out_of_range("Deque is empty");
    }
    return segments->getLast()->getLast();
}

template <typename T>
T &SegmentedDeque<T>::get(int index)
{
    if (totalSize == 0)
    {
        throw std::out_of_range("Deque is empty");
    }

    if (index < 0 || index >= totalSize)
    {
        throw std::out_of_range("Index out of range");
    }

    int segmentIndex = index / segmentSize;
    int segmentPosition = index % segmentSize;

    return segments->get(segmentIndex)->get(segmentPosition);
}

template <typename T>
const T &SegmentedDeque<T>::get(int index) const
{
    if (totalSize == 0)
    {
        throw std::out_of_range("Deque is empty");
    }

    if (index < 0 || index >= totalSize)
    {
        throw std::out_of_range("Index out of range");
    }

    int segmentIndex = index / segmentSize;
    int segmentPosition = index % segmentSize;

    return segments->get(segmentIndex)->get(segmentPosition);
}

template <typename T>
void SegmentedDeque<T>::append(const T &item)
{
    if (segments->getLength() == 0 || segments->getLast()->getLength() >= segmentSize)
    {
        segments->append(new ArraySequence<T>());
    }

    segments->getLast()->append(item);
    totalSize++;
}

template <typename T>
void SegmentedDeque<T>::prepend(const T &item)
{
    if (segments->getLength() == 0 || segments->getFirst()->getLength() >= segmentSize)
    {
        segments->prepend(new ArraySequence<T>());
    }

    segments->getFirst()->prepend(item);
    totalSize++;
}

template <typename T>
void SegmentedDeque<T>::insertAt(const T &item, const int index)
{
    if (index < 0 || index > totalSize)
    {
        throw std::out_of_range("Index is out of range");
    }

    if (index == 0)
    {
        prepend(item);
        return;
    }

    if (index == totalSize)
    {
        append(item);
        return;
    }

    int segmentIndex = index / segmentSize;
    int segmentPosition = index % segmentSize;

    segments->get(segmentIndex)->insertAt(item, segmentPosition);
    totalSize++;
}

template <typename T>
void SegmentedDeque<T>::set(const int index, const T &data)
{
    if (index < 0 || index >= totalSize)
    {
        throw std::out_of_range("Index is out of range");
    }

    int segmentIndex = index / segmentSize;
    int segmentPosition = index % segmentSize;

    segments->get(segmentIndex)->set(segmentPosition, data);
}

template <typename T>
void SegmentedDeque<T>::concat(const Sequence<T> *other)
{
    if (!other)
    {
        return;
    }

    for (int i = 0; i < other->getLength(); ++i)
    {
        append(other->get(i));
    }
}

template <typename T>
int SegmentedDeque<T>::getLength() const
{
    return totalSize;
}

template <typename T>
Sequence<T> *SegmentedDeque<T>::getSubsequence(const int startIndex, const int endIndex) const
{
    int size = getLength();
    if (startIndex < 0 || startIndex >= size ||
        endIndex < 0 || endIndex >= size ||
        startIndex > endIndex)
    {
        throw std::out_of_range("Invalid index range");
    }

    auto *newDq = new SegmentedDeque<T>(segmentSize);

    for (int i = startIndex; i <= endIndex; i++)
    {
        newDq->append(get(i));
    }

    return newDq;
}

template <typename T>
Sequence<T> *SegmentedDeque<T>::appendImmutable(const T &item) const
{
    auto *newDq = new SegmentedDeque<T>(*this);
    newDq->append(item);
    return newDq;
}

template <typename T>
Sequence<T> *SegmentedDeque<T>::prependImmutable(const T &item) const
{
    auto *newDq = new SegmentedDeque<T>(*this);
    newDq->prepend(item);
    return newDq;
}

template <typename T>
Sequence<T> *SegmentedDeque<T>::insertAtImmutable(const T &item, const int index) const
{
    auto *newDq = new SegmentedDeque<T>(*this);
    newDq->insertAt(item, index);
    return newDq;
}

template <typename T>
Sequence<T> *SegmentedDeque<T>::setImmutable(const int index, const T &data) const
{
    auto *newDq = new SegmentedDeque<T>(*this);
    newDq->set(index, data);
    return newDq;
}

template <typename T>
Sequence<T> *SegmentedDeque<T>::concatImmutable(const Sequence<T> *other) const
{
    auto *newDq = new SegmentedDeque<T>(*this);
    newDq->concat(other);
    return newDq;
}

template <typename T>
void SegmentedDeque<T>::print() const
{
    if (totalSize == 0)
    {
        std::cout << "Empty";
        return;
    }

    for (int i = 0; i < segments->getLength(); i++)
    {
        segments->get(i)->print();
    }
}