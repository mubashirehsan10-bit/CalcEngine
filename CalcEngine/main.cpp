#include "src/parser/Tokenizer.h"
#include "src/parser/Parser.h"
#include "src/calculus/Evaluator.h"
#include "src/calculus/Limits.h"
#include "src/calculus/Differentiator.h"
#include "src/calculus/Integrator.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

// ═══════════════════════════════════════════════════════════════
//  PALETTE
// ═══════════════════════════════════════════════════════════════
static const sf::Color BG_DARK{ 14,  16,  24 };
static const sf::Color BG_PANEL{ 22,  26,  40 };
static const sf::Color BG_INPUT{ 28,  33,  50 };
static const sf::Color BG_RESULT{ 20,  25,  38 };
static const sf::Color ACCENT_BLUE{ 90,  190, 255 };
static const sf::Color ACCENT_GOLD{ 255, 210, 90 };
static const sf::Color ACCENT_GRN{ 90,  255, 180 };
static const sf::Color ACCENT_PINK{ 255, 120, 190 };
static const sf::Color BTN_IDLE{ 35,  42,  65 };
static const sf::Color BTN_HOVER{ 55,  68,  110 };
static const sf::Color TEXT_HI{ 240, 245, 255 };
static const sf::Color TEXT_DIM{ 130, 140, 170 };
static const sf::Color TEXT_ERR{ 255, 100, 100 };
static const sf::Color DIVIDER{ 42,  50,  80 };
static const sf::Color GRID_COL{ 35,  41,  65 };
static const sf::Color AXIS_COL{ 70,  82,  120 };

// ═══════════════════════════════════════════════════════════════
//  UTILITIES
// ═══════════════════════════════════════════════════════════════
static std::string fmt(double v, int p = 4)
{
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(p) << v;
    return ss.str();
}

static double safeStod(const std::string& s, double def = 0.0)
{
    if (s.empty()) return def;
    try { return std::stod(s); }
    catch (...) { return def; }
}

static void drawRect(sf::RenderWindow& w, sf::FloatRect r,
    sf::Color fill, sf::Color outline = { 0,0,0,0 }, float ot = 0, float shadowOffset = 0)
{
    if (shadowOffset > 0)
    {
        sf::RectangleShape sh(r.size);
        sh.setPosition({ r.position.x + shadowOffset, r.position.y + shadowOffset });
        sh.setFillColor(sf::Color(0, 0, 0, 70));
        w.draw(sh);
    }

    sf::RectangleShape s(r.size);
    s.setPosition(r.position);
    s.setFillColor(fill);
    if (ot > 0) { s.setOutlineThickness(ot); s.setOutlineColor(outline); }
    w.draw(s);
}

static void drawText(sf::RenderWindow& w, const sf::Font& f,
    const std::string& str, sf::Vector2f pos,
    sf::Color col, unsigned sz, bool bold = false)
{
    sf::Text t(f, str, sz);
    t.setFillColor(col);
    t.setPosition(pos);
    if (bold) t.setStyle(sf::Text::Bold);
    w.draw(t);
}

// ═══════════════════════════════════════════════════════════════
//  BUTTON
// ═══════════════════════════════════════════════════════════════
struct Button
{
    sf::RectangleShape bg;
    sf::Text           lbl;
    sf::Color          accentCol;
    bool               hovered = false;
    bool               active = false;

    Button() = default;
    Button(const sf::Font& font, const std::string& text,
        sf::Vector2f pos, sf::Vector2f size, sf::Color accent)
        : lbl(font, text, 14), accentCol(accent)
    {
        bg.setPosition(pos);
        bg.setSize(size);
        bg.setFillColor(BTN_IDLE);
        bg.setOutlineThickness(1.5f);
        bg.setOutlineColor(DIVIDER);

        auto lb = lbl.getLocalBounds();
        lbl.setOrigin({ lb.position.x + lb.size.x / 2.f,
                       lb.position.y + lb.size.y / 2.f });
        lbl.setPosition({ pos.x + size.x / 2.f, pos.y + size.y / 2.f });
        lbl.setFillColor(TEXT_HI);
    }

