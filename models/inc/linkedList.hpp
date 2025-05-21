#pragma once

#include <iterator>

template <typename T>
class LinkedList
{
private:
    struct Node
    {
        T value;
        Node *next;
        Node *prev;
        Node(const T &value) : value(value), next(nullptr), prev(nullptr) {}
    };
    Node *head;
    int length;

public:
    LinkedList();
    LinkedList(const int count);
    LinkedList(const T *items, const int count);
    LinkedList(const LinkedList<T> &list);
    ~LinkedList();

    const T &getFirst() const;
    const T &getLast() const;
    const T &get(const int index) const;

    T &getFirst();
    T &getLast();
    T &get(const int index);

    int getLength() const;

    void append(const T &item);
    void prepend(const T &item);
    void set(int index, const T &value);
    void insertAt(const T &value, const int index);

    void print() const;
    void clear();

    void concat(const LinkedList<T> &list);
    LinkedList<T> *concatImmutable(const LinkedList<T> &list) const;
    LinkedList<T> *getSubList(const int startIndex, const int endIndex) const;

    LinkedList<T> &operator=(const LinkedList<T> &other);

public:
    class Iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        Node *current;

        Iterator(Node *nodePtr);
        Iterator &operator++();
        Iterator operator++(int);
        Iterator &operator--();
        Iterator operator--(int);
        Iterator(const Iterator &) = default;
        Iterator &operator=(const Iterator &) = default;
        T &operator*() const;
        bool operator!=(const Iterator &other) const;
        bool operator==(const Iterator &other) const;
        void insert(const T &item);
        void erase();
        bool notEnd() const;
    };
    class ConstIterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T *;
        using reference = const T &;

        const Node *current;

        ConstIterator(const Node *nodePtr);
        ConstIterator &operator++();
        ConstIterator operator++(int);
        ConstIterator &operator--();
        ConstIterator operator--(int);
        ConstIterator(const Iterator &it);
        ConstIterator(const ConstIterator &it) = default;
        ConstIterator &operator=(const ConstIterator &) = default;
        const T &operator*() const;
        bool operator!=(const ConstIterator &other) const;
        bool operator==(const ConstIterator &other) const;
        bool notEnd() const;
    };

    Iterator begin() { return Iterator(head); };
    Iterator end() { return Iterator(nullptr); };

    ConstIterator cbegin() const { return ConstIterator(head); };
    ConstIterator cend() const { return ConstIterator(nullptr); };
};

#include "../impl/linkedList.tpp"