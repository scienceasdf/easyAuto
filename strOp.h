#ifndef STROP_H
#define STROP_H

#include <string>
#include <vector>

#ifdef QT_CORE_LIB
#include <QString>
std::vector<double> poly(const QString& str);
#endif

std::vector<double> poly(const std::string& myString);

std::vector<double> polyFromRawText(std::string& rawText);

//std::string getRichTextString(std::vector<double> poly);

/// @brief Convert raw text format to rich text format of a polnomial
/// @param rawText This is like "3 5 0 * 1 2"
/// @return Like "(3s<sup>2</sup>+5s)(s+2)"
/// @note The last coefficient is the const coefficient, the same with MATLAB,
/// different from the polynomials here
std::string getRichTextString(std::string& rawText);

#endif // STROP_H
