#include <iostream>
#include <vector>
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
    if (segments)
    {
        for (int i = 0; i < segments->getLength(); i++)
        {
            if (segments->get(i))
            {
                delete segments->get(i);
            }
        }
        delete segments;
    }
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

    for (int i = 0; i < other->getLength(); i++)
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

template <typename T>
template <class InputIt, class OutputIt, class UnaryOp>
OutputIt SegmentedDeque<T>::apply(InputIt first1, InputIt last1, OutputIt destFirst, const UnaryOp unaryOp)
{
    while (first1 != last1)
    {
        *destFirst = unaryOp(*first1);
        ++first1;
        ++destFirst;
    }

    return destFirst;
}

template <typename T>
template <class InputIt1, class InputIt2, class OutputIt, class BinaryOp>
OutputIt SegmentedDeque<T>::apply(InputIt1 first1, InputIt1 last1, InputIt2 first2, OutputIt destFirst, const BinaryOp binaryOp)
{
    while (first1 != last1)
    {
        *destFirst = binaryOp(*first1, *first2);
        ++first1;
        ++first2;
        ++destFirst;
    }

    return destFirst;
}

template <typename T>
template <typename Predicate>
SegmentedDeque<T> *SegmentedDeque<T>::where(const Predicate &pred) const
{
    auto *result = new SegmentedDeque<T>(this->segmentSize);
    for (int i = 0; i < this->getLength(); i++)
    {
        const T &item = this->get(i);
        if (pred(item))
        {
            result->append(item);
        }
    }
    return result;
}

template <typename T>
template <typename R, typename BinaryOp>
R SegmentedDeque<T>::reduce(const BinaryOp &op, R init) const
{
    for (int i = 0; i < this->getLength(); i++)
    {
        init = op(init, this->get(i));
    }
    return init;
}

template <typename T>
template <class RandomIt, class Compare>
void SegmentedDeque<T>::sort(RandomIt first, RandomIt last, Compare compare)
{
    int n = this->getLength();
    if (n <= 1)
    {
        return;
    }

    mergeSort(0, n - 1, compare);
}

template <typename T>
template <class RandomIt>
void SegmentedDeque<T>::sort(RandomIt first, RandomIt last)
{
    sort(first, last, std::less<T>());
}

template <typename T>
template <class Compare>
void SegmentedDeque<T>::mergeSort(int left, int right, Compare compare)
{
    if (left >= right)
    {
        return;
    }

    int mid = left + (right - left) / 2;
    mergeSort(left, mid, compare);
    mergeSort(mid + 1, right, compare);

    std::vector<T> buffer(right - left + 1);
    for (int i = left; i <= right; ++i)
    {
        buffer[i - left] = this->get(i);
    }

    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right)
    {
        if (compare(buffer[i], buffer[j]))
        {
            this->set(k++, buffer[i++]);
        }
        else
        {
            this->set(k++, buffer[j++]);
        }
    }

    while (i <= mid)
    {
        this->set(k++, buffer[i++]);
    }
    while (j <= right)
    {
        this->set(k++, buffer[j++]);
    }
}

template <typename T>
template <class ForwardIt1, class ForwardIt2>
bool SegmentedDeque<T>::searchSubsequence(ForwardIt1 first, ForwardIt1 last, ForwardIt2 searchFirst, ForwardIt2 searchLast) const
{
    if (searchFirst == searchLast)
    {
        return true;
    }

    while (first != last)
    {
        ForwardIt1 it1 = first;
        ForwardIt2 it2 = searchFirst;

        while (*it1 == *it2)
        {
            ++it1;
            ++it2;

            if (it2 == searchLast)
            {
                return true;
            }

            if (it1 == last)
            {
                return false;
            }
        }

        ++first;
    }

    return false;
}

//* { Iterator
template <typename T>
SegmentedDeque<T>::Iterator::Iterator(SegmentedDeque<T> *deque, const int index)
    : deque(deque), index(index) {}

template <typename T>
T &SegmentedDeque<T>::Iterator::operator*()
{
    return deque->get(index);
}

template <typename T>
typename SegmentedDeque<T>::Iterator &SegmentedDeque<T>::Iterator::operator++()
{
    ++index;
    return *this;
}

template <typename T>
typename SegmentedDeque<T>::Iterator SegmentedDeque<T>::Iterator::operator++(int)
{
    Iterator temp = *this;
    ++(*this);
    return temp;
}

template <typename T>
bool SegmentedDeque<T>::Iterator::operator==(const Iterator &other) const
{
    return index == other.index && deque == other.deque;
}

template <typename T>
bool SegmentedDeque<T>::Iterator::operator!=(const Iterator &other) const
{
    return !(*this == other);
}

template <typename T>
typename SegmentedDeque<T>::Iterator SegmentedDeque<T>::begin()
{
    return Iterator(this, 0);
}

template <typename T>
typename SegmentedDeque<T>::Iterator SegmentedDeque<T>::end()
{
    return Iterator(this, totalSize);
}

template <typename T>
bool SegmentedDeque<T>::Iterator::notEnd() const
{
    return index < deque->getLength();
}

//* } Iterator

//* { ConstIterator
template <typename T>
SegmentedDeque<T>::ConstIterator::ConstIterator(const SegmentedDeque<T> *deque, const int index)
    : deque(deque), index(index) {}

template <typename T>
const T &SegmentedDeque<T>::ConstIterator::operator*() const
{
    return deque->get(index);
}

template <typename T>
typename SegmentedDeque<T>::ConstIterator &SegmentedDeque<T>::ConstIterator::operator++()
{
    ++index;
    return *this;
}

template <typename T>
typename SegmentedDeque<T>::ConstIterator SegmentedDeque<T>::ConstIterator::operator++(int)
{
    ConstIterator temp = *this;
    ++(*this);
    return temp;
}

template <typename T>
bool SegmentedDeque<T>::ConstIterator::operator==(const ConstIterator &other) const
{
    return index == other.index && deque == other.deque;
}

template <typename T>
bool SegmentedDeque<T>::ConstIterator::operator!=(const ConstIterator &other) const
{
    return !(*this == other);
}

template <typename T>
typename SegmentedDeque<T>::ConstIterator SegmentedDeque<T>::cbegin() const
{
    return ConstIterator(this, 0);
}

template <typename T>
typename SegmentedDeque<T>::ConstIterator SegmentedDeque<T>::cend() const
{
    return ConstIterator(this, totalSize);
}

template <typename T>
bool SegmentedDeque<T>::ConstIterator::notEnd() const
{
    return index < deque->getLength();
}

//* } ConstIterator
