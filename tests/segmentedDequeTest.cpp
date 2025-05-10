#include <gtest/gtest.h>
#include "../inc/segmentedDeque.hpp"

class SegmentedDequeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Default deque with segment size 3 for testing
        deque = new SegmentedDeque<int>(3);
    }

    void TearDown() override
    {
        delete deque;
    }

    SegmentedDeque<int> *deque;
};

// Constructor tests
TEST_F(SegmentedDequeTest, ConstructorCreatesEmptyDeque)
{
    EXPECT_EQ(deque->getLength(), 0);
}

TEST_F(SegmentedDequeTest, ConstructorThrowsOnInvalidSegmentSize)
{
    EXPECT_THROW(SegmentedDeque<int>(-1), std::invalid_argument);
    EXPECT_THROW(SegmentedDeque<int>(0), std::invalid_argument);
}

// Basic operations
TEST_F(SegmentedDequeTest, AppendIncreasesSize)
{
    deque->append(1);
    EXPECT_EQ(deque->getLength(), 1);

    deque->append(2);
    EXPECT_EQ(deque->getLength(), 2);
}

TEST_F(SegmentedDequeTest, PrependIncreasesSize)
{
    deque->prepend(1);
    EXPECT_EQ(deque->getLength(), 1);

    deque->prepend(2);
    EXPECT_EQ(deque->getLength(), 2);
}

TEST_F(SegmentedDequeTest, GetFirstAndLastWorkCorrectly)
{
    deque->append(1);
    deque->append(2);
    deque->prepend(0);

    EXPECT_EQ(deque->getFirst(), 0);
    EXPECT_EQ(deque->getLast(), 2);
}

TEST_F(SegmentedDequeTest, GetWorksCorrectly)
{
    deque->append(1);
    deque->append(2);
    deque->append(3);
    deque->append(4); // Should create new segment

    EXPECT_EQ(deque->get(0), 1);
    EXPECT_EQ(deque->get(1), 2);
    EXPECT_EQ(deque->get(2), 3);
    EXPECT_EQ(deque->get(3), 4);
}

// Edge cases
TEST_F(SegmentedDequeTest, GetFirstThrowsWhenEmpty)
{
    EXPECT_THROW(deque->getFirst(), std::out_of_range);
}

TEST_F(SegmentedDequeTest, GetLastThrowsWhenEmpty)
{
    EXPECT_THROW(deque->getLast(), std::out_of_range);
}

TEST_F(SegmentedDequeTest, GetThrowsWhenEmpty)
{
    EXPECT_THROW(deque->get(0), std::out_of_range);
}

TEST_F(SegmentedDequeTest, GetThrowsOnInvalidIndex)
{
    deque->append(1);
    EXPECT_THROW(deque->get(-1), std::out_of_range);
    EXPECT_THROW(deque->get(1), std::out_of_range);
}

// Segment creation tests
TEST_F(SegmentedDequeTest, NewSegmentCreatedWhenFull)
{
    // Segment size is 3
    deque->append(1);
    deque->append(2);
    deque->append(3);
    deque->append(4); // Should create new segment

    EXPECT_EQ(deque->getLength(), 4);
    EXPECT_EQ(deque->get(3), 4);
}

TEST_F(SegmentedDequeTest, NewSegmentCreatedWhenPrependingToFullSegment)
{
    // Segment size is 3
    deque->prepend(3);
    deque->prepend(2);
    deque->prepend(1);
    deque->prepend(0); // Should create new segment

    EXPECT_EQ(deque->getLength(), 4);
    EXPECT_EQ(deque->get(0), 0);
}

// InsertAt tests
TEST_F(SegmentedDequeTest, InsertAtBeginningIsSameAsPrepend)
{
    deque->insertAt(1, 0);
    deque->insertAt(0, 0);

    EXPECT_EQ(deque->getLength(), 2);
    EXPECT_EQ(deque->getFirst(), 0);
}

TEST_F(SegmentedDequeTest, InsertAtEndIsSameAsAppend)
{
    deque->insertAt(0, 0);
    deque->insertAt(1, 1);

    EXPECT_EQ(deque->getLength(), 2);
    EXPECT_EQ(deque->getLast(), 1);
}

TEST_F(SegmentedDequeTest, InsertAtMiddleWorksCorrectly)
{
    deque->append(1);
    deque->append(3);
    deque->insertAt(2, 1);

    EXPECT_EQ(deque->getLength(), 3);
    EXPECT_EQ(deque->get(0), 1);
    EXPECT_EQ(deque->get(1), 2);
    EXPECT_EQ(deque->get(2), 3);
}

