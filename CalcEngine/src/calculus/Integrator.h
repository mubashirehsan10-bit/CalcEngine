#pragma once
#include "Evaluator.h"

class Area
{
	//================ By Reimann Sum =====================
	double LowerLimit, UpperLimit;
	int intervals;
	ASTNode* node;
public:
	Area(ASTNode* ast, double ll, double rl, int n = 10000);
	// ASTNode* AreaExpression(); gives the area under the curve -> Next phase
	double AreaUnderCurve();
};