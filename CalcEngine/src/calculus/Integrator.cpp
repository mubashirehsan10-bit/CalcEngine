#include "Integrator.h"

Area::Area(ASTNode* ast, double ll, double rl, int n)
{
	node = ast;
	LowerLimit = ll;
	UpperLimit = rl;
	intervals = n;
		
}
// ASTNode* AreaExpression(); gives the area under the curve -> Next phase
double Area::AreaUnderCurve()
{
	// fromula:
	// mmation methods are usually best approached with subintervals of equal size. The interval [a, b] is therefore divided into 

	double Δx = ( UpperLimit - LowerLimit ) / intervals; //	Δx = b - a /2

	double sum = 0;

	for (int i = 0; i <= intervals - 1; i++)
	{
		double x = LowerLimit + i * Δx;
		sum += Evaluator(node, x).Result() * Δx;
	}

	return sum;

}