#pragma once
#include "Limits.h"

class TangentToCurve
{
	double x;
	ASTNode* node;
public:
	TangentToCurve(ASTNode* ast, double x);
	//ASTNode* Derivative();  Gives Expression -> Next Phase // Symbolic method
	double Slope();

};