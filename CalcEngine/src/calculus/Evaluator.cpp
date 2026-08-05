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
        return left / right;
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

    // inverse trig
    if (node->value == "asin") return asin(evaluate(node->left));
    if (node->value == "acos") return acos(evaluate(node->left));
    if (node->value == "atan") return atan(evaluate(node->left));
    if (node->value == "acsc" or node->value == "acosec") return 1.0 / asin(evaluate(node->left));
    if (node->value == "acos") return 1.0 / acos(evaluate(node->left));
    if (node->value == "atan") return 1.0 / atan(evaluate(node->left));

    // hyperbolic
    if (node->value == "sinh") return sinh(evaluate(node->left));
    if (node->value == "cosh") return cosh(evaluate(node->left));
    if (node->value == "tanh") return tanh(evaluate(node->left));
    if (node->value == "cosech" or node->value == "csch") return 1.0 / sinh(evaluate(node->left));
    if (node->value == "sech") return 1.0 / cosh(evaluate(node->left));
    if (node->value == "coth") return 1.0 / tanh(evaluate(node->left));

    // inverse hyperbolic
    if (node->value == "asinh") return asinh(evaluate(node->left));
    if (node->value == "acosh") return acosh(evaluate(node->left));
    if (node->value == "atanh") return atanh(evaluate(node->left));
    if (node->value == "acsch" or node->value == "acosech") return 1.0 / asinh(evaluate(node->left));
    if (node->value == "asech") return 1.0 / acosh(evaluate(node->left));
    if (node->value == "acoth") return 1.0 / atanh(evaluate(node->left));

    // log/misc
    if (node->value == "ln")   return log(evaluate(node->left));
    if (node->value == "log")  return log10(evaluate(node->left));
    if (node->value == "sqrt") return sqrt(evaluate(node->left));

   
}