#include "../inc/listSequence.hpp"

template <class T>
ListSequence<T>::ListSequence() : list() {}

template <class T>
ListSequence<T>::ListSequence(const T *items, const int count) : list(items, count) {}

template <class T>
ListSequence<T>::ListSequence(const int count) : list(count) {}

template <class T>
ListSequence<T>::ListSequence(const LinkedList<T> &list) : list(list) {}

template <class T>
ListSequence<T>::~ListSequence() {}

template <class T>
T &ListSequence<T>::getFirst()
{
    return list.getFirst();
}

template <class T>
const T &ListSequence<T>::getFirst() const
{
    return list.getFirst();
}

template <class T>
T &ListSequence<T>::getLast()
{
    return list.getLast();
}

template <class T>
const T &ListSequence<T>::getLast() const
{
    return list.getLast();
}

template <class T>
T &ListSequence<T>::get(int index)
{
    return list.get(index);
}

template <class T>
const T &ListSequence<T>::get(const int index) const
{
    return list.get(index);
}

template <class T>
int ListSequence<T>::getLength() const
{
    return list.getLength();
}

template <class T>
Sequence<T> *ListSequence<T>::getSubsequence(const int startIndex, const int endIndex) const
{
    LinkedList<T> *subLinkedList = list.getSubList(startIndex, endIndex);
    ListSequence<T> *subList = new ListSequence<T>(*subLinkedList);
    delete subLinkedList;
    return subList;
}

template <class T>
void ListSequence<T>::append(const T &item)
{
    list.append(item);
}

template <class T>
void ListSequence<T>::prepend(const T &item)
{
    list.prepend(item);
}

template <class T>
void ListSequence<T>::insertAt(const T &item, const int index)
{
    if (index < 0 || index > getLength())
    {
        throw std::out_of_range("Invalid index for insertion");
    }

    list.insertAt(item, index);
}

template <class T>
void ListSequence<T>::concat(const Sequence<T> *other)
{
    for (int i = 0; i < other->getLength(); i++)
    {
        list.append(other->get(i));
    }
}

template <class T>
void ListSequence<T>::set(const int index, const T &data)
{
    if (index < 0 || index >= getLength())
    {
        throw std::out_of_range("Index out of range");
    }
    this->get(index) = data;
}

template <class T>
Sequence<T> *ListSequence<T>::setImmutable(const int index, const T &data) const
{
    ListSequence<T> *newSequence = new ListSequence<T>(*this);
    newSequence->set(index, data);
    return newSequence;
}

template <class T>
Sequence<T> *ListSequence<T>::appendImmutable(const T &item) const
{
    ListSequence<T> *newSequence = new ListSequence<T>(*this);
    newSequence->append(item);
    return newSequence;
}

template <class T>
Sequence<T> *ListSequence<T>::prependImmutable(const T &item) const
{
    ListSequence<T> *newSequence = new ListSequence<T>(*this);
    newSequence->prepend(item);
    return newSequence;
}

template <class T>
Sequence<T> *ListSequence<T>::insertAtImmutable(const T &item, const int index) const
{
    ListSequence<T> *newSequence = new ListSequence<T>(*this);
    newSequence->insertAt(item, index);
    return newSequence;
}

template <class T>
Sequence<T> *ListSequence<T>::concatImmutable(const Sequence<T> *list) const
{
    ListSequence<T> *newSequence = new ListSequence<T>(*this);
    for (int i = 0; i < list->getLength(); i++)
    {
        newSequence->append(list->get(i));
    }
    return newSequence;
}

template <class T>
void ListSequence<T>::print() const
{
    list.print();
}

template <class T>
void ListSequence<T>::clear()
{
    list.clear();
}

template <class T>
ListSequence<T> &ListSequence<T>::operator=(const ListSequence<T> &other)
{
    if (this != &other)
    {
        list = other.list;
    }
    return *this;
}
