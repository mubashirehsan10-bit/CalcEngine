#include "Limits.h"

double Limit::limit_calc(ASTNode* part, double val) // calculates left and right limit
{
	Evaluator h(part,val);
	return h.Result();
}
Limit::Limit(ASTNode* ast, double x) // stores nearest values of x to calc limit
{
	node = ast;
	this->x = x;

}
bool Limit::Existance()
{
	double h = 0.0001;
	double left_limit = limit_calc(node, x - h);
	double right_limit = limit_calc(node, x + h);
	return abs(left_limit - right_limit) < 0.01;;
}
double Limit::value()
{
	
	double h = 0.0001;
	return (limit_calc(node, x - h) + limit_calc(node, x + h)) / 2.0; // the limit is very close to x but not x
	
}