    bool contains(sf::Vector2i p) const
    {
        return bg.getGlobalBounds().contains(sf::Vector2f(p));
    }

    void update(sf::Vector2i mouse)
    {
        hovered = contains(mouse);
        bg.setFillColor(active ? accentCol : hovered ? BTN_HOVER : BTN_IDLE);
        bg.setOutlineColor(active ? accentCol : hovered ? accentCol : DIVIDER);
        lbl.setFillColor(active ? BG_DARK : TEXT_HI);
    }

    void draw(sf::RenderWindow& w) 
    { 
        if (!active && !hovered) {
             sf::RectangleShape sh = bg;
             sh.setPosition({bg.getPosition().x + 2.f, bg.getPosition().y + 3.f});
             sh.setFillColor(sf::Color(0,0,0,90));
             sh.setOutlineThickness(0);
             w.draw(sh);
        } else if (hovered && !active) {
             sf::RectangleShape sh = bg;
             sh.setPosition({bg.getPosition().x + 1.f, bg.getPosition().y + 2.f});
             sh.setFillColor(sf::Color(0,0,0,70));
             sh.setOutlineThickness(0);
             w.draw(sh);
        }
        w.draw(bg); 
        w.draw(lbl); 
    }
};

// ═══════════════════════════════════════════════════════════════
//  INPUT BOX
// ═══════════════════════════════════════════════════════════════
struct InputBox
{
    sf::RectangleShape bg;
    sf::Text           disp, ph;
    std::string        value;
    bool               focused = false;
    bool               visible = true;

    InputBox() = default;
    InputBox(const sf::Font& font, const std::string& placeholder,
        sf::Vector2f pos, sf::Vector2f size, unsigned sz = 16)
        : disp(font, "", sz), ph(font, placeholder, sz)
    {
        bg.setPosition(pos);
        bg.setSize(size);
        bg.setFillColor(BG_INPUT);
        bg.setOutlineThickness(1.5f);
        bg.setOutlineColor(DIVIDER);

        float ty = pos.y + size.y / 2.f - sz / 2.f - 1.f;
        disp.setFillColor(TEXT_HI);
        disp.setPosition({ pos.x + 12.f, ty });
        ph.setFillColor(TEXT_DIM);
        ph.setPosition({ pos.x + 12.f, ty });
    }

    bool contains(sf::Vector2i p) const
    {
        return bg.getGlobalBounds().contains(sf::Vector2f(p));
    }

    void handleText(uint32_t u)
    {
        if (!focused || !visible) return;
        if (u == 8 && !value.empty()) value.pop_back();
        else if (u >= 32 && u < 128)  value += (char)u;
        disp.setString(value + "|");
    }

    void updateFocus(sf::Vector2i mouse, bool clicked)
    {
        if (!visible) { focused = false; return; }
        if (clicked) focused = contains(mouse);
        bg.setOutlineColor(focused ? ACCENT_BLUE : DIVIDER);
        disp.setString(value + (focused ? "|" : ""));
    }

    void draw(sf::RenderWindow& w)
    {
        if (!visible) return;

        sf::RectangleShape sh = bg;
        sh.setPosition({bg.getPosition().x + 2.f, bg.getPosition().y + 3.f});
        sh.setFillColor(sf::Color(0,0,0,60));
        sh.setOutlineThickness(0);
        w.draw(sh);

        w.draw(bg);
        (value.empty() && !focused) ? w.draw(ph) : w.draw(disp);
    }
};

