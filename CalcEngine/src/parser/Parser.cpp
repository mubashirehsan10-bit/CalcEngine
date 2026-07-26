#include "Parser.h"
#include<iostream>
#include<cctype>
using namespace std;


//------------------------------- APPLYING BODMAS RULE -------------------------------

ASTNode::ASTNode(string value)
{
	this->value = value;
	left = nullptr;
	right = nullptr;

}

Parser::Parser(vector<Token> t)
{
	token = t;
	pos = 0;
}
ASTNode* Parser::parse()
{
	if (pos >= token.size()) {
		return nullptr;
	}

	return parseExpression(); // the chain starts and completes its calling order to compplete the calculations
}

ASTNode* Parser::parseExpression() //— handles + and -(lowest precedence)
{
	ASTNode* left = parseTerm();

	if (pos >= token.size()) {
		return left;
	}

	if (token[pos].value == "+" or token[pos].value == "-")
	{
		ASTNode* node = new ASTNode(token[pos].value);
		pos++;
		ASTNode* right = parseTerm();

		node->left = left;
		node->right = right;

		return node;
	}

	else
		return left;
		
}
ASTNode* Parser::parseTerm() // — handles * and /
{
	ASTNode* left = parsePower();

	if (pos >= token.size()) {
		return left;
	}

	if (token[pos].value == "*" or token[pos].value == "/")
	{
		ASTNode* node = new ASTNode(token[pos].value);
		pos++;
		ASTNode* right = parsePower();

		node->left = left;
		node->right = right;

		return node;
	}

	else
		return left;
}
ASTNode* Parser::parsePower()
{
	ASTNode* left = parsePrimary();
	if (pos >= token.size()) {
		return left;
	}
	
	if (token[pos].value == "^" )
	{
		ASTNode* node = new ASTNode(token[pos].value);
		pos++;
		ASTNode* right = parsePrimary();

		node->left = left;
		node->right = right;

		return node;
	}

	else
		return left;

}
ASTNode* Parser::parsePrimary() // — handles numbers, variables, functions, parentheses (highest precedence)
{
	if (pos >= token.size()) {
		return nullptr;
	}

	if (token[pos].type == TokenType::NUMBER || token[pos].type == TokenType::VARIABLE)
	{
		ASTNode* node = new ASTNode(token[pos].value);
		pos++;
		return node;
	}
	else if (token[pos].type == TokenType::LPAREN)
	{
		pos++; // Left paren
		ASTNode* node = parseExpression(); // parse inner expression
		pos++; // right paren
		return node;
	}
	else if (token[pos].type == TokenType::FUNCTION)
	{
		ASTNode* node = new ASTNode(token[pos].value); // "sin", "cos", whatever
		pos++; // skip function name
		pos++; // skip (
		node->left = parseExpression(); // parse argument
		pos++; // skip )
		return node;
	}
	else
	{
		cout << "Error Occured!!!\nPlease check your expression\n";
		return nullptr;;
	}
	


}
