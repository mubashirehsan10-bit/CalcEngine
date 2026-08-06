#pragma once
#include "../parser/Parser.h"

// ═══════════════════════════════════════════════════════════════════
//  ASTSimplifier
//  Applies algebraic identities to reduce an AST in-place.
//  Returns a NEW tree — never modifies the original.
//  Rules applied (bottom-up, repeated until stable):
//    0+x → x        x+0 → x
//    x-0 → x        0-x → (-1)*x
//    1*x → x        x*1 → x
//    0*x → 0        x*0 → 0
//    x/1 → x
//    x^1 → x        x^0 → 1
//    constant folding: 2+3 → 5, 4*5 → 20, etc.
// ═══════════════════════════════════════════════════════════════════
class ASTSimplifier
{
public:
    // Returns a simplified copy of the tree rooted at node.
    // Caller owns the returned pointer.
    static ASTNode* simplify(const ASTNode* node);

private:
    static ASTNode* copyNode(const ASTNode* node);
    static ASTNode* simplifyOnce(ASTNode* node);
    static ASTNode* applyRules(ASTNode* node);

    // helpers
    static bool   isNumber(const ASTNode* n);
    static bool   isValue(const ASTNode* n, double v);
    static double toDouble(const ASTNode* n);
    static ASTNode* makeNum(double v);
};