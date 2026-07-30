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

        cout << "Lower Limit: ";

        double x; cin >> x;

        cout << "Upper Limit:";
        double y; cin >> y;

        Area rslt(root,x,y);

        cout << "Tangent to the curve: " << equation << " is " << rslt.AreaUnderCurve();

    }
    catch (const char* c)
    {
        cerr << "Error!! " << c << endl;
    }




    
}