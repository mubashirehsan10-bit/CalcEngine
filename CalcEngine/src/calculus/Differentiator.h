#pragma once
#include "Limits.h"

class TangentToCurve
{
	double x;
	ASTNode* node;

	// Member functions for recursicive actions
	ASTNode* powerRule(ASTNode* node);
	ASTNode* sumRule(ASTNode* node);
	ASTNode* productRule(ASTNode* node);
	ASTNode* chainRule(ASTNode* node);
	ASTNode* Differentiate(ASTNode* node); // calls the right rule

public:
	TangentToCurve(ASTNode* ast, double x);
	string toString(ASTNode* node);

	ASTNode* Derivative();  //Gives Expression -> Next Phase  Symbolic method
	double Slope();

};