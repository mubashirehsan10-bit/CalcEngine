#pragma once
#include<string>
#include "Tokenizer.h"
using namespace std;

class ASTNode { // The class to handle order of calculation like trees in DSA
public:
	string value;
	ASTNode* left;
	ASTNode* right;

	ASTNode(string value);

};

class Parser { // - The class to impliment BODMAS Rule
private:
	vector<Token> token;
	bool isValidFunction(string name);
	int pos;
	ASTNode* parseExpression(); //— handles + and -(lowest precedence)
	ASTNode* parseTerm(); // — handles * and /
	ASTNode* parsePower(); // — handles ^
	ASTNode* parsePrimary(); // — handles numbers, variables, functions, parentheses(highest precedence)
public:
	Parser(vector<Token> t);
	ASTNode* parse();
};
