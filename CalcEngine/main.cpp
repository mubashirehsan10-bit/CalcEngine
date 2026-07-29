#include "src/parser/Tokenizer.h"
#include "src/parser/Parser.h"
#include "src/calculus/Evaluator.h"
#include "src/calculus/Limits.h"
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

        Limit rslt(root, x);

        if (rslt.Existance()) {
            cout << "The limit exists at x = " << x << endl;

            cout << "The limit at x = " << x << " is " << rslt.value() << endl;

        }
        else
            cout << "The limit doesn't exists at x = " << x << endl;




    }
    catch (const char* c)
    {
        cerr << "Error!! " << c << endl;
    }




    
}