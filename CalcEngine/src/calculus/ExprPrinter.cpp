#include "ExprPrinter.h"
#include <cmath>
#include <set>
#include <sstream>
#include <iomanip>

// ───────────────────────────────────────────────────────────────────
//  Helpers
// ───────────────────────────────────────────────────────────────────
int ExprPrinter::precedence(const std::string& op)
{
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "^")              return 3;
    return 10; // functions / atoms — highest
}

bool ExprPrinter::isFunction(const std::string& val)
{
    static const std::set<std::string> fns = {
        "sin","cos","tan","sec","csc","cot",
        "asin","acos","atan",
        "sinh","cosh","tanh",
        "asinh","acosh","atanh",
        "ln","log","sqrt","exp"
    };
    return fns.count(val) > 0;
}

bool ExprPrinter::isNumber(const ASTNode* n)
{
    if (!n || n->value.empty()) return false;
    bool seenDot = false, seenDigit = false;
    for (size_t i = 0; i < n->value.size(); ++i)
    {
        char c = n->value[i];
        if (c == '-' && i == 0) continue;
        if (c == '.') { if (seenDot) return false; seenDot = true; continue; }
        if (c >= '0' && c <= '9') { seenDigit = true; continue; }
        return false;
    }
    return seenDigit;
}

// ───────────────────────────────────────────────────────────────────
//  Recursive printer
// ───────────────────────────────────────────────────────────────────
std::string ExprPrinter::printNode(const ASTNode* node,
    int parentPrec, bool isRightChild)
{
    if (!node) return "?";

    const std::string& v = node->value;

    // ── leaf: number ─────────────────────────────────────────────
    if (isNumber(node))
    {
        // format cleanly: drop trailing .000000
        double d = std::stod(v);
        if (d == std::floor(d) && std::abs(d) < 1e15)
        {
            long long i = (long long)d;
            // show negative numbers with parens when inside higher-prec context
            if (i < 0 && parentPrec > 1)
                return "(" + std::to_string(i) + ")";
            return std::to_string(i);
        }
        std::ostringstream ss;
        ss << std::setprecision(6) << d;
        return ss.str();
    }

    // ── leaf: variable or constant (e, pi, x, y, z) ─────────────
    if (!node->left && !node->right)
        return v;

    // ── function node ─────────────────────────────────────────────
    if (isFunction(v))
    {
        // argument is always in left child
        std::string arg = printNode(node->left, 0, false);
        return v + "(" + arg + ")";
    }

    // ── binary operator ───────────────────────────────────────────
    int myPrec = precedence(v);

    // special case: -1 * expr → -expr
    if (v == "*" && node->left && node->left->value == "-1")
    {
        std::string rhs = printNode(node->right, myPrec, false);
        std::string neg = "-" + rhs;
        // parenthesise if parent expects higher precedence
        if (parentPrec > myPrec || (parentPrec == myPrec && isRightChild))
            return "(" + neg + ")";
        return neg;
    }

    // special case: expr * 1 or 1 * expr — already handled by simplifier,
    // but guard here just in case
    if (v == "*" && node->right && node->right->value == "1")
        return printNode(node->left, parentPrec, isRightChild);
    if (v == "*" && node->left && node->left->value == "1")
        return printNode(node->right, parentPrec, isRightChild);

    std::string lhs = printNode(node->left, myPrec, false);
    std::string rhs = printNode(node->right, myPrec, true);

    // decide operator symbol
    std::string sym = v;

    // build expression
    std::string expr = lhs + sym + rhs;

    // parenthesise when:
    //   our precedence < parent precedence, OR
    //   same precedence but we are the right child of - or /
    //   (to preserve left-associativity)
    bool needParens = (myPrec < parentPrec);
    if (!needParens && myPrec == parentPrec && isRightChild)
        if (sym == "-" || sym == "/" || sym == "^")
            needParens = true;

    return needParens ? "(" + expr + ")" : expr;
}

// ───────────────────────────────────────────────────────────────────
//  Public entry
// ───────────────────────────────────────────────────────────────────
std::string ExprPrinter::print(const ASTNode* node)
{
    if (!node) return "";
    return printNode(node, 0, false);
}