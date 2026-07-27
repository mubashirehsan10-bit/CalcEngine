#include "src/parser/Tokenizer.h"
#include "src/parser/Parser.h"
#include "src/calculus/Evaluator.h"
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

   /* for (Token x : arr)
    {
        cout << x.value << "\n";
    }*/

    try {
        Parser parse(arr);

        ASTNode* root = parse.parse();

        cout << "For what avalue of x do u wanna calculate: ";
        double x; cin >> x;

        Evaluator rslt(root, x);

        cout << "ANSWER : " << rslt.Result();
    }
    catch (const char* c)
    {
        cerr << "Error!! " << c << endl;
    }

    
}