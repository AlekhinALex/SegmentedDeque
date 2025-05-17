#include <gtest/gtest.h>
#include "../inc/segmentedDeque.hpp"
#include <functional>

using IntFunc = int (*)(int);
using StringFunc = std::string (*)(const std::string &);

int square(int x)
{
    return x * x;
}

int cube(int x)
{
    return x * x * x;
}

int increment(int x)
{
    return x + 1;
}

std::string appendExclamation(const std::string &s)
{
    return s + "!";
}

std::string appendQuestion(const std::string &s)
{
    return s + "?";
}

TEST(SegmentedDequeFunctionPointerTest, CanStoreIntFunctionPointers)
{
    SegmentedDeque<IntFunc> funcDeque(2);

    funcDeque.append(square);
    funcDeque.append(cube);
    funcDeque.append(increment);

    EXPECT_EQ(funcDeque.getLength(), 3);

    EXPECT_EQ(funcDeque.get(0)(5), 25);
    EXPECT_EQ(funcDeque.get(1)(3), 27);
    EXPECT_EQ(funcDeque.get(2)(7), 8);

    int result = 0;
    for (const auto &func : funcDeque)
    {
        result += func(2);
    }

    EXPECT_EQ(result, 15);
}

TEST(SegmentedDequeFunctionPointerTest, CanStoreStringFunctionPointers)
{
    SegmentedDeque<StringFunc> funcDeque(2);

    funcDeque.append(appendExclamation);
    funcDeque.append(appendQuestion);

    EXPECT_EQ(funcDeque.getLength(), 2);

    EXPECT_EQ(funcDeque.get(0)("Hello"), "Hello!");
    EXPECT_EQ(funcDeque.get(1)("How are you"), "How are you?");

    std::string baseStr = "Test";
    std::string result;

    for (const auto &func : funcDeque)
    {
        result += func(baseStr);
    }

    EXPECT_EQ(result, "Test!Test?");
}

TEST(SegmentedDequeFunctionPointerTest, CanSortFunctionPointers)
{
    SegmentedDeque<IntFunc> funcDeque(2);

    funcDeque.append(cube);
    funcDeque.append(increment);
    funcDeque.append(square);

    auto comparator = [](IntFunc f1, IntFunc f2)
    {
        return f1(2) < f2(2);
    };

    funcDeque.sort(funcDeque.begin(), funcDeque.end(), comparator);

    EXPECT_EQ(funcDeque.get(0)(2), 3);
    EXPECT_EQ(funcDeque.get(1)(2), 4);
    EXPECT_EQ(funcDeque.get(2)(2), 8);
}

TEST(SegmentedDequeFunctionPointerTest, CanUseHigherOrderFunctions)
{
    SegmentedDeque<IntFunc> funcDeque(2);

    funcDeque.append(square);
    funcDeque.append(cube);

    auto applyTwice = [](IntFunc f, int x) -> int
    {
        return f(f(x));
    };

    int result = 0;
    for (const auto &func : funcDeque)
    {
        result += applyTwice(func, 2);
    }

    EXPECT_EQ(result, 528);
}