#include "Differentiator.h"
#include <stdexcept> // For exception handling
#include "../calculus/Simplifier.h"
#include "../calculus/ExprPrinter.h"


TangentToCurve::TangentToCurve(ASTNode* ast, double x)
{
	node = ast;
	this->x = x;
}
ASTNode* TangentToCurve::Derivative() //Gives Expression -> Next Phase  Symbolic method
{
	return Differentiate(node);
}

ASTNode* TangentToCurve::Differentiate(ASTNode* node) // Base funtion to get 
{                                                   // actual differentiator
	ASTNode* expression = new ASTNode("0");

	if (!node) return new ASTNode("0");

	if (node->value == "x")
		return new ASTNode("1");

	else if (node->value == "e")
		return new ASTNode("0"); // e is a constant

	else if (isdigit(node->value[0]))
		return new ASTNode("0");

	else if (node->value == "^")
		expression = powerRule(node);

	else if (node->value == "+" || node->value == "-")
		expression = sumRule(node);

	else if (node->value == "*")
		return productRule(node);

	else if (node->value == "/")
		return quotientRule(node);

	else
		expression = chainRule(node);

	return expression;

}

ASTNode* TangentToCurve::copyTree(const ASTNode* node)
{
	if (!node) return nullptr;
	ASTNode* copy = new ASTNode(node->value);
	copy->left = copyTree(node->left);
	copy->right = copyTree(node->right);
	return copy;
}

ASTNode* TangentToCurve::powerRule(ASTNode* node)
{
	// variable exponent case (e.g. e^x)
	if (!isdigit(node->right->value[0]))
	{
		ASTNode* base = new ASTNode(node->left->value);
		base->left = node->left->left;
		base->right = node->left->right;

		ASTNode* exp = new ASTNode(node->right->value);
		exp->left = node->right->left;
		exp->right = node->right->right;

		ASTNode* result = new ASTNode("^");
		result->left = base;
		result->right = exp;

		ASTNode* chain = new ASTNode("*");
		chain->left = result;
		chain->right = Differentiate(node->right);
		return chain;
	}

	// constant exponent — power rule + chain rule
	double val = stod(node->right->value) - 1;
	string expStr = (val == floor(val)) ? to_string((int)val) : to_string(val);

	ASTNode* power = new ASTNode("^");
	power->left = node->left;
	power->right = new ASTNode(expStr);

	ASTNode* coeff = new ASTNode("*");
	coeff->left = node->right;   // n
	coeff->right = power;          // base^(n-1)

	// multiply by derivative of base (chain rule)
	ASTNode* chainPart = Differentiate(node->left);

	ASTNode* result = new ASTNode("*");
	result->left = coeff;
	result->right = chainPart;

	return result;
}
ASTNode* TangentToCurve::sumRule(ASTNode* node) // d/dx ( f(x) + g(x) )
{

	ASTNode* expression = new ASTNode(node->value);
	
	expression->left = Differentiate(node->left);
	expression->right = Differentiate(node->right);

	return expression;
}
ASTNode* TangentToCurve::productRule(ASTNode* node) // d/dx (x*y) = d/dx(x) * y + d/dx(y) * x
{
	ASTNode* expression = new ASTNode("+");
	ASTNode* left = new ASTNode("*");
	ASTNode* right = new ASTNode("*");
	left->left = Differentiate(node->left);
	left->right = copyTree(node->right);
	right->left = Differentiate(node->right);
	right->right = copyTree(node->left);

	expression->left = left;
	expression->right = right;

	return expression;

}

ASTNode* TangentToCurve::quotientRule(ASTNode* node) // (u/v)' = (u'v - uv') / (v^2)
{

	ASTNode* divide = new ASTNode("/");

	ASTNode* numerator = new ASTNode("-");

	ASTNode* leftMul = new ASTNode("*");
	ASTNode* rightMul = new ASTNode("*");

	// u' * v
	leftMul->left = Differentiate(node->left);
	leftMul->right = copyTree(node->right);  // v

	// u * v'
	rightMul->left = copyTree(node->left);   // u
	rightMul->right = Differentiate(node->right);

	numerator->left = leftMul;
	numerator->right = rightMul;

	ASTNode* denominator = new ASTNode("^");
	denominator->left = copyTree(node->right); // v again	
	denominator->right = new ASTNode("2");

	divide->left = numerator;
	divide->right = denominator;

	return divide;
}


