#include "..\strOp.h"
#include "stroptest.h"

#include <cassert>
#include <iostream>

void richTextTestCase1();
void rawTextToPolyTest1();
void rawTextToPolyTest2();
void rawTextToPolyTest3();

void strOpTest()
{
    richTextTestCase1();
    rawTextToPolyTest1();
    rawTextToPolyTest2();
    rawTextToPolyTest3();
}

void richTextTestCase1()
{
    std::string a = "1 2 3 * -3 1 * 2,-1*2,-1 0";
    assert(getRichTextString(a) == "(s<sup>2</sup>+2s+3)(-3s+1)(2s-1)(2s<sup>2</sup>-s)");
}

void rawTextToPolyTest1()
{
    // NOTE!!! The last coefficient is the const coefficient
    std::string a = "1 1 * -1 1";
    std::vector<double> b = {1,0,-1};
    auto x = polyFromRawText(a);
    assert(polyFromRawText(a) == b);
}

void rawTextToPolyTest2()
{
    std::string a = "1 1 * -1 1 0";
    std::vector<double> b = {0,1,0,-1};
    assert(polyFromRawText(a) == b);
}

void rawTextToPolyTest3()
{
    std::string a = "1 2 3 * -3 1";
    std::vector<double> b = {3   , -7  ,  -5    , -3};
    auto poly = polyFromRawText(a);
    assert(poly == b);
    assert(getRichTextString(poly) == "-3s<sup>3</sup>-5s<sup>2</sup>-7s+3");
}
