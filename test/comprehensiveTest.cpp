#include "comprehensiveTest.h"
#include "util.h"
#include "../strop.h"
#include "../trans.h"

#include <cassert>

const double genericErr = 1e-3;

void compTestCase1();

void comprehensiveTest()
{
    compTestCase1();
}

void compTestCase1()
{
    std::string numStr = "5*12.5 1";
    std::string denStr = "1 0*1 1*0.5 1*111.111 1";

    std::vector<double> num = polyFromRawText(numStr);
    std::vector<double> den = polyFromRawText(denStr);

    trans tsfunc(num,den);
    double phaseMargin, freq1;
    tsfunc.phaseMargin(&phaseMargin,&freq1);
    double gainMargin, freq2;
    tsfunc.gainMargin(&gainMargin,&freq2);

    assert(fuzzyEqual(phaseMargin,41.6,genericErr));
    assert(fuzzyEqual(freq1,0.4955,genericErr));
}
