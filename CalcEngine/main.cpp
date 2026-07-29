#include "src/parser/Tokenizer.h"
#include "src/parser/Parser.h"
#include "src/calculus/Evaluator.h"
#include "src/calculus/Limits.h"
#include "src/calculus/Differentiator.h"
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

        cout << "For what avalue of x do u wanna calculate: ";
        double x; cin >> x;

        TangentToCurve rslt(root, x);

        cout << "Tangent to the curve: " << equation << " is " << rslt.Slope();

    }
    catch (const char* c)
    {
        cerr << "Error!! " << c << endl;
    }




    
}