/// @file basis.h
/// @author SHEN Weihong
/// @date 2017/9/7
/// @section This file implements basic polynomials, transform functions of minimum phase systems

#ifndef BASIS_H
#define BASIS_H

#include <vector>
#include <complex>
#include <cmath>
#include <Eigen/Core>

const double pi = 3.141592653589793;
const double degPerRad = 180.0 / pi;
const double radPerDeg = pi / 180.0;

/// @brief Calculate the value of a polynomial
/// @param first A pointer of an array, or an iterator of a container, representing the coefficient of @f$ a_0 @f$
/// @param last A pointer of an array, or an iterator of a container, representing the coefficient of @f$ a_{n-1} @f$
/// @param x The value representing the varible of the polynomial
/// @f$ f(x)=a_0+a_1x+a_2x^2+...+a_{n-1}x^{n-1} @f$, so the first number in the array, *first, represents @f$ a_0 @f$
/// That is contrary to the format of MATLAB
template <class iter, class numArea>
numArea polyCalc(iter first, iter last, numArea x)
{
    --last;
    numArea result = *last;
    while(first != last){
        --last;
        result = result * x + *last;
    }
    return result;
}

/// @brief Calculating the i'th value of the convolution of two arrays
/// @param x The first array
/// @param y The second array
double convolution(std::vector<double>& x, std::vector<double>& y, int n);

/// @brief Calculating the convolution of two arrays
/// @param x The first array
/// @param y The second array
/// It can also represent the multiplier result of two polynomials
std::vector<double> convolution(std::vector<double>& x, std::vector<double>& y);

/// This is a transform function class, with the implementation of gain margin and phase margin
/// @todo The implementation of std::complex<double> is of low efficiency,
/// try to find an elegant method to improve the efficiency
class trans
{
public:

    /// @brief Constructor that set the numerator and the denominator of a transform function
    /// @param num The numerator polynomial, coefficients stored in std::vector<double>
    /// @param den The denominator polynomial, coefficients stored in std::vector<double>
    /// For compliers not support C++11, no move assignment statement is used here
    trans(std::vector<double> num, std::vector<double> den) :
        mNum(num), mDen(den) {}

    /// @brief Calculating the amplitude at the given frequency
    /// @param freq Frequency
    /// @return Amplitude
    double amplitude(double freq);

    /// @brief Calculating the phase at the given frequency
    /// @param freq Frequency
    /// @return Phase, expressed in degrees
    double phase(double freq);

    /// @brief Calculating the Decibel number at the given frequency
    /// @param freq Frequency
    /// @return Decibel number, 20*log(amplitude)
    double dB(double freq);

    /// @brief Calculating the gain margin and the -180 degree phase crossing frequency
    /// @param margin Pointer to return the gain margin value
    /// @param freq Pointer to return the -180 degree phase crossing frequency
    /// @param initialGuess The initial guess users give, optional parameter with defualt value 1.0
    /// The gain margin and the -180 degree phase crossing frequency are calculated by secant method
    /// The two result values are returned by pointers
    /// /// @warning For the systems that have two or more -180 degree phase crossing frequency, it can only get one result
    void gainMargin(double* margin, double* freq, double initialGuess = 1.0);

    /// @brief Calculating the phase margin and the unity-gain frequency, or gain crossover frequency
    /// @param margin Pointer to return the phase margin value, expressed in degrees
    /// @param freq Pointer to return the gain crossover frequency
    /// @param initialGuess The initial guess users give, optional parameter with defualt value 1.0
    /// The phase margin and the gain crossover frequency are calculated by secant method
    /// The two result values are returned by pointers
    void phaseMargin(double* margin, double* freq, double initialGuess = 1.0);

    /// @brief Calculating the gain margin and the -180 degree phase crossing frequency
    /// @param initialGuess The initial guess users give, optional parameter with defualt value 1.0
    /// @return The pointer to the result array,
    /// the first element is gain margin while the second is the -180 degrees cross frequency
    /// The gain margin and the -180 degree phase crossing frequency are calculated by secant method
    /// @warning Users must release the memory of the pointer manually
    /// @todo The initial guess need to be optimized, or it may not convergent
    double* gainMargin(double initialGuess = 1.0);

    /// @brief Calculating the phase margin and the unity-gain frequency, or gain crossover frequency
    /// @param initialGuess The initial guess users give, optional parameter with defualt value 1.0
    /// @return The pointer to the result array, the first element is phase margin while the second is the crossover frequency
    /// The phase margin and the gain crossover frequency are calculated by secant method
    /// @warning Users must release the memory of the pointer manually
    /// @todo The initial guess need to be optimized, or it may not convergent
    double* phaseMargin(double initialGuess = 1.0);

private:
    std::vector<double> mNum;
    std::vector<double> mDen;
};

std::vector<double> coeff(const std::vector<double> &num, const std::vector<double> &den, double gain);

std::vector<std::complex<double>> roots
(const std::vector<double> coeffs);



#endif // BASIS_H