// ═══════════════════════════════════════════════════════════════
//  GRAPH
// ═══════════════════════════════════════════════════════════════
void drawGraph(sf::RenderWindow& win, const sf::Font& font,
    ASTNode* fx, ASTNode* dx,
    sf::FloatRect area, double xMin, double xMax, bool showD)
{
    drawRect(win, area, BG_PANEL, DIVIDER, 1.f, 3.f);
    if (!fx) {
        drawText(win, font, "Enter an equation and press an operation button.",
            { area.position.x + 20.f, area.position.y + area.size.y / 2.f - 10.f },
            TEXT_DIM, 14);
        return;
    }

    int    N = 700;
    double step = (xMax - xMin) / N;

    // auto-scale y
    double yMn = 1e18, yMx = -1e18;
    for (int i = 0; i <= N; ++i)
    {
        double x = xMin + i * step;
        auto sample = [&](ASTNode* n) {
            if (!n) return;
            try {
                double y = Evaluator(n, x).Result();
                if (std::isfinite(y)) { yMn = std::min(yMn, y); yMx = std::max(yMx, y); }
            }
            catch (...) {}
            };
        sample(fx);
        if (showD) sample(dx);
    }
    if (!std::isfinite(yMn) || !std::isfinite(yMx)) { yMn = -10; yMx = 10; }
    if (yMn == yMx) { yMn -= 1; yMx += 1; }
    double pad = (yMx - yMn) * 0.12;
    yMn -= pad; yMx += pad;

    auto mpX = [&](double x) -> float {
        return area.position.x + (float)((x - xMin) / (xMax - xMin)) * area.size.x;
        };
    auto mpY = [&](double y) -> float {
        return area.position.y + area.size.y - (float)((y - yMn) / (yMx - yMn)) * area.size.y;
        };

    // grid
    for (int i = 0; i <= 8; ++i)
    {
        float gx = area.position.x + (float)i / 8.f * area.size.x;
        float gy = area.position.y + (float)i / 8.f * area.size.y;
        sf::Vertex lx[] = { {{gx,area.position.y},GRID_COL},{{gx,area.position.y + area.size.y},GRID_COL} };
        sf::Vertex ly[] = { {{area.position.x,gy},GRID_COL},{{area.position.x + area.size.x,gy},GRID_COL} };
        win.draw(lx, 2, sf::PrimitiveType::Lines);
        win.draw(ly, 2, sf::PrimitiveType::Lines);
    }

    // axes
    if (xMin < 0 && xMax>0) {
        float ax = mpX(0);
        sf::Vertex v[] = { {{ax,area.position.y},AXIS_COL},{{ax,area.position.y + area.size.y},AXIS_COL} };
        win.draw(v, 2, sf::PrimitiveType::Lines);
    }
    if (yMn < 0 && yMx>0) {
        float ay = mpY(0);
        sf::Vertex v[] = { {{area.position.x,ay},AXIS_COL},{{area.position.x + area.size.x,ay},AXIS_COL} };
        win.draw(v, 2, sf::PrimitiveType::Lines);
    }

    // axis tick labels
    for (int i = 0; i <= 4; ++i)
    {
        double xv = xMin + (xMax - xMin) * i / 4.0;
        double yv = yMn + (yMx - yMn) * i / 4.0;
        drawText(win, font, fmt(xv, 1), { mpX(xv) - 14.f,area.position.y + area.size.y + 4.f }, TEXT_DIM, 11);
        drawText(win, font, fmt(yv, 1), { area.position.x - 44.f,mpY(yv) - 8.f }, TEXT_DIM, 11);
    }

    // plot helper
    auto plotCurve = [&](ASTNode* node, sf::Color col)
        {
            std::vector<sf::Vertex> verts;
            bool prev = false;
            for (int i = 0; i <= N; ++i)
            {
                double x = xMin + i * step;
                try {
                    double y = Evaluator(node, x).Result();
                    if (std::isfinite(y)) {
                        float px = mpX(x), py = mpY(y);
                        // clip to area
                        if (py < area.position.y - 2 || py > area.position.y + area.size.y + 2)
                        {
                            prev = false; continue;
                        }
                        if (prev) verts.push_back({ {px,py},col });
                        verts.push_back({ {px,py},col });
                        prev = true;
                    }
                    else prev = false;
                }
                catch (...) { prev = false; }
            }
            if (verts.size() > 1)
                win.draw(verts.data(), verts.size(), sf::PrimitiveType::LineStrip);
        };

    plotCurve(fx, ACCENT_BLUE);
    if (showD && dx) plotCurve(dx, ACCENT_GOLD);

    // legend
    float lx2 = area.position.x + 10.f, ly2 = area.position.y + 10.f;
    drawRect(win, { {lx2,ly2},{80.f,showD && dx ? 44.f : 22.f} }, sf::Color(0, 0, 0, 120));
    sf::RectangleShape l1({ 12.f,3.f }); l1.setFillColor(ACCENT_BLUE);
    l1.setPosition({ lx2 + 4.f,ly2 + 8.f }); win.draw(l1);
    drawText(win, font, "f(x)", { lx2 + 20.f,ly2 + 1.f }, ACCENT_BLUE, 12);
    if (showD && dx) {
        sf::RectangleShape l2({ 12.f,3.f }); l2.setFillColor(ACCENT_GOLD);
        l2.setPosition({ lx2 + 4.f,ly2 + 26.f }); win.draw(l2);
        drawText(win, font, "f'(x)", { lx2 + 20.f,ly2 + 20.f }, ACCENT_GOLD, 12);
    }
}

