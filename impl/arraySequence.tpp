#include "../inc/arraySequence.hpp"

template <class T>
ArraySequence<T>::ArraySequence() : array() {}

template <class T>
ArraySequence<T>::ArraySequence(const T *items, const int count) : array(items, count)
{
    if (items == nullptr && count > 0)
    {
        throw std::invalid_argument("Null array with non-zero count");
    }
    if (count < 0)
    {
        throw std::invalid_argument("Negative count");
    }
}

template <class T>
ArraySequence<T>::ArraySequence(const int count) : array(count) {}

template <class T>
ArraySequence<T>::ArraySequence(const DynamicArray<T> &array) : array(array) {}

template <class T>
ArraySequence<T>::~ArraySequence() {}

template <class T>
T &ArraySequence<T>::getFirst()
{
    return array.getFirst();
}

template <class T>
const T &ArraySequence<T>::getFirst() const
{
    return array.getFirst();
}

template <class T>
T &ArraySequence<T>::getLast()
{
    return array.getLast();
}

template <class T>
const T &ArraySequence<T>::getLast() const
{
    return array.getLast();
}

template <class T>
T &ArraySequence<T>::get(const int index)
{
    return array.get(index);
}

template <class T>
const T &ArraySequence<T>::get(const int index) const
{
    return array.get(index);
}

template <class T>
int ArraySequence<T>::getLength() const
{
    return array.getSize();
}

template <class T>
Sequence<T> *ArraySequence<T>::getSubsequence(const int startIndex, const int endIndex) const
{
    if (startIndex < 0 || endIndex >= this->getLength() || startIndex > endIndex)
    {
        throw std::out_of_range("Invalid subsequence range");
    }

    ArraySequence<T> *newSequence = new ArraySequence<T>();
    for (int i = startIndex; i <= endIndex; i++)
    {
        newSequence->append(this->get(i));
    }
    return newSequence;
}

template <class T>
void ArraySequence<T>::append(const T &item)
{
    array.append(item);
}

template <class T>
void ArraySequence<T>::prepend(const T &item)
{
    array.prepend(item);
}

template <class T>
void ArraySequence<T>::insertAt(const T &item, const int index)
{
    if (index < 0 || index > getLength())
    {
        throw std::out_of_range("Invalid index for insertion");
    }
    array.insertAt(item, index);
}

template <class T>
void ArraySequence<T>::set(const int index, const T &data)
{
    if (index < 0 || index >= getLength())
    {
        throw std::out_of_range("Index out of range");
    }
    array[index] = data;
}

template <class T>
void ArraySequence<T>::concat(const Sequence<T> *other)
{
    if (!other)
    {
        return;
    }

    for (int i = 0; i < other->getLength(); i++)
    {
        array.append(other->get(i));
    }
}

template <class T>
Sequence<T> *ArraySequence<T>::setImmutable(const int index, const T &data) const
{
    ArraySequence<T> *newSequence = new ArraySequence<T>(*this);
    newSequence->set(index, data);
    return newSequence;
}

template <class T>
Sequence<T> *ArraySequence<T>::appendImmutable(const T &item) const
{
    ArraySequence<T> *newSequence = new ArraySequence<T>(*this);
    newSequence->append(item);
    return newSequence;
}

template <class T>
Sequence<T> *ArraySequence<T>::prependImmutable(const T &item) const
{
    ArraySequence<T> *newSequence = new ArraySequence<T>(*this);
    newSequence->prepend(item);
    return newSequence;
}

template <class T>
Sequence<T> *ArraySequence<T>::insertAtImmutable(const T &item, const int index) const
{
    ArraySequence<T> *newSequence = new ArraySequence<T>(*this);
    newSequence->insertAt(item, index);
    return newSequence;
}

template <class T>
Sequence<T> *ArraySequence<T>::concatImmutable(const Sequence<T> *other) const
{
    ArraySequence<T> *newSequence = new ArraySequence<T>(*this);
    newSequence->concat(other);
    return newSequence;
}

template <class T>
void ArraySequence<T>::print() const
{
    array.print();
}

template <class T>
T &ArraySequence<T>::operator[](const int index)
{
    if (index < 0 || index >= getLength())
    {
        throw std::out_of_range("Index out of range");
    }
    return array[index];
}

template <class T>
const T &ArraySequence<T>::operator[](const int index) const
{
    if (index < 0 || index >= getLength())
    {
        throw std::out_of_range("Index out of range");
    }
    return array[index];
}

template <class T>
void ArraySequence<T>::clear()
{
    array.clear();
}

template <class T>
ArraySequence<T> &ArraySequence<T>::operator=(const ArraySequence &other)
{
    if (this != &other)
    {
        array = other.array;
    }
    return *this;
}