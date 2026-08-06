#pragma once
#include "../parser/Parser.h"
#include <string>

// ═══════════════════════════════════════════════════════════════════
//  ExprPrinter
//  Converts an AST to a textbook-style string.
//  Responsibilities:
//    - Precedence-aware parenthesisation (add parens only when needed)
//    - Textbook formatting (e.g. -1*x → -x, 1*x → x)
//    - Function call notation: sin(x), cos(x^2)
//  NOT responsible for simplification — call ASTSimplifier first.
// ═══════════════════════════════════════════════════════════════════
class ExprPrinter
{
public:
    // Convert AST to string. Pass a simplified tree for clean output.
    static std::string print(const ASTNode* node);

private:
    // Returns operator precedence (higher = tighter binding)
    static int  precedence(const std::string& op);

    // True if node is a known function name
    static bool isFunction(const std::string& val);

    // True if node is a pure number literal
    static bool isNumber(const ASTNode* n);

    // Recursive worker — parenthesises based on parent precedence
    static std::string printNode(const ASTNode* node, int parentPrec,
        bool isRightChild);
};