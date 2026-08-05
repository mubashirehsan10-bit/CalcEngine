# CalcEngine ⚙️
> A symbolic and numerical Calculus Engine built from scratch in C++ — no libraries, no shortcuts.

---

## What is CalcEngine?

CalcEngine is a console-based C++ application that parses, evaluates, and differentiates mathematical expressions. It was built entirely from scratch — including a custom tokenizer, recursive descent parser, and Abstract Syntax Tree (AST) — as a personal project to deeply understand both calculus and compiler theory.

No math libraries were used for the core engine. No AI autocomplete. Just C++, OOP, and first principles.

---

## Features

### ✅ Phase 1 — Single Variable Calculus (Complete)

| Feature | Description |
|---|---|
| **Expression Parser** | Recursive descent parser with full BODMAS support |
| **Tokenizer** | Handles numbers, variables, operators, and 20+ functions dynamically |
| **AST** | Full Abstract Syntax Tree representation of expressions |
| **Evaluator** | Numerical evaluation of any expression at a given x |
| **Limits** | Numerical left/right limit approximation with existence check |
| **Numerical Differentiation** | Derivative via limit definition `[f(x+h) - f(x)] / h` |
| **Symbolic Differentiation** | Exact derivative via AST transformation — power, sum, product, chain rules |
| **Integration** | Riemann sum based numerical integration over `[a, b]` |

### 🔜 Phase 2 — Multivariable Calculus (Planned)
- Partial Derivatives
- Gradient Vector
- Green's Theorem
- Stokes' Theorem
- Divergence Theorem

---

## Supported Functions

| Category | Functions |
|---|---|
| Trigonometric | `sin`, `cos`, `tan`, `sec`, `csc`, `cot` |
| Inverse Trig | `asin`, `acos`, `atan` |
| Hyperbolic | `sinh`, `cosh`, `tanh` |
| Inverse Hyperbolic | `asinh`, `acosh`, `atanh` |
| Logarithmic | `ln`, `log` |
| Misc | `sqrt`, `e^x` |

---

## Architecture

```
CalcEngine/
├── src/
│   ├── parser/
│   │   ├── Tokenizer.h/.cpp     ← Lexical analysis
│   │   └── Parser.h/.cpp        ← Recursive descent parser + AST
│   ├── calculus/
│   │   ├── Evaluator.h/.cpp     ← Numerical evaluation
│   │   ├── Limits.h/.cpp        ← Limit computation
│   │   ├── Differentiator.h/.cpp← Symbolic + numerical differentiation
│   │   └── Integrator.h/.cpp    ← Riemann sum integration
│   └── ui/
│       └── Menu.h/.cpp          ← Console UI
├── include/
├── tests/
└── main.cpp
```

---

## How It Works

### 1. Tokenizer
Breaks a raw string like `"3*x^2 + sin(x)"` into typed tokens — `NUMBER`, `VARIABLE`, `OPERATOR`, `FUNCTION`, `LPAREN`, `RPAREN` — using a dynamic while-loop for multi-character function names.

### 2. Parser
Implements recursive descent parsing with explicit precedence levels:
- `parseExpression()` → `+`, `-`
- `parseTerm()` → `*`, `/`
- `parsePower()` → `^`
- `parsePrimary()` → numbers, variables, functions, parentheses

Produces an AST where operator precedence is encoded in tree depth.

### 3. Symbolic Differentiator
Walks the AST and applies calculus rules to produce a **new expression tree**:
- Power Rule: `d/dx xⁿ = n·xⁿ⁻¹`
- Sum Rule: `d/dx (f+g) = f' + g'`
- Product Rule: `d/dx (fg) = f'g + fg'`
- Chain Rule: `d/dx f(g(x)) = f'(g(x)) · g'(x)`

---

## Sample Output

```
f(x) = x^3
d/dx x^3 = (3*(x^2))
Derivative at x=2: 12

f(x) = sin(x^2)
d/dx sin(x^2) = (cos(x^2) * 2*x)
Derivative at x=1: 1.0806

∫ x^2 dx from 0 to 3 ≈ 9.0000
lim(x→1) (x^2-1)/(x-1) = 2.0000
```

---

## Tech Stack

- **Language:** C++17
- **IDE:** Visual Studio Community 2022
- **Libraries:** `<cmath>`, `<string>`, `<vector>` — standard only
- **Paradigm:** OOP — classes, encapsulation, recursion, dynamic memory
- **Paradigm:** DSA — Trees, Recursion, Linked-Lists 


---

## Built By

**Mubashir** — 1st Year CS Student, FAST NUCES Lahore (Batch 2025)

Built during summer break as a self-driven deep-dive into calculus and compiler theory. GitHub Copilot was intentionally kept off throughout development.

---

## What I Learned

- Recursive descent parsing and AST construction
- Tree-based symbolic computation
- Numerical methods for limits, derivatives, and integrals
- How calculus rules translate directly into recursive algorithms
- OOP & DSA design patterns in a real, non-trivial C++ project
