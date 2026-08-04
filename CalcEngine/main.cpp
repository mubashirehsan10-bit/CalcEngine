#include "src/parser/Tokenizer.h"
#include "src/parser/Parser.h"
#include "src/calculus/Evaluator.h"
#include "src/calculus/Limits.h"
#include "src/calculus/Differentiator.h"
#include "src/calculus/Integrator.h"

#include<iostream>;
using namespace std;

int main()
{
    string equation;
    cout << "Enter the Equataion: \n";
    cout << " f(x) = ";

    getline(cin, equation);


    Tokenizer t(equation);

    vector<Token> arr = t.tokenize();

    try {
        Parser parse(arr);

        ASTNode* root = parse.parse();

        TangentToCurve diff(root, 0); // x value doesn't matter for symbolic
        ASTNode* derivative = diff.Derivative();
        Evaluator eval(derivative, 1); // evaluate derivative at x=3
        cout << "Derivative at x=1: " << eval.Result();
    }
    catch (const char* c)
    {
        cerr << "\033[31m" << "Error!" << "\033[0m" << c << endl;
    }




    
}