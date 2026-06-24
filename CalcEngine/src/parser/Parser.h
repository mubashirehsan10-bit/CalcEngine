#pragma once
#include<string>
using namespace std;

class ASTNode {
public:
	string value;
	ASTNode* left;
	ASTNode* right;

	ASTNode(string value);

}

class Parser{
private:
	string input;
	int pos;
public:
	Parser(string input);
	ASTNode* parse();
	
}
