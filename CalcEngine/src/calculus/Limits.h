#pragma once
#include<vector>
#include "Evaluator.h"
using namespace std;

class Limit
{
	double x;
	ASTNode* node; 
	double limit_calc(ASTNode* part, double val); // calculates left and right limit
public:
	Limit(ASTNode* ast, double x); // stores nearest values of x to calc limit
	bool Existance(); //uses evlauator for calculation on diff values of x
	double value();
};