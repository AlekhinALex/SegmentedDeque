#include <gtest/gtest.h>
#include "../inc/segmentedDeque.hpp"
#include "../types/complex.hpp"
#include "../types/person.hpp"
#include <sstream>

class SegmentedDequeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        deque = new SegmentedDeque<int>(3);
    }

    void TearDown() override
    {
        delete deque;
    }

    SegmentedDeque<int> *deque;
};

TEST_F(SegmentedDequeTest, ConstructorCreatesEmptyDeque)
{
    EXPECT_EQ(deque->getLength(), 0);
}

TEST_F(SegmentedDequeTest, ConstructorThrowsOnInvalidSegmentSize)
{
    EXPECT_THROW(SegmentedDeque<int>(-1), std::invalid_argument);
    EXPECT_THROW(SegmentedDeque<int>(0), std::invalid_argument);
}

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
    deque->append(4);

    EXPECT_EQ(deque->get(0), 1);
    EXPECT_EQ(deque->get(1), 2);
    EXPECT_EQ(deque->get(2), 3);
    EXPECT_EQ(deque->get(3), 4);
}

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

TEST_F(SegmentedDequeTest, NewSegmentCreatedWhenFull)
{
    deque->append(1);
    deque->append(2);
    deque->append(3);
    deque->append(4);

    EXPECT_EQ(deque->getLength(), 4);
    EXPECT_EQ(deque->get(3), 4);
}

TEST_F(SegmentedDequeTest, NewSegmentCreatedWhenPrependingToFullSegment)
{
    deque->prepend(3);
    deque->prepend(2);
    deque->prepend(1);
    deque->prepend(0);

    EXPECT_EQ(deque->getLength(), 4);
    EXPECT_EQ(deque->get(0), 0);
}

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

TEST_F(SegmentedDequeTest, PrintDoesNotThrow)
{
    EXPECT_NO_THROW(deque->print());

    deque->append(1);
    EXPECT_NO_THROW(deque->print());
}

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

TEST(SegmentedDequeRangeLoopTest, RangeBasedForLoopWorks)
{
    SegmentedDeque<int> deque(3);
    for (int i = 0; i < 5; i++)
    {
        deque.append(i);
    }

    int sum = 0;
    for (const auto &item : deque)
    {
        sum += item;
    }
    EXPECT_EQ(sum, 10);

    for (auto &item : deque)
    {
        item *= 2;
    }

    sum = 0;
    for (const auto &item : deque)
    {
        sum += item;
    }
    EXPECT_EQ(sum, 20);
}

TEST(SegmentedDequeComplexTest, WorksWithComplexNumbers)
{
    SegmentedDeque<Complex> deque(2);
    
    deque.append(Complex(1, 2));
    deque.append(Complex(3, 4));
    deque.append(Complex(5, 6));
    
    EXPECT_EQ(deque.getLength(), 3);
    EXPECT_EQ(deque.get(0), Complex(1, 2));
    EXPECT_EQ(deque.get(1), Complex(3, 4));
    EXPECT_EQ(deque.get(2), Complex(5, 6));
    
    double magnitudeSum = 0;
    for (const auto &c : deque)
    {
        magnitudeSum += c.magnitude();
    }
    
    EXPECT_NEAR(magnitudeSum, 15.05, 0.01);
    
    deque.sort(deque.begin(), deque.end());
    
    EXPECT_EQ(deque.get(0), Complex(1, 2));
    EXPECT_EQ(deque.get(1), Complex(3, 4));
    EXPECT_EQ(deque.get(2), Complex(5, 6));
}

TEST(SegmentedDequePersonTest, WorksWithPersonObjects)
{
    SegmentedDeque<Person> deque(2);
    
    deque.append(Person("Alice", 30));
    deque.append(Person("Bob", 25));
    deque.append(Person("Charlie", 35));
    
    EXPECT_EQ(deque.getLength(), 3);
    EXPECT_EQ(deque.get(0), Person("Alice", 30));
    EXPECT_EQ(deque.get(1), Person("Bob", 25));
    EXPECT_EQ(deque.get(2), Person("Charlie", 35));
    
    int totalAge = 0;
    for (const auto &person : deque)
    {
        totalAge += person.getAge();
    }
    
    EXPECT_EQ(totalAge, 90);
    
    deque.sort(deque.begin(), deque.end());
    
    EXPECT_EQ(deque.get(0), Person("Bob", 25));
    EXPECT_EQ(deque.get(1), Person("Alice", 30));
    EXPECT_EQ(deque.get(2), Person("Charlie", 35));
}

TEST(SegmentedDequeIteratorTest, IteratorFunctionsWork)
{
    SegmentedDeque<int> deque(3);
    for (int i = 0; i < 5; i++)
    {
        deque.append(i);
    }
    
    auto it = deque.begin();
    EXPECT_EQ(*it, 0);
    
    ++it;
    EXPECT_EQ(*it, 1);
    
    it++;
    EXPECT_EQ(*it, 2);
    
    auto it2 = deque.begin();
    ++it2;
    ++it2;
    EXPECT_EQ(it, it2);
    
    ++it2;
    EXPECT_NE(it, it2);
    
    EXPECT_TRUE(it.notEnd());
    
    it = deque.end();
    EXPECT_FALSE(it.notEnd());
}

TEST(SegmentedDequeConstIteratorTest, ConstIteratorFunctionsWork)
{
    SegmentedDeque<int> deque(3);
    for (int i = 0; i < 5; i++)
    {
        deque.append(i);
    }
    
    const SegmentedDeque<int> &constDeque = deque;
    
    auto it = constDeque.cbegin();
    EXPECT_EQ(*it, 0);
    
    ++it;
    EXPECT_EQ(*it, 1);
    
    it++;
    EXPECT_EQ(*it, 2);
    
    auto it2 = constDeque.cbegin();
    ++it2;
    ++it2;
    EXPECT_EQ(it, it2);
    
    ++it2;
    EXPECT_NE(it, it2);
    
    EXPECT_TRUE(it.notEnd());
    
    it = constDeque.cend();
    EXPECT_FALSE(it.notEnd());
}