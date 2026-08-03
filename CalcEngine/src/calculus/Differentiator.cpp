#include "Differentiator.h"
#include <stdexcept> // For exception handling


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

	if (node->value == "x")
		return new ASTNode("1");

	else if (isdigit(node->value[0]))
		return new ASTNode("0");

	else if (node->value == "^")
		expression = powerRule(node);

	else if (node->value == "+" || node->value == "-")
		expression = sumRule(node);

	else if (node->value == "*" || node->value == "/")
		expression = productRule(node);

	return expression;

}

ASTNode* TangentToCurve::powerRule(ASTNode* node) // ---> x^n <---
{
	ASTNode* expression = new ASTNode("*");
	ASTNode* power = new ASTNode("^");
	power->left = node->left;                                          // x
	power->right = new ASTNode(to_string(stod(node->right->value) - 1)); // 1
	expression->left = node->right;                                    // 2
	expression->right = power;
	return expression;

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
	left->right = node->right;
	right->left = Differentiate(node->right);
	right->right = node->left;

	expression->left = left;
	expression->right = right;

	return expression;

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

	

}


double TangentToCurve::Slope()
{
	if (!Limit(node, x).Existance())
		throw "Derivative is not possible. Thus, limit does not exist\n";

	double h = 0.001;

	return (Evaluator(node, x + h).Result() - Evaluator(node, x).Result()) / h; // Numeric Method

}