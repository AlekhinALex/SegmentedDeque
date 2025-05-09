#include <iostream>

#include "../inc/segmentedDeque.hpp"

// TODO: make Empty Check as func

template <typename T>
SegmentedDeque<T>::SegmentedDeque(int segmentSize)
    : segmentSize(segmentSize), totalSize(0)
{
    segments = new ListSequence<std::unique_ptr<ArraySequence<T>>>();
}

template <typename T>
SegmentedDeque<T>::SegmentedDeque(const SegmentedDeque<T> &other)
    : segmentSize(other.segmentSize), totalSize(other.totalSize)
{
    segments = new ListSequence<std::unique_ptr<ArraySequence<T>>>();

    for (int i = 0; i < other.segments->getLength(); i++)
    {
        auto newSegment = std::make_unique<ArraySequence<T>>();
        const auto &otherSegment = other.segments->get(i);

        for (int j = 0; j < otherSegment->getLength(); j++)
        {
            newSegment->append(otherSegment->get(j));
        }

        segments->append(std::move(newSegment));
    }
}

template <typename T>
SegmentedDeque<T>::~SegmentedDeque()
{
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

    int segmentIndex = index / segmentSize;
    int segmentPosition = index % segmentSize;

    return segments->get(segmentIndex)->get(segmentPosition);
}

template <typename T>
const T &SegmentedDeque<T>::get(int index) const
{
    int segmentIndex = index / segmentSize;
    int segmentPosition = index % segmentSize;

    return segments->get(segmentIndex)->get(segmentPosition);
}

template <typename T>
void SegmentedDeque<T>::append(const T &item)
{
    int segmentNumber = totalSize / segmentSize;
    int segmentIndex = totalSize % segmentSize;

    if (segments->getLength() == 0 || segmentIndex == 0)
    {
        segments->append(std::make_unique<ArraySequence<T>>(segmentSize));
    }

    segments->getLast()->append(item);
    totalSize++;
}

template <typename T>
void SegmentedDeque<T>::prepend(const T &item)
{
    int segmentNumber = totalSize / segmentSize;
    int segmentIndex = totalSize % segmentSize;

    if (segments->getLength() == 0 || segmentIndex == 0)
    {
        segments->prepend(std::make_unique<ArraySequence<T>>(segmentSize));
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

    int segmentNumber = index / segmentSize;
    int segmentIndex = index % segmentSize;

    segments->get(segmentNumber)->set(segmentIndex, data);
}

template <typename T>
void SegmentedDeque<T>::concat(const SegmentedDeque<T> *other)
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
void SegmentedDeque<T>::print() const
{
    if (totalSize == 0)
    {
        std::cout << "Empty";
    }

    for (const auto &segment : *segments)
    {
        segment->print();
    }
}
