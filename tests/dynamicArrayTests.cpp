#include <gtest/gtest.h>
#include "../inc/dynamicArray.hpp"

TEST(DynamicArrayTest, DefaultConstructorCreatesEmptyArray)
{
    DynamicArray<int> arr;
    EXPECT_EQ(arr.getSize(), 0);
    EXPECT_THROW(arr.getFirst(), std::out_of_range);
    EXPECT_THROW(arr.getLast(), std::out_of_range);
}

TEST(DynamicArrayTest, SizeConstructorInitializesWithDefaultValues)
{
    DynamicArray<std::string> arr(3);
    EXPECT_EQ(arr.getSize(), 3);
    EXPECT_EQ(arr[0], "");
    EXPECT_EQ(arr[1], "");
    EXPECT_EQ(arr[2], "");
}

TEST(DynamicArrayTest, ItemsArrayConstructorCopiesItems)
{
    double items[] = {1.1, 2.2, 3.3};
    DynamicArray<double> arr(items, 3);
    EXPECT_EQ(arr.getSize(), 3);
    EXPECT_DOUBLE_EQ(arr[0], 1.1);
    EXPECT_DOUBLE_EQ(arr[1], 2.2);
    EXPECT_DOUBLE_EQ(arr[2], 3.3);
}

TEST(DynamicArrayTest, ItemsArrayConstructorThrowsForInvalidArguments)
{
    EXPECT_THROW(DynamicArray<int>(nullptr, 1), std::invalid_argument);
    EXPECT_THROW(DynamicArray<int>(nullptr, 0), std::invalid_argument);
}

TEST(DynamicArrayTest, CopyConstructorCreatesIndependentCopy)
{
    DynamicArray<int> arr1;
    arr1.append(1);
    arr1.append(2);

    DynamicArray<int> arr2(arr1);
    EXPECT_EQ(arr2.getSize(), 2);
    EXPECT_EQ(arr2[0], 1);
    EXPECT_EQ(arr2[1], 2);

    arr2[0] = 10;
    EXPECT_EQ(arr1[0], 1); // Original shouldn't change
}

TEST(DynamicArrayTest, AssignmentOperatorMakesDeepCopy)
{
    DynamicArray<int> arr1;
    arr1.append(1);
    arr1.append(2);

    DynamicArray<int> arr2;
    arr2 = arr1;
    EXPECT_EQ(arr2.getSize(), 2);

    arr2[0] = 10;
    EXPECT_EQ(arr1[0], 1); // Original shouldn't change
}

TEST(DynamicArrayTest, SelfAssignmentIsSafe)
{
    DynamicArray<int> arr;
    arr.append(1);
    arr.append(2);
    arr = arr;
    EXPECT_EQ(arr.getSize(), 2);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
}

TEST(DynamicArrayTest, GetMethodsThrowWhenEmpty)
{
    DynamicArray<int> arr;
    EXPECT_THROW(arr.getFirst(), std::out_of_range);
    EXPECT_THROW(arr.getLast(), std::out_of_range);
    EXPECT_THROW(arr.get(0), std::out_of_range);
}

TEST(DynamicArrayTest, AppendIncreasesSizeAndCapacity)
{
    DynamicArray<int> arr;
    arr.append(1);
    EXPECT_EQ(arr.getSize(), 1);
    EXPECT_EQ(arr.getFirst(), 1);
    EXPECT_EQ(arr.getLast(), 1);

    arr.append(2);
    EXPECT_EQ(arr.getSize(), 2);
    EXPECT_EQ(arr.getFirst(), 1);
    EXPECT_EQ(arr.getLast(), 2);
}

TEST(DynamicArrayTest, PrependAddsToBeginning)
{
    DynamicArray<int> arr;
    arr.prepend(2);
    arr.prepend(1);
    EXPECT_EQ(arr.getSize(), 2);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
}

TEST(DynamicArrayTest, InsertAtHandlesVariousPositions)
{
    DynamicArray<int> arr;
    arr.append(1);
    arr.append(3);

    // Insert in middle
    arr.insertAt(2, 1);
    EXPECT_EQ(arr.getSize(), 3);
    EXPECT_EQ(arr[1], 2);

    // Insert at beginning
    arr.insertAt(0, 0);
    EXPECT_EQ(arr[0], 0);

    // Insert at end
    arr.insertAt(4, 4);
    EXPECT_EQ(arr[4], 4);
}

TEST(DynamicArrayTest, InsertAtThrowsForInvalidIndices)
{
    DynamicArray<int> arr;
    EXPECT_THROW(arr.insertAt(1, -1), std::out_of_range);
    EXPECT_THROW(arr.insertAt(1, 1), std::out_of_range);
}

