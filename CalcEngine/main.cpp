#include "src/parser/Tokenizer.h"
#include "src/parser/Parser.h"
#include<iostream>;
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

    Parser parse(arr);

    ASTNode* root = parse.parse();

    if (root == nullptr) {
        cout << "root is null" << endl;
    }
    else {
        cout << "Value:" << root->value << endl;
        if (root->left) cout << "Left: " << root->left->value << endl;
        if (root->right) cout << "Right: " << root->right->value << endl;
    }

    cout << "Equation : " << equation;

    
}