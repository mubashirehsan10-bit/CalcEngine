#include "Simplifier.h"
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <iomanip>

// ───────────────────────────────────────────────────────────────────
//  Internal helpers
// ───────────────────────────────────────────────────────────────────

bool ASTSimplifier::isNumber(const ASTNode* n)
{
    if (!n || n->value.empty()) return false;
    // accept digits, leading '-', decimal point
    bool seenDot = false;
    bool seenDigit = false;
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

bool ASTSimplifier::isValue(const ASTNode* n, double v)
{
    if (!isNumber(n)) return false;
    return std::abs(std::stod(n->value) - v) < 1e-12;
}

double ASTSimplifier::toDouble(const ASTNode* n)
{
    return std::stod(n->value);
}

ASTNode* ASTSimplifier::makeNum(double v)
{
    std::ostringstream ss;
    // print as integer when possible
    if (v == std::floor(v) && std::abs(v) < 1e15)
        ss << (long long)v;
    else
        ss << std::setprecision(10) << v;
    return new ASTNode(ss.str());
}

// ───────────────────────────────────────────────────────────────────
//  Deep copy
// ───────────────────────────────────────────────────────────────────
ASTNode* ASTSimplifier::copyNode(const ASTNode* node)
{
    if (!node) return nullptr;
    ASTNode* copy = new ASTNode(node->value);
    copy->left = copyNode(node->left);
    copy->right = copyNode(node->right);
    return copy;
}

// ───────────────────────────────────────────────────────────────────
//  Apply one pass of rules (children must already be simplified)
// ───────────────────────────────────────────────────────────────────
ASTNode* ASTSimplifier::applyRules(ASTNode* node)
{
    if (!node) return nullptr;

    ASTNode* L = node->left;
    ASTNode* R = node->right;
    const std::string& op = node->value;

    // ── constant folding ─────────────────────────────────────────
    if (L && R && isNumber(L) && isNumber(R))
    {
        double l = toDouble(L), r = toDouble(R);
        double result = 0;
        bool   fold = true;

        if (op == "+") result = l + r;
        else if (op == "-") result = l - r;
        else if (op == "*") result = l * r;
        else if (op == "/" && r != 0.0) result = l / r;
        else if (op == "^") result = std::pow(l, r);
        else fold = false;

        if (fold)
        {
            ASTNode* n = makeNum(result);
            delete node->left;
            delete node->right;
            node->left = node->right = nullptr;
            delete node;
            return n;
        }
    }

    // ── addition rules ───────────────────────────────────────────
    if (op == "+")
    {
        // 0 + x → x
        if (L && isValue(L, 0.0))
        {
            ASTNode* keep = R; node->right = nullptr;
            delete node->left; node->left = nullptr; delete node;
            return keep;
        }
        // x + 0 → x
        if (R && isValue(R, 0.0))
        {
            ASTNode* keep = L; node->left = nullptr;
            delete node->right; node->right = nullptr; delete node;
            return keep;
        }
    }

    // ── subtraction rules ─────────────────────────────────────────
    if (op == "-")
    {
        // x - 0 → x
        if (R && isValue(R, 0.0))
        {
            ASTNode* keep = L; node->left = nullptr;
            delete node->right; node->right = nullptr; delete node;
            return keep;
        }
        // 0 - x → -1 * x
        if (L && isValue(L, 0.0))
        {
            ASTNode* neg = makeNum(-1.0);
            ASTNode* mul = new ASTNode("*");
            mul->left = neg;
            mul->right = R;
            node->right = nullptr;
            delete node->left; node->left = nullptr; delete node;
            return mul;
        }
    }

    // ── multiplication rules ──────────────────────────────────────
    if (op == "*")
    {
        // 0 * x → 0  |  x * 0 → 0
        if ((L && isValue(L, 0.0)) || (R && isValue(R, 0.0)))
        {
            ASTNode* zero = makeNum(0.0);
            delete node->left;  node->left = nullptr;
            delete node->right; node->right = nullptr;
            delete node;
            return zero;
        }
        // 1 * x → x
        if (L && isValue(L, 1.0))
        {
            ASTNode* keep = R; node->right = nullptr;
            delete node->left; node->left = nullptr; delete node;
            return keep;
        }
        // x * 1 → x
        if (R && isValue(R, 1.0))
        {
            ASTNode* keep = L; node->left = nullptr;
            delete node->right; node->right = nullptr; delete node;
            return keep;
        }
        // -1 * (-1 * x) → x
        if (L && isValue(L, -1.0) && R && R->value == "*"
            && R->left && isValue(R->left, -1.0))
        {
            ASTNode* keep = R->right; R->right = nullptr;
            delete node->left; node->left = nullptr;
            delete node->right; node->right = nullptr;
            delete node;
            return keep;
        }
    }

    // ── division rules ────────────────────────────────────────────
    if (op == "/")
    {
        // x / 1 → x
        if (R && isValue(R, 1.0))
        {
            ASTNode* keep = L; node->left = nullptr;
            delete node->right; node->right = nullptr; delete node;
            return keep;
        }
        // 0 / x → 0
        if (L && isValue(L, 0.0))
        {
            ASTNode* zero = makeNum(0.0);
            delete node->left;  node->left = nullptr;
            delete node->right; node->right = nullptr;
            delete node;
            return zero;
        }
    }

    // ── power rules ───────────────────────────────────────────────
    if (op == "^")
    {
        // x ^ 1 → x
        if (R && isValue(R, 1.0))
        {
            ASTNode* keep = L; node->left = nullptr;
            delete node->right; node->right = nullptr; delete node;
            return keep;
        }
        // x ^ 0 → 1
        if (R && isValue(R, 0.0))
        {
            ASTNode* one = makeNum(1.0);
            delete node->left;  node->left = nullptr;
            delete node->right; node->right = nullptr;
            delete node;
            return one;
        }
        // 1 ^ x → 1
        if (L && isValue(L, 1.0))
        {
            ASTNode* one = makeNum(1.0);
            delete node->left;  node->left = nullptr;
            delete node->right; node->right = nullptr;
            delete node;
            return one;
        }
    }

    return node;
}

// ───────────────────────────────────────────────────────────────────
//  Recursive simplification (post-order: children first)
// ───────────────────────────────────────────────────────────────────
ASTNode* ASTSimplifier::simplifyOnce(ASTNode* node)
{
    if (!node) return nullptr;

    // simplify children first
    node->left = simplifyOnce(node->left);
    node->right = simplifyOnce(node->right);

    // then apply rules at this node
    return applyRules(node);
}

// ───────────────────────────────────────────────────────────────────
//  Public entry point — copy then simplify until stable
// ───────────────────────────────────────────────────────────────────
ASTNode* ASTSimplifier::simplify(const ASTNode* node)
{
    if (!node) return nullptr;

    // work on a copy so the original is never touched
    ASTNode* current = copyNode(node);

    // iterate until no further changes (max 8 passes to avoid infinite loops)
    for (int pass = 0; pass < 8; ++pass)
    {
        ASTNode* simplified = simplifyOnce(current);
        // if the root pointer changed or value changed, loop again
        if (simplified != current)
        {
            current = simplified;
            continue;
        }
        break;
    }

    return current;
}