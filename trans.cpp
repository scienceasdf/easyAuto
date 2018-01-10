/// @file basis.cpp
/// @author SHEN Weihong
/// @date 2017/9/7
/// @section This file implements basic polynomials, transform functions of minimum phase systems

#include "trans.h"
#include <algorithm>
#include <Eigen/Core>
#include <Eigen/Eigenvalues>

inline double norm(std::complex<double> &a)
{
    return a.real()*a.real()+a.imag()*a.imag();
}

/// @brief Calculating the i'th value of the convolution of two arrays
/// @param x The first array
/// @param y The second array
double convolution(const std::vector<double>& x, const std::vector<double>& y, int n)
{
    double result = 0;
    int an = x.size();
    int bn = y.size();

    int a = 0, b = n;
    for(int i = 0;i <= n; ++i){
        result += (a < an && b < bn) ? x[a] * y[b] : 0;
        ++a; --b;
    }
    return result;
}

/// @brief Calculating the convolution of two arrays
/// @param x The first array
/// @param y The second array
/// It can also represent the multiplier result of two polynomials
std::vector<double> convolution(const std::vector<double>& x, const std::vector<double>& y)
{
    int an = x.size();
    int bn = y.size();
    int m = an + bn - 1;
    std::vector<double> poly(m);
    for(int i = 0; i < m; ++i){
        poly[i] = convolution(x,y,i);
    }
    return poly;
}

/// @brief Calculating the amplitude at the given frequency
/// @param freq Frequency
/// @return Amplitude
double trans::amplitude(double freq)
{
    std::complex<double> s(0,freq);
    double amp1 = std::abs(polyCalc(mNum.begin(),mNum.end(),s));
    double amp2 = std::abs(polyCalc(mDen.begin(),mDen.end(),s));
    return amp1 / amp2;
}

/// @brief Calculating the phase at the given frequency
/// @param freq Frequency
/// @return Phase, expressed in degrees
double trans::phase(double freq)
{
    std::complex<double> s(0,freq);
    double phase1 = std::arg(polyCalc(mNum.begin(),mNum.end(),s));
    double phase2 = std::arg(polyCalc(mDen.begin(),mDen.end(),s));
    phase1 = (phase1 > 0) ? phase1 : 2* pi + phase1;
    phase2 = (phase2 > 0) ? phase2 : 2* pi + phase2;
    return (phase1 - phase2) * degPerRad;
}


/// @brief Calculating the Decibel number at the given frequency
/// @param freq Frequency
/// @return Decibel number, 20*log(amplitude)
double trans::dB(double freq)
{
    return 20.0 * std::log10(amplitude(freq));
}

/// @brief Calculating the gain margin and the -180 degree phase crossing frequency
/// @param margin Pointer to return the gain margin value
/// @param freq Pointer to return the -180 degree phase crossing frequency
/// @param initialGuess The initial guess users give, optional parameter with defualt value 1.0
/// The gain margin and the -180 degree phase crossing frequency are calculated by secant method
/// The two result values are returned by pointers
/// @todo The initial guess need to be optimized, or it may not convergent
/// @warning For the systems that have two or more -180 degree phase crossing frequency, it can only get one result
void trans::gainMargin(double* margin, double* freq, double initialGuess)
{
    double x0 = initialGuess;
    double x1 = 1.05 * x0;
    double x,y0,y1;
    do
    {
        x = x1;
        x1 = x1-(180.0 + phase(x1)) / (phase(x1) - phase(x0)) * (x1 - x0);
        x0 = x;
    } while(std::abs(phase(x1) + 180.0) > 1e-5);
    *freq = x1;
    *margin = -dB(x1);
}

/// @brief Calculating the phase margin and the unity-gain frequency, or gain crossover frequency
/// @param margin Pointer to return the phase margin value, expressed in degrees
/// @param freq Pointer to return the gain crossover frequency
/// @param initialGuess The initial guess users give, optional parameter with defualt value 1.0
/// The phase margin and the gain crossover frequency are calculated by secant method
/// The two result values are returned by pointers
/// @todo The initial guess need to be optimized, or it may not convergent
void trans::phaseMargin(double *margin, double *freq, double initialGuess)
{
    double x0 = initialGuess;
    double x1 = 1.5 * x0;
    double x,y0,y1;
    int iter=0;
    do
    {
        x = x1;
        y0 = 1.0 - amplitude(x0);
        y1 = 1.0 - amplitude(x1);
        x1 = (y1 == y0)?.5 * x1:x1 - y1 / (y1 - y0) * (x1 - x0);
        x0 = x;
        ++iter;
    } while(std::abs(amplitude(x1) - 1.0) > 1e-5 && iter<20);

    if(iter>=20 || x1 < 0){
        x0 = initialGuess * .03;
        x1 = 1.5 * x0;
        x,y0,y1;
        iter=0;
        do
        {
            x = x1;
            y0 = 1.0 - amplitude(x0);
            y1 = 1.0 - amplitude(x1);
            x1 = (y1 == y0)?.5 * x1:x1 - y1 / (y1 - y0) * (x1 - x0);
            x0 = x;
            ++iter;
        } while(std::abs(amplitude(x1) - 1.0) > 1e-5 && iter<20);
    }


    *freq = x1;
    *margin = phase(x1) + 180.0;
    if(std::isinf(x1)){
        *margin = phase(1e8) + 180.0;
    }
    if(*margin > 180){
        *margin = *margin - 360.0;
    }

}