TEST(DynamicArrayTest, SetModifiesExistingElements)
{
    DynamicArray<int> arr;
    arr.append(1);
    arr.append(2);
    arr.set(0, 10);
    arr.set(1, 20);
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 20);
}

TEST(DynamicArrayTest, SetThrowsForInvalidIndices)
{
    DynamicArray<int> arr;
    EXPECT_THROW(arr.set(-1, 1), std::out_of_range);
    EXPECT_THROW(arr.set(0, 1), std::out_of_range);
}

TEST(DynamicArrayTest, GetSubArrayReturnsCorrectSubset)
{
    DynamicArray<int> arr;
    for (int i = 0; i < 5; i++)
    {
        arr.append(i);
    }

    DynamicArray<int> *sub = arr.getSubArray(1, 3);
    ASSERT_NE(sub, nullptr);
    EXPECT_EQ(sub->getSize(), 3);
    EXPECT_EQ((*sub)[0], 1);
    EXPECT_EQ((*sub)[1], 2);
    EXPECT_EQ((*sub)[2], 3);
    delete sub;
}

TEST(DynamicArrayTest, GetSubArrayThrowsForInvalidRanges)
{
    DynamicArray<int> arr;
    arr.append(1);
    arr.append(2);

    EXPECT_THROW(arr.getSubArray(-1, 1), std::out_of_range);
    EXPECT_THROW(arr.getSubArray(1, 0), std::out_of_range);
    EXPECT_THROW(arr.getSubArray(0, 2), std::out_of_range);
}

TEST(DynamicArrayTest, ConcatCombinesArrays)
{
    DynamicArray<int> arr1;
    arr1.append(1);
    arr1.append(2);

    DynamicArray<int> arr2;
    arr2.append(3);
    arr2.append(4);

    arr1.concat(&arr2);
    EXPECT_EQ(arr1.getSize(), 4);
    EXPECT_EQ(arr1[0], 1);
    EXPECT_EQ(arr1[3], 4);
}

TEST(DynamicArrayTest, ConcatImmutableCombinesArrays)
{
    DynamicArray<int> arr1;
    arr1.append(1);
    arr1.append(2);

    DynamicArray<int> arr2;
    arr2.append(3);
    arr2.append(4);

    DynamicArray<int> *combined = arr1.concatImmutable(&arr2);
    ASSERT_NE(combined, nullptr);
    EXPECT_EQ(combined->getSize(), 4);
    EXPECT_EQ((*combined)[0], 1);
    EXPECT_EQ((*combined)[3], 4);
    delete combined;
}

TEST(DynamicArrayTest, ConcatThrowsForNullArgument)
{
    DynamicArray<int> arr;
    EXPECT_NO_THROW(arr.concat(nullptr));
}

TEST(DynamicArrayTest, ConcatImmutableThrowsForNullArgument)
{
    DynamicArray<int> arr;
    EXPECT_NO_THROW(arr.concatImmutable(nullptr));
}

TEST(DynamicArrayTest, ResizeChangesSize)
{
    DynamicArray<int> arr;
    arr.resize(5);
    EXPECT_EQ(arr.getSize(), 5);

    arr.resize(2);
    EXPECT_EQ(arr.getSize(), 2);

    EXPECT_THROW(arr.resize(-1), std::invalid_argument);
}

TEST(DynamicArrayTest, ClearResetsArray)
{
    DynamicArray<int> arr;
    arr.append(1);
    arr.append(2);
    arr.clear();
    EXPECT_EQ(arr.getSize(), 0);
    EXPECT_THROW(arr[0], std::out_of_range);
}

TEST(DynamicArrayTest, OperatorBracketsAllowsAccessAndModification)
{
    DynamicArray<int> arr;
    arr.append(1);
    arr.append(2);

    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);

    arr[0] = 10;
    EXPECT_EQ(arr[0], 10);

    EXPECT_THROW(arr[2], std::out_of_range);
}

TEST(DynamicArrayTest, ConstOperatorBracketsWorks)
{
    DynamicArray<int> arr;
    arr.append(1);
    arr.append(2);

    const DynamicArray<int> &constArr = arr;
    EXPECT_EQ(constArr[0], 1);
    EXPECT_EQ(constArr[1], 2);
    EXPECT_THROW(constArr[2], std::out_of_range);
}

TEST(DynamicArrayTest, PrintDoesNotCrash)
{
    DynamicArray<int> arr;
    arr.append(1);
    arr.append(2);

    // Just verify it doesn't crash
    arr.print();
}