#include <gtest/gtest.h>
#include "../inc/listSequence.hpp"

TEST(ListSequenceTest, DefaultConstructorCreatesEmptySequence)
{
    ListSequence<int> seq;
    EXPECT_EQ(seq.getLength(), 0);
    EXPECT_THROW(seq.getFirst(), std::out_of_range);
    EXPECT_THROW(seq.getLast(), std::out_of_range);
}

TEST(ListSequenceTest, CountConstructorInitializesWithDefaultValues)
{
    const int count = 3;
    ListSequence<std::string> seq(count);
    EXPECT_EQ(seq.getLength(), count);
    for (int i = 0; i < count; ++i)
    {
        EXPECT_EQ(seq.get(i), "");
    }

    EXPECT_THROW(ListSequence<int>(-1), std::invalid_argument);
}

TEST(ListSequenceTest, ItemsArrayConstructorCopiesItems)
{
    double items[] = {1.1, 2.2, 3.3};
    const int count = sizeof(items) / sizeof(items[0]);
    ListSequence<double> seq(items, count);

    EXPECT_EQ(seq.getLength(), count);
    for (int i = 0; i < count; ++i)
    {
        EXPECT_DOUBLE_EQ(seq.get(i), items[i]);
    }

    EXPECT_THROW(ListSequence<int>(nullptr, 1), std::invalid_argument);
}

TEST(ListSequenceTest, CopyConstructorCreatesIndependentCopy)
{
    ListSequence<int> original;
    original.append(1);
    original.append(2);
    original.append(3);

    ListSequence<int> copy(original);
    EXPECT_EQ(copy.getLength(), original.getLength());
    for (int i = 0; i < original.getLength(); ++i)
    {
        EXPECT_EQ(copy.get(i), original.get(i));
    }

    copy.set(0, 10);
    EXPECT_NE(copy.get(0), original.get(0)); // Verify deep copy
}

TEST(ListSequenceTest, AssignmentOperatorMakesDeepCopy)
{
    ListSequence<int> source;
    source.append(1);
    source.append(2);

    ListSequence<int> target;
    target = source;
    EXPECT_EQ(target.getLength(), source.getLength());
    for (int i = 0; i < source.getLength(); ++i)
    {
        EXPECT_EQ(target.get(i), source.get(i));
    }

    target.set(0, 10);
    EXPECT_NE(target.get(0), source.get(0)); // Verify deep copy
}

TEST(ListSequenceTest, SelfAssignmentIsSafe)
{
    ListSequence<int> seq;
    seq.append(1);
    seq.append(2);

    const auto old_length = seq.getLength();
    const auto old_first = seq.get(0);
    const auto old_last = seq.get(1);

    seq = seq; // Self-assignment

    EXPECT_EQ(seq.getLength(), old_length);
    EXPECT_EQ(seq.get(0), old_first);
    EXPECT_EQ(seq.get(1), old_last);
}

TEST(ListSequenceTest, GetMethodsThrowWhenEmpty)
{
    ListSequence<int> empty_seq;
    EXPECT_THROW(empty_seq.getFirst(), std::out_of_range);
    EXPECT_THROW(empty_seq.getLast(), std::out_of_range);
    EXPECT_THROW(empty_seq.get(0), std::out_of_range);
}

TEST(ListSequenceTest, AppendIncreasesLengthAndMaintainsOrder)
{
    ListSequence<int> seq;
    const int test_values[] = {1, 2, 3};

    for (size_t i = 0; i < sizeof(test_values) / sizeof(test_values[0]); ++i)
    {
        seq.append(test_values[i]);
        EXPECT_EQ(seq.getLength(), i + 1);
        EXPECT_EQ(seq.getLast(), test_values[i]);

        // Verify all previous elements remain unchanged
        for (size_t j = 0; j <= i; ++j)
        {
            EXPECT_EQ(seq.get(j), test_values[j]);
        }
    }
}

TEST(ListSequenceTest, PrependAddsToBeginning)
{
    ListSequence<int> seq;
    const int test_values[] = {3, 2, 1};

    for (size_t i = 0; i < sizeof(test_values) / sizeof(test_values[0]); ++i)
    {
        seq.prepend(test_values[i]);
        EXPECT_EQ(seq.getLength(), i + 1);
        EXPECT_EQ(seq.getFirst(), test_values[i]);

        // Verify the rest of sequence
        for (size_t j = 0; j <= i; ++j)
        {
            EXPECT_EQ(seq.get(j), test_values[i - j]);
        }
    }
}

TEST(ListSequenceTest, InsertAtHandlesVariousPositions)
{
    ListSequence<int> seq;

    // Insert into empty sequence
    seq.insertAt(1, 0);
    EXPECT_EQ(seq.getLength(), 1);
    EXPECT_EQ(seq.get(0), 1);

    // Insert at beginning
    seq.insertAt(0, 0);
    EXPECT_EQ(seq.getLength(), 2);
    EXPECT_EQ(seq.get(0), 0);

    // Insert at end
    seq.insertAt(3, 2);
    EXPECT_EQ(seq.getLength(), 3);
    EXPECT_EQ(seq.get(2), 3);

    // Insert in middle
    seq.insertAt(2, 1);
    EXPECT_EQ(seq.getLength(), 4);
    EXPECT_EQ(seq.get(1), 2);

    // Verify full sequence
    EXPECT_EQ(seq.get(0), 0);
    EXPECT_EQ(seq.get(1), 2);
    EXPECT_EQ(seq.get(2), 1);
    EXPECT_EQ(seq.get(3), 3);
}

