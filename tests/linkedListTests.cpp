#include <gtest/gtest.h>
#include "../inc/linkedList.hpp"
#include <iostream>

TEST(LinkedListTest, DefaultConstructorCreatesEmptyList)
{
    LinkedList<int> list;
    EXPECT_EQ(list.getLength(), 0);
    EXPECT_THROW(list.getFirst(), std::out_of_range);
    EXPECT_THROW(list.getLast(), std::out_of_range);
}

TEST(LinkedListTest, CountConstructorInitializesWithDefaultValues)
{
    LinkedList<int> list(5);
    EXPECT_EQ(list.getLength(), 5);
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(list.get(i), 0);
    }

    EXPECT_THROW(LinkedList<int>(-1), std::invalid_argument);
}

TEST(LinkedListTest, ArrayConstructorCopiesItems)
{
    int items[] = {1, 2, 3, 4, 5};
    LinkedList<int> list(items, 5);
    EXPECT_EQ(list.getLength(), 5);
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(list.get(i), items[i]);
    }
}

TEST(LinkedListTest, ArrayConstructorThrowsForInvalidArguments)
{
    EXPECT_THROW(LinkedList<int>(nullptr, 1), std::invalid_argument);
    EXPECT_THROW(LinkedList<int>(nullptr, 0), std::invalid_argument);
}

TEST(LinkedListTest, AssignmentOperatorMakesDeepCopy)
{
    LinkedList<int> original;
    original.append(1);
    original.append(2);
    original.append(3);

    LinkedList<int> copy;
    copy = original;
    EXPECT_EQ(copy.getLength(), 3);
    EXPECT_EQ(copy.get(0), 1);
    EXPECT_EQ(copy.get(1), 2);
    EXPECT_EQ(copy.get(2), 3);

    copy.set(1, 5);
    EXPECT_EQ(original.get(1), 2); // Original shouldn't change
}

TEST(LinkedListTest, SelfAssignmentIsSafe)
{
    LinkedList<int> list;
    list.append(1);
    list.append(2);
    list = list;
    EXPECT_EQ(list.getLength(), 2);
    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 2);
}

TEST(LinkedListTest, AppendIncreasesLength)
{
    LinkedList<int> list;
    list.append(1);
    EXPECT_EQ(list.getLength(), 1);
    EXPECT_EQ(list.getFirst(), 1);
    EXPECT_EQ(list.getLast(), 1);

    list.append(2);
    EXPECT_EQ(list.getLength(), 2);
    EXPECT_EQ(list.getFirst(), 1);
    EXPECT_EQ(list.getLast(), 2);
}

TEST(LinkedListTest, PrependAddsToBeginning)
{
    LinkedList<int> list;
    list.prepend(2);
    EXPECT_EQ(list.getLength(), 1);
    EXPECT_EQ(list.getFirst(), 2);

    list.prepend(1);
    EXPECT_EQ(list.getLength(), 2);
    EXPECT_EQ(list.getFirst(), 1);
    EXPECT_EQ(list.getLast(), 2);
}

TEST(LinkedListTest, GetMethodsThrowWhenEmpty)
{
    LinkedList<int> list;
    EXPECT_THROW(list.getFirst(), std::out_of_range);
    EXPECT_THROW(list.getLast(), std::out_of_range);
    EXPECT_THROW(list.get(0), std::out_of_range);
}

TEST(LinkedListTest, GetSetMethodsWorkCorrectly)
{
    LinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(3);

    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(1), 2);
    EXPECT_EQ(list.get(2), 3);

    list.set(1, 5);
    EXPECT_EQ(list.get(1), 5);

    EXPECT_THROW(list.get(-1), std::out_of_range);
    EXPECT_THROW(list.get(3), std::out_of_range);
    EXPECT_THROW(list.set(-1, 0), std::out_of_range);
    EXPECT_THROW(list.set(3, 0), std::out_of_range);
}

TEST(LinkedListTest, InsertAtHandlesVariousPositions)
{
    LinkedList<int> list;

    // Insert into empty list
    list.insertAt(1, 0);
    EXPECT_EQ(list.getLength(), 1);
    EXPECT_EQ(list.get(0), 1);

    // Insert at beginning
    list.insertAt(0, 0);
    EXPECT_EQ(list.getLength(), 2);
    EXPECT_EQ(list.get(0), 0);
    EXPECT_EQ(list.get(1), 1);

    // Insert at end
    list.insertAt(3, 2);
    EXPECT_EQ(list.getLength(), 3);
    EXPECT_EQ(list.get(2), 3);

    // Insert in middle
    list.insertAt(2, 1);
    EXPECT_EQ(list.getLength(), 4);
    EXPECT_EQ(list.get(1), 2);

    EXPECT_THROW(list.insertAt(0, -1), std::out_of_range);
    EXPECT_THROW(list.insertAt(0, 5), std::out_of_range);
}

