#include "Differentiator.h"

TangentToCurve::TangentToCurve(ASTNode* ast, double x)
{
	node = ast;
	this->x = x;
}
double TangentToCurve::Slope()
{
	if (!Limit(node, x).Existance())
		throw "Derivative is not possible. Thus, limit does not exist\n";







}