TEST_F(SegmentedDequeTest, InsertAtThrowsOnInvalidIndex)
{
    EXPECT_THROW(deque->insertAt(1, -1), std::out_of_range);
    EXPECT_THROW(deque->insertAt(1, 1), std::out_of_range);
}

// Set tests
TEST_F(SegmentedDequeTest, SetWorksCorrectly)
{
    deque->append(1);
    deque->append(2);
    deque->set(0, 10);
    deque->set(1, 20);

    EXPECT_EQ(deque->get(0), 10);
    EXPECT_EQ(deque->get(1), 20);
}

TEST_F(SegmentedDequeTest, SetThrowsOnInvalidIndex)
{
    EXPECT_THROW(deque->set(-1, 1), std::out_of_range);
    EXPECT_THROW(deque->set(0, 1), std::out_of_range);
}

// Copy constructor tests
TEST_F(SegmentedDequeTest, CopyConstructorCreatesIndependentCopy)
{
    deque->append(1);
    deque->append(2);

    SegmentedDeque<int> copy(*deque);
    copy.append(3);
    copy.set(0, 10);

    EXPECT_EQ(deque->getLength(), 2);
    EXPECT_EQ(copy.getLength(), 3);
    EXPECT_EQ(deque->get(0), 1);
    EXPECT_EQ(copy.get(0), 10);
}

// Concatenation tests
TEST_F(SegmentedDequeTest, ConcatWorksCorrectly)
{
    deque->append(1);
    deque->append(2);

    SegmentedDeque<int> other(3);
    other.append(3);
    other.append(4);

    deque->concat(&other);

    EXPECT_EQ(deque->getLength(), 4);
    EXPECT_EQ(deque->get(0), 1);
    EXPECT_EQ(deque->get(1), 2);
    EXPECT_EQ(deque->get(2), 3);
    EXPECT_EQ(deque->get(3), 4);
}

TEST_F(SegmentedDequeTest, ConcatWithNullDoesNothing)
{
    deque->append(1);
    deque->concat(nullptr);

    EXPECT_EQ(deque->getLength(), 1);
}

// Subsequence tests
TEST_F(SegmentedDequeTest, GetSubsequenceWorksCorrectly)
{
    for (int i = 1; i <= 5; i++)
    {
        deque->append(i);
    }

    Sequence<int> *sub = deque->getSubsequence(1, 3);

    EXPECT_EQ(sub->getLength(), 3);
    EXPECT_EQ(sub->get(0), 2);
    EXPECT_EQ(sub->get(1), 3);
    EXPECT_EQ(sub->get(2), 4);

    delete sub;
}

TEST_F(SegmentedDequeTest, GetSubsequenceThrowsOnInvalidRange)
{
    deque->append(1);

    EXPECT_THROW(deque->getSubsequence(-1, 0), std::out_of_range);
    EXPECT_THROW(deque->getSubsequence(0, 1), std::out_of_range);
    EXPECT_THROW(deque->getSubsequence(1, 0), std::out_of_range);
}

// Immutable operations tests
TEST_F(SegmentedDequeTest, AppendImmutableCreatesNewDeque)
{
    deque->append(1);
    Sequence<int> *newDeque = deque->appendImmutable(2);

    EXPECT_EQ(deque->getLength(), 1);
    EXPECT_EQ(newDeque->getLength(), 2);
    EXPECT_EQ(newDeque->get(1), 2);

    delete newDeque;
}

TEST_F(SegmentedDequeTest, PrependImmutableCreatesNewDeque)
{
    deque->append(1);
    Sequence<int> *newDeque = deque->prependImmutable(0);

    EXPECT_EQ(deque->getLength(), 1);
    EXPECT_EQ(newDeque->getLength(), 2);
    EXPECT_EQ(newDeque->get(0), 0);

    delete newDeque;
}

// Print test (mostly for coverage)
TEST_F(SegmentedDequeTest, PrintDoesNotThrow)
{
    EXPECT_NO_THROW(deque->print());

    deque->append(1);
    EXPECT_NO_THROW(deque->print());
}

// Test with different segment sizes
TEST(SegmentedDequeCustomSizeTest, WorksWithDifferentSegmentSizes)
{
    SegmentedDeque<int> smallSegments(1);
    for (int i = 0; i < 10; i++)
    {
        smallSegments.append(i);
    }
    EXPECT_EQ(smallSegments.getLength(), 10);

    SegmentedDeque<int> largeSegments(100);
    for (int i = 0; i < 10; i++)
    {
        largeSegments.append(i);
    }
    EXPECT_EQ(largeSegments.getLength(), 10);
}