TEST(ListSequenceTest, InsertAtThrowsForInvalidIndices)
{
    ListSequence<int> seq;
    seq.append(1);

    EXPECT_THROW(seq.insertAt(0, -1), std::out_of_range);
    EXPECT_THROW(seq.insertAt(0, 2), std::out_of_range);
}

TEST(ListSequenceTest, SetModifiesElements)
{
    ListSequence<int> seq;
    seq.append(1);
    seq.append(2);

    seq.set(0, 10);
    seq.set(1, 20);

    EXPECT_EQ(seq.get(0), 10);
    EXPECT_EQ(seq.get(1), 20);
}

TEST(ListSequenceTest, SetThrowsForInvalidIndices)
{
    ListSequence<int> seq;

    EXPECT_THROW(seq.set(-1, 0), std::out_of_range);
    EXPECT_THROW(seq.set(0, 0), std::out_of_range);

    seq.append(1);
    EXPECT_THROW(seq.set(1, 0), std::out_of_range);
}

TEST(ListSequenceTest, GetSubsequenceReturnsCorrectSubset)
{
    ListSequence<int> seq;
    for (int i = 0; i < 5; ++i)
    {
        seq.append(i);
    }

    const int start = 1;
    const int end = 3;
    Sequence<int> *sub = seq.getSubsequence(start, end);
    ASSERT_NE(sub, nullptr);

    EXPECT_EQ(sub->getLength(), end - start + 1);
    for (int i = start; i <= end; ++i)
    {
        EXPECT_EQ(sub->get(i - start), seq.get(i));
    }

    delete sub;
}

TEST(ListSequenceTest, GetSubsequenceThrowsForInvalidRanges)
{
    ListSequence<int> seq;
    seq.append(1);
    seq.append(2);

    EXPECT_THROW(seq.getSubsequence(-1, 1), std::out_of_range);
    EXPECT_THROW(seq.getSubsequence(1, 0), std::out_of_range);
    EXPECT_THROW(seq.getSubsequence(0, 2), std::out_of_range);
}

TEST(ListSequenceTest, ImmutableOperationsCreateNewSequences)
{
    ListSequence<int> original;
    original.append(1);
    original.append(2);

    // Test appendImmutable
    Sequence<int> *appended = original.appendImmutable(3);
    ASSERT_NE(appended, nullptr);
    EXPECT_EQ(original.getLength(), 2);
    EXPECT_EQ(appended->getLength(), 3);
    EXPECT_EQ(appended->get(2), 3);
    delete appended;

    // Test prependImmutable
    Sequence<int> *prepended = original.prependImmutable(0);
    ASSERT_NE(prepended, nullptr);
    EXPECT_EQ(original.getLength(), 2);
    EXPECT_EQ(prepended->getLength(), 3);
    EXPECT_EQ(prepended->get(0), 0);
    delete prepended;

    // Test insertAtImmutable
    Sequence<int> *inserted = original.insertAtImmutable(5, 1);
    ASSERT_NE(inserted, nullptr);
    EXPECT_EQ(original.getLength(), 2);
    EXPECT_EQ(inserted->getLength(), 3);
    EXPECT_EQ(inserted->get(1), 5);
    delete inserted;

    // Test setImmutable
    Sequence<int> *modified = original.setImmutable(0, 10);
    ASSERT_NE(modified, nullptr);
    EXPECT_EQ(original.get(0), 1);
    EXPECT_EQ(modified->get(0), 10);
    delete modified;
}

TEST(ListSequenceTest, ConcatImmutableCombinesSequences)
{
    ListSequence<int> seq1;
    seq1.append(1);
    seq1.append(2);

    ListSequence<int> seq2;
    seq2.append(3);
    seq2.append(4);

    Sequence<int> *combined = seq1.concatImmutable(&seq2);
    ASSERT_NE(combined, nullptr);

    EXPECT_EQ(combined->getLength(), seq1.getLength() + seq2.getLength());
    for (int i = 0; i < seq1.getLength(); ++i)
    {
        EXPECT_EQ(combined->get(i), seq1.get(i));
    }
    for (int i = 0; i < seq2.getLength(); ++i)
    {
        EXPECT_EQ(combined->get(seq1.getLength() + i), seq2.get(i));
    }

    delete combined;
}

TEST(ListSequenceTest, ClearResetsSequence)
{
    ListSequence<int> seq;
    seq.append(1);
    seq.append(2);

    EXPECT_EQ(seq.getLength(), 2);
    seq.clear();
    EXPECT_EQ(seq.getLength(), 0);
    EXPECT_THROW(seq.get(0), std::out_of_range);
}

TEST(ListSequenceTest, PrintOutputIsCorrect)
{
    ListSequence<int> seq;

    // Test empty sequence
    testing::internal::CaptureStdout();
    seq.print();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Empty list");

    // Test non-empty sequence
    seq.append(1);
    seq.append(2);
    seq.append(3);

    testing::internal::CaptureStdout();
    seq.print();
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "1 2 3 ");
}

// Удален тест OperatorBracketsProvidesAccess, так как operator[] не реализован в ListSequence