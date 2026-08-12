#include "Evaluator.h"

#include <iostream>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cerrno>
#include <limits>
#include <string>

#include <corecrt_math_defines.h>

using namespace std;


// ============================================================
// Helper functions
// ============================================================

namespace
{
    constexpr double EPS = 1e-12;

    bool isZero(double value)
    {
        return std::abs(value) < EPS;
    }

    bool isFinite(double value)
    {
        return std::isfinite(value);
    }

    // Checks whether a string represents a valid number.
    bool isNumber(const string& text)
    {
        if (text.empty())
            return false;

        char* end = nullptr;
        errno = 0;

        std::strtod(text.c_str(), &end);

        return end != text.c_str() &&
            *end == '\0' &&
            errno != ERANGE;
    }

    [[noreturn]] void mathError(const string& message)
    {
        throw string("Math error: " + message);
    }
}

Evaluator::Evaluator(ASTNode* ast, double x)
{
    node = ast;
    this->x = x;
}

double Evaluator::Result()
{
    return evaluate(node);
}

double Evaluator::evaluate(ASTNode* node)
{
    if (node == nullptr)
        return 0.0;

    // Empty node protection
    if (node->value.empty())
        mathError("empty expression node");

    // Numbers
    if (isNumber(node->value))
        return stod(node->value);

    // Variable
    if (node->value == "x")
        return x;

    // Constants
    if (node->value == "e")
        return M_E;

    if (node->value == "pi")
        return M_PI;

    // BASIC OPERATORS
    if (node->value == "+")
    {
        double left = evaluate(node->left);
        double right = evaluate(node->right);

        return left + right;
    }


    if (node->value == "-")
    {
        double left = evaluate(node->left);
        double right = evaluate(node->right);

        return left - right;
    }


    if (node->value == "*")
    {
        double left = evaluate(node->left);
        double right = evaluate(node->right);

        return left * right;
    }


    if (node->value == "/")
    {
        double left = evaluate(node->left);
        double right = evaluate(node->right);

        if (isZero(right))
            mathError("division by zero");

        return left / right;
    }


    if (node->value == "^")
    {
        double left = evaluate(node->left);
        double right = evaluate(node->right);

        double result = pow(left, right);

        if (std::isnan(result))
            mathError("invalid exponentiation");

        if (std::isinf(result))
            mathError("result overflow in exponentiation");

        return result;
    }

    // TRIG FUNCTIONS
    if (node->value == "sin")
    {
        double val = evaluate(node->left);
        return sin(val);
    }


    if (node->value == "cos")
    {
        double val = evaluate(node->left);
        return cos(val);
    }

    if (node->value == "tan")
    {
        double val = evaluate(node->left);

        if (std::abs(cos(val)) < EPS)
            mathError("tan undefined at this value");

        return tan(val);
    }

    if (node->value == "csc" || node->value == "cosec")
    {
        double val = evaluate(node->left);
        double denominator = sin(val);

        if (std::abs(denominator) < EPS)
            mathError("csc undefined at this value");

        return 1.0 / denominator;
    }

    if (node->value == "sec")
    {
        double val = evaluate(node->left);
        double denominator = cos(val);

        if (std::abs(denominator) < EPS)
            mathError("sec undefined at this value");

        return 1.0 / denominator;
    }

    if (node->value == "cot")
    {
        double val = evaluate(node->left);
        double denominator = tan(val);

        if (std::abs(denominator) < EPS)
            mathError("cot undefined at this value");

        return 1.0 / denominator;
    }

    // INVERSE TRIGONOMETRIC FUNCTIONS
    if (node->value == "asin")
    {
        double val = evaluate(node->left);

        if (val < -1.0 || val > 1.0)
            mathError("asin undefined for |x| > 1");

        return asin(val);
    }


    if (node->value == "acos")
    {
        double val = evaluate(node->left);

        if (val < -1.0 || val > 1.0)
            mathError("acos undefined for |x| > 1");

        return acos(val);
    }


    if (node->value == "atan")
    {
        double val = evaluate(node->left);

        return atan(val);
    }

    if (node->value == "asec")
    {
        double val = evaluate(node->left);

        if (std::abs(val) < 1.0)
            mathError("asec undefined for |x| < 1");

        return acos(1.0 / val);
    }

    if (node->value == "acsc")
    {
        double val = evaluate(node->left);

        if (std::abs(val) < 1.0)
            mathError("acsc undefined for |x| < 1");

        return asin(1.0 / val);
    }


    // Arccot
    // Convention:
    // acot(x) = atan(1/x)
    // acot(0) = pi/2

    if (node->value == "acot")
    {
        double val = evaluate(node->left);

        if (isZero(val))
            return M_PI / 2.0;

        return atan(1.0 / val);
    }

    // HYPERBOLIC FUNCTIONS

    if (node->value == "sinh")
    {
        double val = evaluate(node->left);

        double result = sinh(val);

        if (std::isinf(result))
            mathError("sinh result overflow");

        return result;
    }


    if (node->value == "cosh")
    {
        double val = evaluate(node->left);

        double result = cosh(val);

        if (std::isinf(result))
            mathError("cosh result overflow");

        return result;
    }


    if (node->value == "tanh")
    {
        double val = evaluate(node->left);

        return tanh(val);
    }

    if (node->value == "sech")
    {
        double val = evaluate(node->left);

        double denominator = cosh(val);

        if (isZero(denominator))
            mathError("sech undefined");

        return 1.0 / denominator;
    }

    if (node->value == "csch" || node->value == "cosech")
    {
        double val = evaluate(node->left);

        double denominator = sinh(val);

        if (isZero(denominator))
            mathError("csch undefined at x = 0");

        return 1.0 / denominator;
    }

    if (node->value == "coth")
    {
        double val = evaluate(node->left);

        double denominator = sinh(val);

        if (isZero(denominator))
            mathError("coth undefined at x = 0");

        return cosh(val) / denominator;
    }

    // INVERSE HYPERBOLIC FUNCTIONS

    if (node->value == "asinh")
    {
        double val = evaluate(node->left);

        return asinh(val);
    }

    if (node->value == "acosh")
    {
        double val = evaluate(node->left);

        if (val < 1.0)
            mathError("acosh undefined for x < 1");

        return acosh(val);
    }

    if (node->value == "atanh")
    {
        double val = evaluate(node->left);

        if (val <= -1.0 || val >= 1.0)
            mathError("atanh undefined for |x| >= 1");

        return atanh(val);
    }

    if (node->value == "asech")
    {
        double val = evaluate(node->left);

        if (val <= 0.0 || val > 1.0)
            mathError("asech undefined; domain is 0 < x <= 1");

        return acosh(1.0 / val);
    }

    if (node->value == "acsch" || node->value == "acosech")
    {
        double val = evaluate(node->left);

        if (isZero(val))
            mathError("acsch undefined at x = 0");

        return asinh(1.0 / val);
    }

    if (node->value == "acoth")
    {
        double val = evaluate(node->left);

        if (std::abs(val) <= 1.0)
            mathError("acoth undefined for |x| <= 1");

        return atanh(1.0 / val);
    }

    // LOGARITHMIC FUNCTIONS

    if (node->value == "ln")
    {
        double val = evaluate(node->left);

        if (val <= 0.0)
            mathError("ln undefined for x <= 0");

        return log(val);
    }

    if (node->value == "log")
    {
        double val = evaluate(node->left);

        if (val <= 0.0)
            mathError("log undefined for x <= 0");

        return log10(val);
    }
    // ROOT / EXPONENTIAL

    if (node->value == "sqrt")
    {
        double val = evaluate(node->left);

        if (val < 0.0)
            mathError("sqrt undefined for x < 0");

        return sqrt(val);
    }


    if (node->value == "exp")
    {
        double val = evaluate(node->left);

        double result = exp(val);

        if (std::isinf(result))
            mathError("exp result overflow");

        return result;
    }

    // UNKNOWN FUNCTION / OPERATOR
 
    throw string("Unknown operation: " + node->value);
}