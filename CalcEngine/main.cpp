#include <iostream>
#include "src/parser/Tokenizer.h"
using namespace std;

int main()
{
    string equation;
    cout << "Enter the Equataion: \n";
    cout << " f = ";

    getline(cin, equation);


    Tokenizer t(equation);

    vector<Token> arr = t.tokenize();

    for (Token x : arr)
    {
        cout << x.value << "\n";
    }




    cout << "Equation : " << equation;

    
}