#include "strOp.h"
#include "trans.h"

#include <sstream>
#include <iostream>

#include<boost/tokenizer.hpp>

#ifdef QT_VERSION
std::vector<double> poly(const QString& str)
{
    std::string myString = str.toStdString();

    return poly(myString);
}
#endif

std::vector<double> poly(const std::string &myString)
{
    /*std::stringstream iss( myString );

    double number;
    std::vector<double> res;
    while ( iss >> number )
      res.push_back( number );*/

    std::vector<double> res;
    boost::char_separator<char> sep(" ,");
    boost::tokenizer<boost::char_separator<char>> tok(myString,sep);
    for(boost::tokenizer<boost::char_separator<char>>::iterator beg=tok.begin(); beg!=tok.end();++beg){
        res.push_back(std::stod(*beg));
    }

    std::reverse(res.begin(),res.end());
    return res;
}

/// @brief Convert raw text format to rich text format of a polnomial
/// @param rawText This is like "3 5 0 * 1 2"
/// @return Like "(3s<sup>2</sup>+5s)(s+2)"
/// @note The last coefficient is the const coefficient, the same with MATLAB,
/// different from the polynomials here
std::string getRichTextString(std::string& rawText)
{
    std::string res = "";
    boost::char_separator<char> sep("*");
    boost::tokenizer<boost::char_separator<char>> tok(rawText,sep);
    for(boost::tokenizer<boost::char_separator<char>>::iterator beg=tok.begin(); beg!=tok.end();++beg){
        boost::char_separator<char> sep2(" ,");
        boost::tokenizer<boost::char_separator<char>> tok2(*beg,sep2);
        int n = 0;
        for(auto iter=tok2.begin(); iter!=tok2.end();++iter) ++n;
        int m = n;
        res += "(";
        for(auto iter=tok2.begin(); iter!=tok2.end();++iter){
            --n;
            double co = std::stod(*iter);
            int co_int = static_cast<int>(co);
            bool int_flag = fabs(co_int - co) < 1e-5;
            if(n != m - 1 && n != 0){
                switch (co_int) {
                case 1:
                    if(int_flag)
                        res +=(n == 1)?"+s":"+s<sup>" + std::to_string(n) + "</sup>";
                    break;
                case -1:
                    if(int_flag)
                        res +=(n == 1)?"-s":"-s<sup>" + std::to_string(n) + "</sup>";
                    break;
                default:
                    res += (co > 0)?("+" + *iter):*iter;
                    res += (n == 1)?"s":"s<sup>" + std::to_string(n) + "</sup>";
                    break;
                }
            }
            if(n == m - 1 && n != 0){
                switch (co_int) {
                case 1:
                    res +=(n == 1)?"s":"s<sup>" + std::to_string(n) + "</sup>";
                    break;
                case -1:
                    res +=(n == 1)?"s":"s<sup>" + std::to_string(n) + "</sup>";
                    break;
                default:
                    res += *iter;
                    res += (n == 1)?"s":"s<sup>" + std::to_string(n) + "</sup>";
                    break;
                }
            }
            if(n == 0){
                switch (co_int) {
                case 0:
                    break;
                default:
                    res += (co > 0)?("+" + *iter):*iter;
                    break;
                }
            }
        }
        res += ")";
    }

    return res;
}

std::vector<double> polyFromRawText(std::string& rawText)
{
    std::vector<double> res = {1};
    boost::char_separator<char> sep("*");
    boost::tokenizer<boost::char_separator<char>> tok(rawText,sep);
    for(boost::tokenizer<boost::char_separator<char>>::iterator beg=tok.begin(); beg!=tok.end();++beg){
        auto vec = poly(*beg);
        res = convolution(res,vec);
    }

    return res;
}

std::string getRichTextString(std::vector<double> poly)
{
    std::string res;
    int n = poly.size();
    std::reverse(poly.begin(),poly.end());
    int m = n;
    for(auto iter=poly.begin(); iter!=poly.end();++iter){
        --n;
        double co = *iter;
        int co_int = static_cast<int>(co);

        std::stringstream ss;
        ss<<co;

        std::string   numStr=ss.str();

        bool int_flag = fabs(co_int - co) < 1e-5;
        if(n != m - 1 && n != 0){
            switch (co_int) {
            case 1:
                if(int_flag)
                    res +=(n == 1)?"+s":"+s<sup>" + std::to_string(n) + "</sup>";
                break;
            case -1:
                if(int_flag)
                    res +=(n == 1)?"-s":"-s<sup>" + std::to_string(n) + "</sup>";
                break;
            default:
                res += (co > 0)?("+" + numStr):numStr;
                res += (n == 1)?"s":"s<sup>" + std::to_string(n) + "</sup>";
                break;
            }
        }
        if(n == m - 1 && n != 0){
            switch (co_int) {
            case 1:
                res +=(n == 1)?"s":"s<sup>" + std::to_string(n) + "</sup>";
                break;
            case -1:
                res +=(n == 1)?"s":"s<sup>" + std::to_string(n) + "</sup>";
                break;
            default:
                res += numStr;
                res += (n == 1)?"s":"s<sup>" + std::to_string(n) + "</sup>";
                break;
            }
        }
        if(n == 0){
            switch (co_int) {
            case 0:
                break;
            default:
                res += (co > 0)?("+" + numStr):numStr;
                break;
            }
        }
    }

    return res;
}
