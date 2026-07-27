#pragma once
#include "../parser/Parser.h"

class Evaluator {
    double x;
    ASTNode* node;
    double evaluate(ASTNode* node);
public:
    Evaluator(ASTNode* ast,double x); //construct
    double Result();
};