/// @brief Calculating the gain margin and the -180 degree phase crossing frequency
/// @param initialGuess The initial guess users give, optional parameter with defualt value 1.0
/// @return The pointer to the result array,
/// the first element is gain margin while the second is the -180 degrees cross frequency
/// The gain margin and the -180 degree phase crossing frequency are calculated by secant method
/// @warning Users must release the memory of the pointer manually
/// @todo The initial guess need to be optimized, or it may not convergent
double* trans::gainMargin(double initialGuess)
{
    double* res_ptr = new double[2];
    double x0 = initialGuess;
    double x1 = 1.05 * x0;
    double x,y0,y1;
    do
    {
        x = x1;
        x1 = x1-(180.0 + phase(x1)) / (phase(x1) - phase(x0)) * (x1 - x0);
        x0 = x;
    } while(std::abs(phase(x1) + 180.0) > 1e-5);

    res_ptr[0] = -dB(x1);   // The gain margin
    res_ptr[1] = x1;    // The -180 degree cross frequency
    return res_ptr;
}

/// @brief Calculating the phase margin and the unity-gain frequency, or gain crossover frequency
/// @param initialGuess The initial guess users give, optional parameter with defualt value 1.0
/// @return The pointer to the result array, the first element is phase margin while the second is the crossover frequency
/// The phase margin and the gain crossover frequency are calculated by secant method
/// @warning Users must release the memory of the pointer manually
/// @todo The initial guess need to be optimized, or it may not convergent
double* trans::phaseMargin(double initialGuess)
{
    double* res_ptr = new double[2]; // A pointer to the result array

    double x0 = initialGuess;
    double x1 = 1.05 * x0;
    double x,y0,y1;
    do
    {
        x = x1;
        y0 = 1.0 - amplitude(x0);
        y1 = 1.0 - amplitude(x1);
        x1 = x1 - y1 / (y1 - y0) * (x1 - x0);
        x0 = x;
    } while(std::abs(amplitude(x1) - 1.0) > 1e-5);

    if(std::isinf(x1)) x1=1e8;

    res_ptr[0] = phase(x1) + 180.0; // Phase margin
    res_ptr[1] = x1;    // Crossover frequency

    return res_ptr;
}


std::vector<double> coeff(const std::vector<double> &num, const std::vector<double> &den, double gain)
{
    int big,small;
    bool flag;
    if(num.size() > den.size()){
        big = num.size();
        small = den.size();
        flag = true;
    }
    else{
        big = den.size();
        small = num.size();
        flag = false;
    }

    std::vector<double> res;
    res.reserve(big);

    for(int i=0; i<big; ++i){
        if(i < small){
            res[i] = gain * num[i] + den[i];
        }
        else{
            res[i] = (flag)?gain * num[i]:den[i];
        }
    }

    return res;
}

std::vector<std::complex<double>> roots
(const std::vector<double> coeffs)
{
    int matsz = coeffs.size() - 1;
    std::vector<std::complex<double>> vret;
    Eigen::MatrixXd companion_mat(matsz,matsz);

    static std::vector<std::complex<double>> pre(8);

    if(pre.size() < matsz) pre.resize(matsz);

    for(int i=0; i<matsz; ++i){
        for(int j=0; j<matsz; ++j){
            if(j == matsz - 1){
                companion_mat(i,j)=-coeffs[i] / coeffs[matsz];
            }
            else if(i == j + 1){
                companion_mat(i,j)=1.0;
            }
            else{
                companion_mat(i,j)=0;
            }
        }
    }

    auto eivals = companion_mat.eigenvalues();

    std::vector<std::complex<double>> res;
    res.reserve(matsz);
    for(int i=0; i<matsz; ++i){
        res.push_back(eivals(i));
    }


    for(int i=0; i<matsz; ++i){
        std::swap(res[i],*std::min_element(res.begin()+i,res.end(),
        [&i](std::complex<double> &a, std::complex<double> &b)  // static variables don't need to be captured
        {return norm(a-pre[i]) < norm(b-pre[i]);}));

        pre[i] = res[i];
    }

    return res;
}

/// @brief Calculating the unit step response of a transform function
/// @param time The simulation end time (The begin time is 0)
/// @param slices The slices of the simulation, thus the step time is time divide slices
/// Thanks to C++11, the move semantic can make return elegant
std::vector<double> trans::unitStepResponse(double time, int slices)
{
    int n = mDen.size() - 1;
    Eigen::MatrixXd A(n,n);
    Eigen::MatrixXd B(n,1);
    Eigen::MatrixXd C(1,n);
    std::vector<double> res(slices);

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(i == j + 1){
                A(i,j) = 1.0;
            }
            else if(j == n - 1){
                A(i,j) = mDen[i] / mDen[n];
            }
            else{
                A(i,j) = 0;
            }
        }
    }
    return res;
}