// ═══════════════════════════════════════════════════════════════
//  MAIN
// ═══════════════════════════════════════════════════════════════
int main()
{
    const unsigned W = 1300, H = 780;
    sf::RenderWindow window(sf::VideoMode({ W,H }), "CalcEngine", sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Font not found\n"; return -1;
    }

    // ── Layout ────────────────────────────────────────────────
    const float PAD = 22.f, LW = 400.f;
    const float GX = LW + PAD * 2.f, GY = 72.f;
    const float GW = W - GX - PAD, GH = H - GY - PAD;

    // ── Modes ─────────────────────────────────────────────────
    enum class Mode { NONE, EVAL, DERIV, LIMIT, INTEG };
    enum class Step { SELECT_OP, ENTER_PARAMS, SHOW_RESULT };
    Mode mode = Mode::NONE;
    Step step = Step::SELECT_OP;

    ASTNode* rootNode = nullptr;
    ASTNode* derivNode = nullptr;
    bool     showDeriv = false;

    std::string res1, res2, res3, errText;
    double xRangeMin = -5.0, xRangeMax = 5.0;

    // ── Widgets ───────────────────────────────────────────────
    float cy = 78.f;

    // Equation box — always visible
    InputBox eqBox(font, "Enter equation:  x^2 + sin(x)", { PAD,cy }, { LW - PAD,48.f }, 18);
    cy += 64.f;

    // Op buttons
    float bw = (LW - PAD - 30.f) / 4.f;
    Button btnEval(font, "Evaluate", { PAD,           cy }, { bw,40.f }, ACCENT_GRN);
    Button btnDeriv(font, "Derive", { PAD + bw + 10,     cy }, { bw,40.f }, ACCENT_BLUE);
    Button btnLimit(font, "Limit", { PAD + (bw + 10) * 2, cy }, { bw,40.f }, ACCENT_GOLD);
    Button btnInteg(font, "Integrate", { PAD + (bw + 10) * 3,cy }, { bw,40.f }, ACCENT_PINK);
    cy += 56.f;

    // Param inputs — shown conditionally
    InputBox xBox(font, "x value", { PAD,cy }, { (LW - PAD - 10) / 2.f,40.f }, 15);
    InputBox aBox(font, "a  (lower limit)", { PAD,cy }, { (LW - PAD - 10) / 2.f,40.f }, 15);
    InputBox bBox(font, "b  (upper limit)", { PAD + (LW - PAD - 10) / 2.f + 10,cy }, { (LW - PAD - 10) / 2.f,40.f }, 15);
    cy += 56.f;

    // Calculate button
    Button btnCalc(font, "CALCULATE", { PAD,cy }, { LW - PAD,42.f }, ACCENT_GRN);
    cy += 58.f;

    // Show derivative toggle
    Button btnToggleDeriv(font, "Show f'(x) on graph", { PAD,cy }, { LW - PAD,36.f }, ACCENT_BLUE);
    cy += 52.f;

    // x-range inputs
    InputBox xMinBox(font, "x-min  (default -5)", { PAD,cy }, { (LW - PAD - 10) / 2.f,36.f }, 14);
    InputBox xMaxBox(font, "x-max  (default  5)", { PAD + (LW - PAD - 10) / 2.f + 10,cy }, { (LW - PAD - 10) / 2.f,36.f }, 14);

    // ── Main loop ─────────────────────────────────────────────
    while (window.isOpen())
    {
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        bool clicked = false;

        // Visibility logic
        bool needX = (mode == Mode::EVAL || mode == Mode::DERIV || mode == Mode::LIMIT);
        bool needAB = (mode == Mode::INTEG);
        xBox.visible = needX && step >= Step::ENTER_PARAMS;
        aBox.visible = needAB && step >= Step::ENTER_PARAMS;
        bBox.visible = needAB && step >= Step::ENTER_PARAMS;
        btnCalc.bg.setFillColor(step == Step::ENTER_PARAMS ? ACCENT_GRN : BTN_IDLE);

        while (const std::optional ev = window.pollEvent())
        {
            if (ev->is<sf::Event::Closed>()) window.close();

            if (const auto* k = ev->getIf<sf::Event::KeyPressed>())
                if (k->code == sf::Keyboard::Key::Escape) window.close();

            if (const auto* t = ev->getIf<sf::Event::TextEntered>())
            {
                eqBox.handleText(t->unicode);
                if (xBox.visible)  xBox.handleText(t->unicode);
                if (aBox.visible)  aBox.handleText(t->unicode);
                if (bBox.visible)  bBox.handleText(t->unicode);
                xMinBox.handleText(t->unicode);
                xMaxBox.handleText(t->unicode);
            }

            if (const auto* mb = ev->getIf<sf::Event::MouseButtonPressed>())
                if (mb->button == sf::Mouse::Button::Left) clicked = true;
        }

        // Update focus
        eqBox.updateFocus(mouse, clicked);
        xBox.updateFocus(mouse, clicked);
        aBox.updateFocus(mouse, clicked);
        bBox.updateFocus(mouse, clicked);
        xMinBox.updateFocus(mouse, clicked);
        xMaxBox.updateFocus(mouse, clicked);

        // Update buttons
        btnEval.update(mouse);  btnDeriv.update(mouse);
        btnLimit.update(mouse); btnInteg.update(mouse);
        btnCalc.update(mouse);  btnToggleDeriv.update(mouse);

        btnEval.active = (mode == Mode::EVAL);
        btnDeriv.active = (mode == Mode::DERIV);
        btnLimit.active = (mode == Mode::LIMIT);
        btnInteg.active = (mode == Mode::INTEG);
        btnToggleDeriv.active = showDeriv;

        // x-range update
        if (!xMinBox.value.empty()) xRangeMin = safeStod(xMinBox.value, -5.0);
        if (!xMaxBox.value.empty()) xRangeMax = safeStod(xMaxBox.value, 5.0);

        // ── Click handling ────────────────────────────────────
        if (clicked)
        {
            // Op button → move to ENTER_PARAMS
            if (btnEval.contains(mouse) && mode != Mode::EVAL)
            {
                mode = Mode::EVAL;  step = Step::ENTER_PARAMS; res1 = res2 = res3 = errText = "";
            }
            else if (btnDeriv.contains(mouse) && mode != Mode::DERIV)
            {
                mode = Mode::DERIV; step = Step::ENTER_PARAMS; res1 = res2 = res3 = errText = "";
            }
            else if (btnLimit.contains(mouse) && mode != Mode::LIMIT)
            {
                mode = Mode::LIMIT; step = Step::ENTER_PARAMS; res1 = res2 = res3 = errText = "";
            }
            else if (btnInteg.contains(mouse) && mode != Mode::INTEG)
            {
                mode = Mode::INTEG; step = Step::ENTER_PARAMS; res1 = res2 = res3 = errText = "";
            }

            // Toggle derivative on graph
            if (btnToggleDeriv.contains(mouse))
            {
                showDeriv = !showDeriv;
                if (showDeriv && !derivNode && rootNode)
                {
                    try {
                        double xv = safeStod(xBox.value, 1.0);
                        TangentToCurve tc(rootNode, xv);
                        derivNode = tc.Derivative();
                    }
                    catch (...) {}
                }
            }

            // Calculate
            if (btnCalc.contains(mouse) && step == Step::ENTER_PARAMS)
            {
                if (eqBox.value.empty()) { errText = "Enter an equation first."; }
                else
                {
                    errText = ""; res1 = res2 = res3 = "";
                    rootNode = nullptr; derivNode = nullptr;
                    try
                    {
                        Tokenizer tk(eqBox.value);
                        auto arr = tk.tokenize();
                        Parser p(arr);
                        rootNode = p.parse();

                        if (!rootNode) throw std::runtime_error("Failed to parse expression.");

                        double xv = safeStod(xBox.value, 1.0);

                        if (mode == Mode::EVAL)
                        {
                            double r = Evaluator(rootNode, xv).Result();
                            res1 = "f(" + fmt(xv, 2) + ")";
                            res2 = fmt(r, 6);
                        }
                        else if (mode == Mode::DERIV)
                        {
                            TangentToCurve tc(rootNode, xv);
                            derivNode = tc.Derivative();
                            if (!derivNode) throw std::runtime_error("Derivative failed.");
                            res1 = "f'(x)  =  " + tc.toString(derivNode);
                            res2 = "f'(" + fmt(xv, 2) + ")  =  " + fmt(Evaluator(derivNode, xv).Result(), 6);
                            try { res3 = "Numerical slope  =  " + fmt(tc.Slope(), 6); }
                            catch (...) { res3 = "Slope undefined at this point."; }
                        }
                        else if (mode == Mode::LIMIT)
                        {
                            Limit lim(rootNode, xv);
                            bool ex = lim.Existance();
                            res1 = ex ? "Limit EXISTS" : "Limit does NOT exist";
                            if (ex) res2 = "lim(x→" + fmt(xv, 2) + ") f(x)  =  " + fmt(lim.value(), 6);
                        }
                        else if (mode == Mode::INTEG)
                        {
                            double a = safeStod(aBox.value, 0.0);
                            double b = safeStod(bBox.value, 1.0);
                            Area ar(rootNode, a, b);
                            double r = ar.AreaUnderCurve();
                            res1 = "∫ f(x) dx";
                            res2 = "from " + fmt(a, 2) + " to " + fmt(b, 2);
                            res3 = "≈  " + fmt(r, 6);
                        }

                        step = Step::SHOW_RESULT;
                    }
                    catch (const std::string& s) { errText = s; }
                    catch (const char* s) { errText = s; }
                    catch (const std::exception& e) { errText = e.what(); }
                    catch (...) { errText = "An unknown error occurred."; }
                }
            }
        }

        // ── DRAW ──────────────────────────────────────────────
        window.clear(BG_DARK);

        // Header
        drawRect(window, { {0,0},{(float)W,62.f} }, BG_PANEL, DIVIDER, 0);
        sf::RectangleShape hline({ (float)W,1.f });
        hline.setPosition({ 0,62.f }); hline.setFillColor(DIVIDER); window.draw(hline);
        drawText(window, font, "CALCENGINE", { PAD,14.f }, ACCENT_BLUE, 28, true);
        drawText(window, font, "Symbolic & Numerical Calculus Engine  ·  C++",
            { PAD + 190.f,22.f }, TEXT_DIM, 13);

        // Vertical divider
        drawRect(window, { {LW + PAD,62.f},{1.f,(float)H - 62.f} }, DIVIDER);

        // ── Left panel ────────────────────────────────────────

        // Step indicator
        std::string stepHint;
        if (mode == Mode::NONE)                stepHint = "① Select an operation below";
        else if (step == Step::ENTER_PARAMS && needX)  stepHint = "② Enter x value, then Calculate";
        else if (step == Step::ENTER_PARAMS && needAB) stepHint = "② Enter a and b limits, then Calculate";
        else if (step == Step::SHOW_RESULT)          stepHint = "✓ Done — edit equation or choose another operation";

        drawText(window, font, stepHint, { PAD,64.f }, ACCENT_GOLD, 12);

        // Equation label + box
        drawText(window, font, "EQUATION", { PAD,74.f }, TEXT_DIM, 11);
        eqBox.draw(window);

        // Op buttons label
        drawText(window, font, "OPERATION", { PAD,148.f }, TEXT_DIM, 11);
        btnEval.draw(window);
        btnDeriv.draw(window);
        btnLimit.draw(window);
        btnInteg.draw(window);

        // Param inputs
        if (xBox.visible)
            drawText(window, font, "x  VALUE", { PAD,208.f }, TEXT_DIM, 11);
        if (aBox.visible) {
            drawText(window, font, "LOWER LIMIT  a", { PAD,208.f }, TEXT_DIM, 11);
            drawText(window, font, "UPPER LIMIT  b", { PAD + (LW - PAD - 10) / 2.f + 10,208.f }, TEXT_DIM, 11);
        }
        xBox.draw(window);
        aBox.draw(window);
        bBox.draw(window);

        // Calculate button — only show when params needed
        if (step == Step::ENTER_PARAMS || step == Step::SHOW_RESULT)
            btnCalc.draw(window);

        // Toggle deriv button
        btnToggleDeriv.draw(window);

        // x-range label + inputs
        drawText(window, font, "GRAPH  X-RANGE", { PAD,cy + 2.f }, TEXT_DIM, 11);
        xMinBox.draw(window);
        xMaxBox.draw(window);

        // ── Result card ───────────────────────────────────────
        if (!res1.empty() || !errText.empty())
        {
            float ry = cy + 56.f;
            sf::Color borderCol = errText.empty() ? ACCENT_GRN : TEXT_ERR;
            drawRect(window, { {PAD,ry},{LW - PAD,H - ry - PAD} }, BG_RESULT, borderCol, 1.5f);

            // Left accent bar
            drawRect(window, { {PAD,ry},{3.f,H - ry - PAD} }, borderCol);

            float ty = ry + 14.f;
            drawText(window, font, "RESULT", { PAD + 12.f,ty }, TEXT_DIM, 11);
            ty += 20.f;

            if (!errText.empty())
            {
                // wrap long error
                drawText(window, font, errText, { PAD + 12.f,ty }, TEXT_ERR, 14);
            }
            else
            {
                // Line 1 — symbolic / label
                if (!res1.empty())
                {
                    drawText(window, font, res1, { PAD + 12.f,ty }, ACCENT_GRN, 14);
                    ty += 22.f;
                }
                // Line 2 — main value (bigger)
                if (!res2.empty())
                {
                    drawText(window, font, res2, { PAD + 12.f,ty }, TEXT_HI, 18, true);
                    ty += 28.f;
                }
                // Line 3 — secondary
                if (!res3.empty())
                    drawText(window, font, res3, { PAD + 12.f,ty }, ACCENT_GOLD, 14);
            }
        }

        // ── Graph panel ───────────────────────────────────────
        drawText(window, font, "GRAPH", { GX + 50.f,GY - 18.f }, TEXT_DIM, 11);
        sf::FloatRect graphArea{ {GX + 50.f,GY},{GW - 50.f,GH - 20.f} };
        drawGraph(window, font, rootNode, derivNode, graphArea,
            xRangeMin, xRangeMax, showDeriv);

        window.display();
    }
    return 0;
}