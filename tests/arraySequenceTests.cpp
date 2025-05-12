#include <gtest/gtest.h>
#include "../inc/arraySequence.hpp"

TEST(ArraySequenceTest, DefaultConstructorCreatesEmptySequence)
{
    ArraySequence<int> seq;
    EXPECT_EQ(seq.getLength(), 0);
    EXPECT_THROW(seq.getFirst(), std::out_of_range);
    EXPECT_THROW(seq.getLast(), std::out_of_range);
}

TEST(ArraySequenceTest, CountConstructorInitializesWithDefaultValues)
{
    ArraySequence<std::string> seq(3);
    EXPECT_EQ(seq.getLength(), 3);
    EXPECT_EQ(seq[0], "");
    EXPECT_EQ(seq[1], "");
    EXPECT_EQ(seq[2], "");
}

TEST(ArraySequenceTest, ItemsArrayConstructorCopiesItems)
{
    double items[] = {1.1, 2.2, 3.3};
    ArraySequence<double> seq(items, 3);
    EXPECT_EQ(seq.getLength(), 3);
    EXPECT_DOUBLE_EQ(seq[0], 1.1);
    EXPECT_DOUBLE_EQ(seq[1], 2.2);
    EXPECT_DOUBLE_EQ(seq[2], 3.3);
}

TEST(ArraySequenceTest, ItemsArrayConstructorThrowsForNullWithPositiveCount)
{
    EXPECT_THROW(ArraySequence<int>(nullptr, 1), std::invalid_argument);
}

TEST(ArraySequenceTest, CopyConstructorCreatesIndependentCopy)
{
    ArraySequence<int> seq1;
    seq1.append(1);
    seq1.append(2);

    ArraySequence<int> seq2(seq1);
    EXPECT_EQ(seq2.getLength(), 2);
    EXPECT_EQ(seq2[0], 1);
    EXPECT_EQ(seq2[1], 2);

    seq2[0] = 10;
    EXPECT_EQ(seq1[0], 1);
}

TEST(ArraySequenceTest, AssignmentOperatorMakesDeepCopy)
{
    ArraySequence<int> seq1;
    seq1.append(1);
    seq1.append(2);

    ArraySequence<int> seq2;
    seq2 = seq1;
    EXPECT_EQ(seq2.getLength(), 2);

    seq2[0] = 10;
    EXPECT_EQ(seq1[0], 1);
}

TEST(ArraySequenceTest, SelfAssignmentIsSafe)
{
    ArraySequence<int> seq;
    seq.append(1);
    seq.append(2);
    seq = seq;
    EXPECT_EQ(seq.getLength(), 2);
    EXPECT_EQ(seq[0], 1);
    EXPECT_EQ(seq[1], 2);
}

TEST(ArraySequenceTest, GetMethodsThrowWhenEmpty)
{
    ArraySequence<int> seq;
    EXPECT_THROW(seq.getFirst(), std::out_of_range);
    EXPECT_THROW(seq.getLast(), std::out_of_range);
    EXPECT_THROW(seq.get(0), std::out_of_range);
}

TEST(ArraySequenceTest, AppendIncreasesLength)
{
    ArraySequence<int> seq;
    seq.append(1);
    EXPECT_EQ(seq.getLength(), 1);
    EXPECT_EQ(seq.getFirst(), 1);
    EXPECT_EQ(seq.getLast(), 1);

    seq.append(2);
    EXPECT_EQ(seq.getLength(), 2);
    EXPECT_EQ(seq.getFirst(), 1);
    EXPECT_EQ(seq.getLast(), 2);
}

TEST(ArraySequenceTest, PrependAddsToBeginning)
{
    ArraySequence<int> seq;
    seq.prepend(2);
    seq.prepend(1);
    EXPECT_EQ(seq.getLength(), 2);
    EXPECT_EQ(seq[0], 1);
    EXPECT_EQ(seq[1], 2);
}

TEST(ArraySequenceTest, InsertAtHandlesVariousPositions)
{
    ArraySequence<int> seq;
    seq.append(1);
    seq.append(3);

    seq.insertAt(2, 1);
    EXPECT_EQ(seq.getLength(), 3);
    EXPECT_EQ(seq[1], 2);

    seq.insertAt(0, 0);
    EXPECT_EQ(seq[0], 0);

    seq.insertAt(4, 4);
    EXPECT_EQ(seq[3], 3);
    EXPECT_EQ(seq[4], 4);
}

TEST(ArraySequenceTest, InsertAtThrowsForInvalidIndices)
{
    ArraySequence<int> seq;
    EXPECT_THROW(seq.insertAt(1, -1), std::out_of_range);
    EXPECT_THROW(seq.insertAt(1, 1), std::out_of_range);
}

TEST(ArraySequenceTest, SetModifiesExistingElements)
{
    ArraySequence<int> seq;
    seq.append(1);
    seq.append(2);
    seq.set(0, 10);
    seq.set(1, 20);
    EXPECT_EQ(seq[0], 10);
    EXPECT_EQ(seq[1], 20);
}

TEST(ArraySequenceTest, SetThrowsForInvalidIndices)
{
    ArraySequence<int> seq;
    EXPECT_THROW(seq.set(-1, 1), std::out_of_range);
    EXPECT_THROW(seq.set(0, 1), std::out_of_range);
}

