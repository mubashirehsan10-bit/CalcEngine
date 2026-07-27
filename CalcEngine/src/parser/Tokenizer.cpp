#include "Tokenizer.h"

Token::Token(TokenType type , string value) // constructor for the Token class
{
	this->type = type;
	this->value = value;
}


Tokenizer::Tokenizer(string input)
{
	pos = 0;
	this->input = input;
}
vector<Token> Tokenizer::tokenize()
{
	vector<Token> tokens;
	TokenType type = TokenType::NONE;

	
	while (pos < input.length())
	{
		char c = input[pos];

		string temp;

		if (isdigit(c))
		{

			while (pos < input.length() && (isdigit(input[pos]) || input[pos] == '.')) { // get the whole number (including decimal point)
				temp += input[pos];
				pos++;
			}

			tokens.push_back(Token(TokenType::NUMBER, temp)); // get the number
			continue;
		
		}
		else if(c == '+' or c == '-' or c == '*' or c == '/' or c == '^') {
			type = TokenType::OPERATOR;
			tokens.push_back(Token(type, string(1, c))); // get the operator
		}
		else if(c== '(') {
			type = TokenType::LPAREN;
			tokens.push_back(Token(type, string(1, c))); // get the parenthesis

		}
		else if (c == ')') {
			type = TokenType::RPAREN;
			tokens.push_back(Token(type, string(1, c))); // get the parenthesis
		}
		else if (c == 'x' or c == 'y' or c == 'z') { // 3 dimensions
			type = TokenType::VARIABLE;
			tokens.push_back(Token(type, string(1, c))); // get the variable name
		}
		else if (c == 's' or c == 'c' or c == 't' or c == 'l' or c == 'a') { // sin, cos, tan, log
			type = TokenType::FUNCTION;

			while(pos<input.length() && isalpha(input[pos])) { // get the whole function name
				temp += input[pos];
				pos++;
			}
			
			tokens.push_back(Token(type, temp)); // get the function name
			continue;
		}
		else {
			pos++; // skip any whitespace or unrecognized characters
			continue;
		}

		pos++;
		
	}
	return tokens;
}