#pragma once

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
    /// @brief Iterator section
    class Iterator
    {
    public:
        Node *current;

        Iterator(Node *nodePtr);
        Iterator &operator++();
        Iterator operator++(int);
        T &operator*() const;
        bool operator!=(const Iterator &other) const;
        bool operator==(const Iterator &other) const;
        void insert(const T &item);
        void erase();
        bool notEnd() const;
    };

    /// @brief ConstIterator section
    class ConstIterator
    {
    public:
        const Node *current;

        ConstIterator(const Node *nodePtr);
        ConstIterator &operator++();
        ConstIterator operator++(int);
        const T &operator*() const;
        bool operator!=(const ConstIterator &other) const;
        bool operator==(const ConstIterator &other) const;
        bool notEnd() const;
    };

    Iterator begin() { return Iterator(head); };
    Iterator end() { return Iterator(nullptr); };

    ConstIterator begin() const { return ConstIterator(head); };
    ConstIterator end() const { return ConstIterator(nullptr); };

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
};

#include "../impl/linkedList.tpp"