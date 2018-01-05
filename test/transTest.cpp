#include "transTest.h"
#include <iostream>

const double genericErr = 1e-3;

void transTestCase1();
void transTestCase2();
void transTestCase3();
// The above three test cases can be seen in the document

void transTestCase4();
void transTestCase5();
void transTestCase6();

void transTest()
{
    transTestCase1();
    transTestCase2();
    transTestCase3();
    transTestCase4();
    transTestCase5();
    transTestCase6();
}

void transTestCase1()
{
    /// \todo Find a suitable solution to test the -180 degree phase crossing frequency is inf
    std::vector<double> num={1,1};
    std::vector<double> den={0,0,1,0.1};
    trans transform1(num,den);
    double margin, freq;
    transform1.phaseMargin(&margin,&freq);
    assert(fuzzyEqual(margin,44.4593,genericErr));
    assert(fuzzyEqual(freq,1.26474,genericErr));
}



void transTestCase2()
{
    // The following system has two -180 degree phase crossing frequencies
    std::vector<double> num={1000,1000.1,100};
    std::vector<double> den={0,0,0,1,.011,.00001};
    trans transform1(num,den);
    double freq,margin;

    transform1.phaseMargin(&margin,&freq);
    assert(fuzzyEqual(margin,40.0107,genericErr));
    assert(fuzzyEqual(freq,78.8029,genericErr));

    // If we provide an intial guess, test if it works well
    transform1.gainMargin(&margin,&freq,200);
    assert(fuzzyEqual(margin,19.8164,genericErr));
    assert(fuzzyEqual(freq,298.325,genericErr));

    // If no initial guess is provided, test if it works well
    transform1.gainMargin(&margin,&freq);
    assert(fuzzyEqual(margin,19.8164,genericErr) || fuzzyEqual(margin,-38.9894,genericErr));
    assert(fuzzyEqual(freq,298.325,genericErr) || fuzzyEqual(freq,3.35205,genericErr));
}

void transTestCase3()
{
    // This is to test the other interface
    // and the convolution of two polynomials
    double margin,freq;
    std::vector<double> a1={1,1};
    std::vector<double> a2={1,.1};
    std::vector<double> b1={0,0,0,1,.01};
    std::vector<double> b2={1,.01};
    trans transform2(convolution(a1,a2),convolution(b1,b2));
    transform2.phaseMargin(&margin,&freq);
    assert(fuzzyEqual(margin,-35.649,genericErr));
    assert(fuzzyEqual(freq,1.15404,genericErr));

    double* ptr = transform2.gainMargin();
    assert(fuzzyEqual(ptr[0],21.3222,genericErr));
    assert(fuzzyEqual(ptr[1],3.58122,genericErr));
    delete [] ptr; /* WARNING: IT IS CRUCIAL!!! */
}


void transTestCase4()
{
    std::vector<double> num={20,5};
    std::vector<double> den={0,1,.55,.025};
    trans transform1(num,den);
    double margin, freq;
    transform1.phaseMargin(&margin,&freq);
    assert(fuzzyEqual(margin,53.5,genericErr));
    assert(fuzzyEqual(freq,9.57,genericErr));
}

void transTestCase5()
{
    std::vector<double> num={10,200};
    std::vector<double> den={0,1,401.1,440.1,40};
    trans transform1(num,den);
    double margin, freq;
    transform1.phaseMargin(&margin,&freq);
    assert(fuzzyEqual(margin,56.89,genericErr));
    assert(fuzzyEqual(freq,0.457,genericErr));
}

void transTestCase6()
{
    std::vector<double> num={60,300};
    std::vector<double> den={0,1,100.2,20.01,1};
    trans transform1(num,den);
    double margin, freq;
    transform1.phaseMargin(&margin,&freq);
    assert(fuzzyEqual(margin,54.9,genericErr));
    assert(fuzzyEqual(freq,2.79,genericErr));
}
