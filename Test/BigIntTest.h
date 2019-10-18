#pragma once
#include <gtest/gtest.h>
#include "BigInt.h"
#include <string>
#include <vector>
struct TestCases
{
    std::vector<std::string> A;
    std::vector<std::string> B;
    std::vector<std::string> C;
    size_t                size;
};

namespace details
{

enum Operator : size_t
{
/*  + */  add = 0
/*  - */, sub
/*  * */, mul
/*  / */, div
/*  % */, mod
/* == */, eq
/* != */, neq
/*  ^ */, pow
/*  > */, more
/*  < */, less
/* <= */, leq // less and eq
/* >= */, meq // more and eq
/* << */, lsh
/* >> */, rsh
     , Size
};

extern TestCases casesAdd;
extern TestCases casesSub;
extern TestCases casesMul;


template <Operator opr>
BigInteger::BigInt doing(BigInteger::BigInt const& a, BigInteger::BigInt const& b);
template <>
BigInteger::BigInt doing<Operator::add>(BigInteger::BigInt const& a, BigInteger::BigInt const& b)
{
    return a + b;
}
template <>
BigInteger::BigInt doing<Operator::sub>(BigInteger::BigInt const& a, BigInteger::BigInt const& b)
{
    return a - b;
}
template <>
BigInteger::BigInt doing<Operator::mul>(BigInteger::BigInt const& a, BigInteger::BigInt const& b)
{
    return a * b;
}
template <>
BigInteger::BigInt doing<Operator::div>(BigInteger::BigInt const& a, BigInteger::BigInt const& b)
{
    return a / b;
}
template <>
BigInteger::BigInt doing<Operator::mod>(BigInteger::BigInt const& a, BigInteger::BigInt const& b)
{
    return a % b;
}
template <>
BigInteger::BigInt doing<Operator::eq>(BigInteger::BigInt const& a, BigInteger::BigInt const& b)
{
    using namespace BigInteger;
    return (a == b) ? "1"_BigInt : "0"_BigInt;
}
template <>
BigInteger::BigInt doing<Operator::neq>(BigInteger::BigInt const& a, BigInteger::BigInt const& b)
        {
    using namespace BigInteger;
    return (a != b) ? "1"_BigInt : "0"_BigInt;
}
template <>
BigInteger::BigInt doing<Operator::pow>(BigInteger::BigInt const& a, BigInteger::BigInt const& b)
{
    return a ^ b;
}
template <>
BigInteger::BigInt doing<Operator::more>(BigInteger::BigInt const& a, BigInteger::BigInt const& b)
{
    using namespace BigInteger;
    return (a > b) ? "1"_BigInt : "0"_BigInt;
}
template <>
BigInteger::BigInt doing<Operator::less>(BigInteger::BigInt const& a, BigInteger::BigInt const& b)
{
    using namespace BigInteger;
    return (a < b) ? "1"_BigInt : "0"_BigInt;
}
template <>
BigInteger::BigInt doing<Operator::leq>(BigInteger::BigInt const& a, BigInteger::BigInt const& b)
{
    using namespace BigInteger;
    return (a <= b) ? "1"_BigInt : "0"_BigInt;
}
template <>
BigInteger::BigInt doing<Operator::meq>(BigInteger::BigInt const& a, BigInteger::BigInt const& b)
{
    using namespace BigInteger;
    return (a >= b) ? "1"_BigInt : "0"_BigInt;
}
template <>
BigInteger::BigInt doing<Operator::lsh>(BigInteger::BigInt const& a, BigInteger::BigInt const& b)
{
    auto str = b.toString();
    return a << std::stoi(str);
}
template <>
BigInteger::BigInt doing<Operator::rsh>(BigInteger::BigInt const& a, BigInteger::BigInt const& b)
{
    auto str = b.toString();
    return a >> std::stoi(str);
}

};

class BigIntTest : public ::testing::Test
{
public:

public:
    void check(details::Operator opr, TestCases cases)
    {
        for(size_t i = 0; i < cases.size; i++)
        {
            a.setByString(cases.A[i]);
            b.setByString(cases.B[i]);
            c = arr[opr](a ,b);
            EXPECT_STRCASEEQ(cases.C[i].c_str(), c.toString().c_str()) << "Try " << cases.C[i] << " != " << c.toString() << " find";
            //EXPECT_EQ(true, false) << "Try" << cases.C[i] << " != " << c.toString() << " find";
        }
    }

private:
    BigInteger::BigInt a,b,c;
    std::array<std::function<BigInteger::BigInt(BigInteger::BigInt const& a, BigInteger::BigInt const& b)>, details::Operator::Size> arr =
    {
              details::doing<details::Operator::add>
            , details::doing<details::Operator::sub>
            , details::doing<details::Operator::mul>
            , details::doing<details::Operator::div>
            , details::doing<details::Operator::mod>
            , details::doing<details::Operator::eq>
            , details::doing<details::Operator::neq>
            , details::doing<details::Operator::pow>
            , details::doing<details::Operator::more>
            , details::doing<details::Operator::less>
            , details::doing<details::Operator::leq> // less and eq
            , details::doing<details::Operator::meq> // more and eq
            , details::doing<details::Operator::lsh>
            , details::doing<details::Operator::rsh>
    };
};

TEST_F(BigIntTest, add)
{
    details::casesAdd.size = details::casesAdd.A.size();
    check(details::Operator::add, details::casesAdd);
}

TEST_F(BigIntTest, sub)
{
    details::casesSub.size = details::casesSub.A.size();
    check(details::Operator::sub, details::casesSub);
}

TEST_F(BigIntTest, mul)
{
    details::casesMul.size = details::casesMul.A.size();
    check(details::Operator::mul, details::casesMul);
}