#include "Parser.h"
#include<iostream>
#include<cctype>
using namespace std;

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

}

ASTNode* Parser::parseExpression() //— handles + and -(lowest precedence)
{

}
ASTNode* Parser::parseTerm() // — handles * and /
{

}
ASTNode* Parser::parsePower() // — handles ^
{

}
ASTNode* Parser::parsePrimary() // — handles numbers, variables, functions, parentheses (highest precedence)
{
	if (token[pos].type == TokenType::NUMBER || token[pos].type == TokenType::VARIABLE)
	{
		ASTNode* node = new ASTNode(token[pos].value);
		pos++;
		return node;
	}
	else if (token[pos].type == TokenType::LPAREN)
	{
		pos++; // Left paren
		ASTNode* node = parseExpression();
		pos++; // right paren
		return node;
	}
	else if (token[pos].type == TokenType::FUNCTION)
	{
		if (token[pos].value == "sin") // will it work like this?
		{
			// then for cos and and tan..........
		}
	}
	else
	{
		cout << "Error Occured!!!\nPlease check your expression\n";
		return nullptr;;
	}
	


}
