#include "Evaluator.h"
#include<iostream>
#include<cctype>
#include<cmath>
#include <corecrt_math_defines.h>
using namespace std;

Evaluator::Evaluator(ASTNode* ast, double x) //construct
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
        return 0;

    if (isdigit(node->value[0]) || (node->value[0] == '-' && node->value.length() > 1))
        return stod(node->value);

    if (node->value == "x")
        return x;

    double left = evaluate(node->left);
    double right = evaluate(node->right);

    if (node->value == "+") 
        return left + right;
    if (node->value == "-")
        return left - right;
    if (node->value == "*")
        return left * right;
    if (node->value == "/")
    {
        double left = evaluate(node->left);
        double right = evaluate(node->right);
        if (right == 0.0)
            throw string("Math error: division by zero");
        return left / right;
    }
    if (node->value == "^")
        return pow(left, right);


    // exponential
    if (node->value == "e") return M_E; // exponent value

    // trig
    if (node->value == "sin")  return sin(evaluate(node->left));
    if (node->value == "cos")  return cos(evaluate(node->left));
    if (node->value == "tan")  return tan(evaluate(node->left));
    if (node->value == "cosec" or node->value == "csc")  return 1.0 / sin(evaluate(node->left));
    if (node->value == "sec")  return 1.0 / cos(evaluate(node->left));
    if (node->value == "cot")  return 1.0 / tan(evaluate(node->left));

    if (node->value == "asin" || node->value == "acos")
    {
        double val = evaluate(node->left);
        if (val < -1.0 || val > 1.0)
            throw string("Math error: " + node->value + " undefined for |x| > 1");
        return node->value == "asin" ? asin(val) : acos(val);
    }

    if (node->value == "atanh")
    {
        double val = evaluate(node->left);
        if (val <= -1.0 || val >= 1.0)
            throw string("Math error: atanh undefined for |x| >= 1");
        return atanh(val);
    }

    if (node->value == "asec" || node->value == "acsc")
    {
        double val = evaluate(node->left);
        if (val > -1.0 && val < 1.0)
            throw string("Math error: " + node->value + " undefined for |x| < 1");
        return node->value == "asec" ? acos(1.0 / val) : asin(1.0 / val);
    }

    if (node->value == "acosh")
    {
        double val = evaluate(node->left);
        if (val < 1.0)
            throw string("Math error: acosh undefined for x < 1");
        return acosh(val);
    }

    if (node->value == "asech")
    {
        double val = evaluate(node->left);
        if (val <= 0.0 || val > 1.0)
            throw string("Math error: asech undefined for x <= 0 or x > 1");
        return acosh(1.0 / val);
    }

    // inverse hyperbolic
    if (node->value == "asinh") return asinh(evaluate(node->left));
    if (node->value == "acosh") return acosh(evaluate(node->left));
    if (node->value == "atanh") return atanh(evaluate(node->left));
    if (node->value == "acsch" or node->value == "acosech") return asinh(1.0 / evaluate(node->left));
    if (node->value == "asech") return acosh(1.0 / evaluate(node->left));
    if (node->value == "acoth") return atanh(1.0 / evaluate(node->left));

    // log/misc
    if (node->value == "ln")
    {
        double val = evaluate(node->left);
        if (val <= 0.0)
            throw string("Math error: ln undefined for x ≤ 0");
        return log(val);
    }

    if (node->value == "log")
    {
        double val = evaluate(node->left);
        if (val <= 0.0)
            throw string("Math error: log undefined for x ≤ 0");
        return log10(val);
    }
    if (node->value == "sqrt")
    {
        double val = evaluate(node->left);
        if (val < 0.0)
            throw string("Math error: sqrt undefined for x < 0");
        return sqrt(val);
    }
    if (node->value == "exp")
    {
        return exp(evaluate(node->left));
    }

    return 0.0;
}