TEST(LinkedListTest, ClearResetsList)
{
    LinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(3);

    EXPECT_EQ(list.getLength(), 3);
    list.clear();
    EXPECT_EQ(list.getLength(), 0);
    EXPECT_THROW(list.getFirst(), std::out_of_range);

    // Clear empty list
    list.clear();
    EXPECT_EQ(list.getLength(), 0);
}

TEST(LinkedListTest, GetSubListReturnsCorrectSubset)
{
    LinkedList<int> list;
    for (int i = 0; i < 5; ++i)
    {
        list.append(i);
    }

    LinkedList<int> *subList = list.getSubList(1, 3);
    ASSERT_NE(subList, nullptr);
    EXPECT_EQ(subList->getLength(), 3);
    EXPECT_EQ(subList->get(0), 1);
    EXPECT_EQ(subList->get(1), 2);
    EXPECT_EQ(subList->get(2), 3);
    delete subList;

    // Single element sublist
    subList = list.getSubList(0, 0);
    EXPECT_EQ(subList->getLength(), 1);
    EXPECT_EQ(subList->get(0), 0);
    delete subList;

    // Invalid ranges
    EXPECT_THROW(list.getSubList(-1, 2), std::out_of_range);
    EXPECT_THROW(list.getSubList(2, 1), std::out_of_range);
    EXPECT_THROW(list.getSubList(3, 5), std::out_of_range);
}

TEST(LinkedListTest, ConcatModifiesOriginalList)
{
    LinkedList<int> list1;
    list1.append(1);
    list1.append(2);

    LinkedList<int> list2;
    list2.append(3);
    list2.append(4);

    list1.concat(list2);
    EXPECT_EQ(list1.getLength(), 4);
    EXPECT_EQ(list1.get(0), 1);
    EXPECT_EQ(list1.get(1), 2);
    EXPECT_EQ(list1.get(2), 3);
    EXPECT_EQ(list1.get(3), 4);

    // Concat with empty list
    LinkedList<int> empty;
    list1.concat(empty);
    EXPECT_EQ(list1.getLength(), 4);

    // Self-concat should throw
    EXPECT_THROW(list1.concat(list1), std::invalid_argument);
}

TEST(LinkedListTest, ConcatImmutableCreatesNewList)
{
    LinkedList<int> list1;
    list1.append(1);
    list1.append(2);

    LinkedList<int> list2;
    list2.append(3);
    list2.append(4);

    LinkedList<int> *result = list1.concatImmutable(list2);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getLength(), 4);
    EXPECT_EQ(result->get(0), 1);
    EXPECT_EQ(result->get(1), 2);
    EXPECT_EQ(result->get(2), 3);
    EXPECT_EQ(result->get(3), 4);

    // Original lists unchanged
    EXPECT_EQ(list1.getLength(), 2);
    EXPECT_EQ(list2.getLength(), 2);

    delete result;

    // Self-concat should throw
    EXPECT_THROW(list1.concatImmutable(list1), std::invalid_argument);
}

TEST(LinkedListTest, IteratorOperations)
{
    LinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(3);

    // Prefix increment
    auto it = list.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(it, list.end());

    // Postfix increment
    it = list.begin();
    auto old_it = it++;
    EXPECT_EQ(*old_it, 1);
    EXPECT_EQ(*it, 2);

    // NotEnd check
    it = list.begin();
    EXPECT_TRUE(it.notEnd());
    ++it;
    ++it;
    ++it;
    EXPECT_FALSE(it.notEnd());
    
    // Test that insert and erase throw exceptions
    it = list.begin();
    EXPECT_THROW(it.insert(5), std::runtime_error);
    EXPECT_THROW(it.erase(), std::runtime_error);
}

TEST(LinkedListTest, ConstIteratorOperations)
{
    LinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(3);

    const LinkedList<int> &constList = list;

    auto it = constList.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
    ++it;
    EXPECT_EQ(*it, 3);
    ++it;
    EXPECT_EQ(it, constList.end());

    // Postfix increment
    it = constList.begin();
    auto old_it = it++;
    EXPECT_EQ(*old_it, 1);
    EXPECT_EQ(*it, 2);

    // NotEnd check
    it = constList.begin();
    EXPECT_TRUE(it.notEnd());
    ++it;
    ++it;
    ++it;
    EXPECT_FALSE(it.notEnd());
}

TEST(LinkedListTest, IteratorEqualityComparison)
{
    LinkedList<int> list;
    list.append(1);
    list.append(2);

    auto it1 = list.begin();
    auto it2 = list.begin();
    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 != it2);

    ++it1;
    EXPECT_FALSE(it1 == it2);
    EXPECT_TRUE(it1 != it2);

    auto end1 = list.end();
    auto end2 = list.end();
    EXPECT_TRUE(end1 == end2);
}