TEST(ArraySequenceTest, GetSubsequenceReturnsCorrectSubset)
{
    ArraySequence<int> seq;
    for (int i = 0; i < 5; i++)
    {
        seq.append(i);
    }

    Sequence<int> *sub = seq.getSubsequence(1, 3);
    ASSERT_NE(sub, nullptr);
    EXPECT_EQ(sub->getLength(), 3);
    EXPECT_EQ(sub->get(0), 1);
    EXPECT_EQ(sub->get(1), 2);
    EXPECT_EQ(sub->get(2), 3);
    delete sub;
}

TEST(ArraySequenceTest, GetSubsequenceThrowsForInvalidRanges)
{
    ArraySequence<int> seq;
    seq.append(1);
    seq.append(2);

    EXPECT_THROW(seq.getSubsequence(-1, 1), std::out_of_range);
    EXPECT_THROW(seq.getSubsequence(1, 0), std::out_of_range);
    EXPECT_THROW(seq.getSubsequence(0, 2), std::out_of_range);
}

TEST(ArraySequenceTest, AppendImmutableCreatesNewSequence)
{
    ArraySequence<int> seq;
    seq.append(1);
    seq.append(2);

    Sequence<int> *newSeq = seq.appendImmutable(3);
    ASSERT_NE(newSeq, nullptr);
    EXPECT_EQ(seq.getLength(), 2);
    EXPECT_EQ(newSeq->getLength(), 3);
    EXPECT_EQ(newSeq->get(2), 3);
    delete newSeq;
}

TEST(ArraySequenceTest, PrependImmutableCreatesNewSequence)
{
    ArraySequence<int> seq;
    seq.append(1);
    seq.append(2);

    Sequence<int> *newSeq = seq.prependImmutable(0);
    ASSERT_NE(newSeq, nullptr);
    EXPECT_EQ(seq.getLength(), 2);
    EXPECT_EQ(newSeq->getLength(), 3);
    EXPECT_EQ(newSeq->get(0), 0);
    delete newSeq;
}

TEST(ArraySequenceTest, InsertAtImmutableCreatesNewSequence)
{
    ArraySequence<int> seq;
    seq.append(1);
    seq.append(3);

    Sequence<int> *newSeq = seq.insertAtImmutable(2, 1);
    ASSERT_NE(newSeq, nullptr);
    EXPECT_EQ(seq.getLength(), 2);
    EXPECT_EQ(newSeq->getLength(), 3);
    EXPECT_EQ(newSeq->get(1), 2);
    delete newSeq;
}

TEST(ArraySequenceTest, SetImmutableCreatesNewSequence)
{
    ArraySequence<int> seq;
    seq.append(1);
    seq.append(2);

    Sequence<int> *newSeq = seq.setImmutable(0, 10);
    ASSERT_NE(newSeq, nullptr);
    EXPECT_EQ(seq.get(0), 1);
    EXPECT_EQ(newSeq->get(0), 10);
    delete newSeq;
}

TEST(ArraySequenceTest, ConcatImmutableCombinesSequences)
{
    ArraySequence<int> seq1;
    seq1.append(1);
    seq1.append(2);

    ArraySequence<int> seq2;
    seq2.append(3);
    seq2.append(4);

    Sequence<int> *combined = seq1.concatImmutable(&seq2);
    ASSERT_NE(combined, nullptr);
    EXPECT_EQ(combined->getLength(), 4);
    EXPECT_EQ(combined->get(0), 1);
    EXPECT_EQ(combined->get(3), 4);
    delete combined;
}

TEST(ArraySequenceTest, ClearResetsSequence)
{
    ArraySequence<int> seq;
    seq.append(1);
    seq.append(2);
    seq.clear();
    EXPECT_EQ(seq.getLength(), 0);
    EXPECT_THROW(seq[0], std::out_of_range);
}

TEST(ArraySequenceTest, OperatorBracketsAllowsAccessAndModification)
{
    ArraySequence<int> seq;
    seq.append(1);
    seq.append(2);

    EXPECT_EQ(seq[0], 1);
    EXPECT_EQ(seq[1], 2);

    seq[0] = 10;
    EXPECT_EQ(seq[0], 10);

    EXPECT_THROW(seq[2], std::out_of_range);
}

TEST(ArraySequenceTest, PrintDoesNotCrash)
{
    ArraySequence<int> seq;
    seq.append(1);
    seq.append(2);

    seq.print();
}

TEST(ArraySequenceTest, ConcatMutableCombinesSequences)
{
    ArraySequence<int> seq1;
    seq1.append(1);
    seq1.append(2);

    ArraySequence<int> seq2;
    seq2.append(3);
    seq2.append(4);

    seq1.concat(&seq2);
    EXPECT_EQ(seq1.getLength(), 4);
    EXPECT_EQ(seq1[0], 1);
    EXPECT_EQ(seq1[3], 4);
}

TEST(ArraySequenceTest, ConcatWithNullDoesNotThrow)
{
    ArraySequence<int> seq;
    EXPECT_NO_THROW(seq.concat(nullptr));
}

TEST(ArraySequenceTest, ConcatImmutableWithNullDoesNotThrow)
{
    ArraySequence<int> seq;
    EXPECT_NO_THROW(seq.concatImmutable(nullptr));
}