ASTNode* TangentToCurve::chainRule(ASTNode* node) // f'(g(x))*g'(x) for trigno functions
{
	// no right node val for trigno and algo funs
	if (node->value == "sin")
	{
		ASTNode* cosNode = new ASTNode("cos");
		cosNode->left = node->left;          // keep same argument

		ASTNode* result = new ASTNode("*");
		result->left = cosNode;              // cos(g(x))
		result->right = Differentiate(node->left); // g'(x)
		return result;
	}

	else if (node->value == "cos")
	{
		ASTNode* sinNode = new ASTNode("sin");
		sinNode->left = node->left;

		ASTNode* neg = new ASTNode("*");
		neg->left = new ASTNode("-1");
		neg->right = sinNode;

		ASTNode* result = new ASTNode("*");
		result->left = neg;
		result->right = Differentiate(node->left);
		return result;
	}

	else if (node->value == "tan") // d/dx tan x = 1 / cos^2 x
	{

		// Through tree tracing

		ASTNode* cosNode = new ASTNode("cos");
		cosNode->left = node->left;

		ASTNode* pow = new ASTNode("^");
		pow->left = cosNode;
		pow->right = new ASTNode("2");

		ASTNode* divide = new ASTNode("/");
		divide->left = new ASTNode("1");
		divide->right = pow;

		ASTNode* principle = new ASTNode("*");
		principle->left = divide;
		principle->right = Differentiate(node->left);

		return principle;

	}

	else if (node->value == "asin") // asin x  → 1 / sqrt(1 - x^2)
	{
		ASTNode* pow = new ASTNode("^");
		pow->left = node->left;
		pow->right = new ASTNode("2");

		ASTNode* neg = new ASTNode("-");
		neg->left = new ASTNode("1");
		neg->right = pow;

		ASTNode* sqrt = new ASTNode("sqrt");
		sqrt->left = neg;

		ASTNode* div = new ASTNode("/");
		div->left = new ASTNode("1");
		div->right = sqrt;

		ASTNode* principle = new ASTNode("*");
		principle->left = div;
		principle->right = Differentiate(node->left);

		return principle;


	}
	else if (node->value == "acos") // acos x → -1 / sqrt(1 - x^2)
	{

		ASTNode* pow = new ASTNode("^");
		pow->left = node->left;
		pow->right = new ASTNode("2");

		ASTNode* neg = new ASTNode("-");
		neg->left = new ASTNode("1");
		neg->right = pow;

		ASTNode* sqrt = new ASTNode("sqrt");
		sqrt->left = neg;

		ASTNode* div = new ASTNode("/");
		div->left = new ASTNode("-1");
		div->right = sqrt;

		ASTNode* principle = new ASTNode("*");
		principle->left = div;
		principle->right = Differentiate(node->left);

		return principle;


	}

	else if (node->value == "atan") // atan x → 1 / (1 + x^2)
	{
		ASTNode* pow = new ASTNode("^");
		pow->left = node->left;
		pow->right = new ASTNode("2");

		ASTNode* pos = new ASTNode("+");
		pos->left = new ASTNode("1");
		pos->right = pow;

		ASTNode* div = new ASTNode("/");
		div->left = new ASTNode("1");
		div->right = pos;

		ASTNode* principle = new ASTNode("*");
		principle->left = div;
		principle->right = Differentiate(node->left);

		return principle;

	}
	else if (node->value == "sinh") // sinh x → cosh(x)
	{
		ASTNode* cosNode = new ASTNode("cosh");
		cosNode->left = node->left;          // keep same argument

		ASTNode* result = new ASTNode("*");
		result->left = cosNode;              // cos(g(x))
		result->right = Differentiate(node->left); // g'(x)
		return result;

	}
	else if (node->value == "cosh") // cosh → sinh(x)
	{

		ASTNode* sinNode = new ASTNode("sinh");
		sinNode->left = node->left;

		ASTNode* neg = new ASTNode("*");
		neg->left = new ASTNode("1");
		neg->right = sinNode;

		ASTNode* result = new ASTNode("*");
		result->left = neg;
		result->right = Differentiate(node->left);
		return result;

	}
	else if (node->value == "tanh") // tanh → 1 / cosh^2(x)
	{
		ASTNode* cosNode = new ASTNode("cosh");
		cosNode->left = node->left;

		ASTNode* pow = new ASTNode("^");
		pow->left = cosNode;
		pow->right = new ASTNode("2");

		ASTNode* divide = new ASTNode("/");
		divide->left = new ASTNode("1");
		divide->right = pow;

		ASTNode* principle = new ASTNode("*");
		principle->left = divide;
		principle->right = Differentiate(node->left);

		return principle;
	}
	else if (node->value == "asinh") // asinh → 1 / sqrt(x^2 + 1)
	{
		ASTNode* pow = new ASTNode("^");
		pow->left = node->left;
		pow->right = new ASTNode("2");

		ASTNode* pos = new ASTNode("+");
		pos->left = new ASTNode("1");
		pos->right = pow;

		ASTNode* sqrt = new ASTNode("sqrt");
		sqrt->left = pos;

		ASTNode* div = new ASTNode("/");
		div->left = new ASTNode("1");
		div->right = sqrt;

		ASTNode* principle = new ASTNode("*");
		principle->left = div;
		principle->right = Differentiate(node->left);

		return principle;

	}
	else if (node->value == "acosh") // acosh → 1 / sqrt(x^2 - 1)
	{
		ASTNode* pow = new ASTNode("^");
		pow->left = node->left;
		pow->right = new ASTNode("2");

		ASTNode* neg = new ASTNode("+");
		neg->left = new ASTNode("-1");
		neg->right = pow;

		ASTNode* sqrt = new ASTNode("sqrt");
		sqrt->left = neg;

		ASTNode* div = new ASTNode("/");
		div->left = new ASTNode("1");
		div->right = sqrt;

		ASTNode* principle = new ASTNode("*");
		principle->left = div;
		principle->right = Differentiate(node->left);

		return principle;

	}
	else if (node->value == "atanh") // atanh → 1 / (1 - x^2)
	{

		ASTNode* pow = new ASTNode("^");
		pow->left = node->left;
		pow->right = new ASTNode("2");

		ASTNode* neg = new ASTNode("-");
		neg->left = new ASTNode("1");
		neg->right = pow;

		ASTNode* div = new ASTNode("/");
		div->left = new ASTNode("1");
		div->right = neg;

		ASTNode* principle = new ASTNode("*");
		principle->left = div;
		principle->right = Differentiate(node->left);

		return principle;

	}
	else if (node->value == "ln") // sqrt x  → 1 / x
	{
		ASTNode* div = new ASTNode("/");
		div->left = new ASTNode("1");
		div->right = node->left;

		ASTNode* exp = new ASTNode("*");
		exp->left = div;
		exp->right = Differentiate(node->left);

		return exp;

	}
	else if (node->value == "log") // log x → 1 / (x * sqrt(10))
	{
		ASTNode* ln = new ASTNode("ln");
		ln->left = new ASTNode("10");

		ASTNode* prod = new ASTNode("*");
		prod->left = node->left;
		prod->right = ln;

		ASTNode* div = new ASTNode("/");
		div->left = new ASTNode("1");
		div->right = prod;

		ASTNode* exp = new ASTNode("*");
		exp->left = div;
		exp->right = Differentiate(node->left);

		return exp;

	}
	else if (node->value == "sqrt") // sqrt → 1 / (2 * sqrt(x))
	{
		ASTNode* sqrt = new ASTNode("sqrt");
		sqrt->left = node->left;

		ASTNode* prod = new ASTNode("*");
		prod->left = new ASTNode("2");
		prod->right = sqrt;

		ASTNode* div = new ASTNode("/");
		div->left = new ASTNode("1");
		div->right = prod;

		ASTNode* exp = new ASTNode("*");
		exp->left = div;
		exp->right = Differentiate(node->left);

		return exp;
	}
	else if (node->value == "sec") // sec → sec(x) * tan(x)
	{
		ASTNode* secx = new ASTNode("sec");
		secx->left = node->left;

		ASTNode* tanx = new ASTNode("tan");
		tanx->left = node->left;

		ASTNode* prod = new ASTNode("*");
		prod->left = secx;
		prod->right = tanx;

		ASTNode* exp = new ASTNode("*");
		exp->left = prod;
		exp->right = Differentiate(node->left);

		return exp;

	}
	else if (node->value == "csc") // csc → -1 * csc(x) * cot(x)
	{
		ASTNode* cscx = new ASTNode("csc");
		cscx->left = node->left;

		ASTNode* cotx = new ASTNode("cot");
		cotx->left = node->left;

		ASTNode* prod = new ASTNode("*");
		prod->left = cscx;
		prod->right = cotx;

		ASTNode* exp = new ASTNode("*");
		exp->left = prod;
		exp->right = new ASTNode("-1");

		ASTNode* rslt = new ASTNode("*");
		rslt->left = exp;
		rslt->right = Differentiate(node->left);

		return rslt;

	}
	else if (node->value == "cot") // cot → -1 / sin^2(x)
	{
		ASTNode* csc = new ASTNode("csc");
		csc->left = node->left;

		ASTNode* pow = new ASTNode("^");
		pow->left = csc;
		pow->right = new ASTNode("2");

		ASTNode* prod = new ASTNode("*");
		prod->left = new ASTNode("-1");
		prod->right = pow;

		ASTNode* exp = new ASTNode("*");
		exp->left = prod;
		exp->right = Differentiate(node->left);

		return exp;


	}
	else if (node->left->value == "e") {
		// d/dx e^x = e^x * ln(a) where a=e, ln(e)=1
		// so just return the same node
		ASTNode* result = new ASTNode("^");
		result->left = new ASTNode("e");
		result->right = node->right;
		ASTNode* chain = new ASTNode("*");
		chain->left = result;
		chain->right = Differentiate(node->right);
		return chain;
	}

	return new ASTNode("0"); // unknown function — treat as constant
}

double TangentToCurve::Slope()
{
	if (!Limit(node, x).Existance())
		throw "Derivative is not possible. Thus, limit does not exist\n";

	double h = 0.001;

	return (Evaluator(node, x + h).Result() - Evaluator(node, x).Result()) / h; // Numeric Method

}

int TangentToCurve::precedence(const std::string& op)
{
	if (op == "+" || op == "-") return 1;
	if (op == "*" || op == "/") return 2;
	if (op == "^") return 3;
	return 100; // numbers, variables, functions
}

string TangentToCurve::toString(ASTNode* node)
{
	if (!node) return "";
	return ExprPrinter::print(node);
}