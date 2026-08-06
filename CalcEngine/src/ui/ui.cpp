#include "ui.h"
#include "../parser/Tokenizer.h"
#include "../parser/Parser.h"
#include "../calculus/Evaluator.h"
#include "../calculus/Limits.h"
#include "../calculus/Differentiator.h"
#include "../calculus/Integrator.h"

#include <stdexcept>
#include <algorithm>

// ═══════════════════════════════════════════════════════════════════
//  UTILITY
// ═══════════════════════════════════════════════════════════════════
std::string UI::fmt(double v, int prec)
{
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(prec) << v;
    return ss.str();
}

double UI::safeStod(const std::string& s, double def)
{
    if (s.empty()) return def;
    try { return std::stod(s); }
    catch (...) { return def; }
}

void UI::drawRect(sf::RenderWindow& w, sf::FloatRect r,
    sf::Color fill, sf::Color outline, float ot)
{
    sf::RectangleShape s(r.size);
    s.setPosition(r.position);
    s.setFillColor(fill);
    if (ot > 0.f) { s.setOutlineThickness(ot); s.setOutlineColor(outline); }
    w.draw(s);
}

void UI::drawText(sf::RenderWindow& w, const sf::Font& f,
    const std::string& str, sf::Vector2f pos,
    sf::Color col, unsigned sz, bool bold)
{
    if (str.empty()) return;
    sf::Text t(f, str, sz);
    t.setFillColor(col);
    t.setPosition(pos);
    if (bold) t.setStyle(sf::Text::Bold);
    w.draw(t);
}

void UI::drawGradientRect(sf::RenderWindow& w, sf::FloatRect r,
    sf::Color top, sf::Color bot)
{
    sf::Vertex q[4] = {
        {{r.position.x,          r.position.y         }, top},
        {{r.position.x + r.size.x, r.position.y         }, top},
        {{r.position.x + r.size.x, r.position.y + r.size.y}, bot},
        {{r.position.x,          r.position.y + r.size.y}, bot}
    };
    w.draw(q, 4, sf::PrimitiveType::TriangleFan);
}

// ═══════════════════════════════════════════════════════════════════
//  BUTTON
// ═══════════════════════════════════════════════════════════════════
Button::Button(const std::string& t, sf::Vector2f pos, sf::Vector2f size,
    sf::Color ac, unsigned fs)
    : text(t), fontSize(fs), accent(ac)
{
    bg.setPosition(pos);
    bg.setSize(size);
    bg.setFillColor(Pal::BTN_IDLE);
    bg.setOutlineThickness(1.5f);
    bg.setOutlineColor(Pal::DIVIDER);
}

bool Button::contains(sf::Vector2i p) const
{
    return visible && bg.getGlobalBounds().contains(sf::Vector2f(p));
}

void Button::update(sf::Vector2i mouse)
{
    if (!visible) return;
    hovered = contains(mouse);
    if (active) { bg.setFillColor(accent);       bg.setOutlineColor(accent); }
    else if (hovered) { bg.setFillColor(Pal::BTN_HOV); bg.setOutlineColor(accent); }
    else { bg.setFillColor(Pal::BTN_IDLE); bg.setOutlineColor(Pal::DIVIDER); }
}

void Button::draw(sf::RenderWindow& w, const sf::Font& font)
{
    if (!visible) return;

    if (active)
    {
        sf::RectangleShape gl(bg.getSize() + sf::Vector2f(6.f, 6.f));
        gl.setPosition(bg.getPosition() - sf::Vector2f(3.f, 3.f));
        gl.setFillColor({ 0,0,0,0 });
        gl.setOutlineThickness(3.f);
        gl.setOutlineColor(Pal::glow(accent, 55));
        w.draw(gl);
    }
    w.draw(bg);

    sf::Text lbl(font, text, fontSize);
    lbl.setFillColor(active ? Pal::BG_BASE : hovered ? Pal::TXT_HI : Pal::TXT_MID);
    if (active) lbl.setStyle(sf::Text::Bold);
    auto lb = lbl.getLocalBounds();
    lbl.setOrigin({ lb.position.x + lb.size.x / 2.f, lb.position.y + lb.size.y / 2.f });
    auto p = bg.getPosition(); auto s = bg.getSize();
    lbl.setPosition({ p.x + s.x / 2.f, p.y + s.y / 2.f });
    w.draw(lbl);
}

// ═══════════════════════════════════════════════════════════════════
//  INPUT BOX
// ═══════════════════════════════════════════════════════════════════
InputBox::InputBox(const std::string& ph, sf::Vector2f pos,
    sf::Vector2f size, unsigned sz)
    : placeholder(ph), charSz(sz)
{
    bg.setPosition(pos);
    bg.setSize(size);
    bg.setFillColor(Pal::BG_INPUT);
    bg.setOutlineThickness(1.5f);
    bg.setOutlineColor(Pal::DIVIDER);
}

bool InputBox::contains(sf::Vector2i p) const
{
    return visible && bg.getGlobalBounds().contains(sf::Vector2f(p));
}

void InputBox::handleText(uint32_t u)
{
    if (!focused || !visible) return;
    if (u == 8 && !value.empty()) { value.pop_back(); cursor.reset(); }
    else if (u >= 32 && u < 128) { value += (char)u;   cursor.reset(); }
}

void InputBox::setFocus(bool f)
{
    focused = f;
    bg.setOutlineColor(f ? Pal::CYAN : Pal::DIVIDER);
    if (f) cursor.reset();
}

void InputBox::updateFocus(sf::Vector2i mouse, bool clicked)
{
    if (!visible) { setFocus(false); return; }
    if (clicked)   setFocus(contains(mouse));
}

void InputBox::draw(sf::RenderWindow& w, const sf::Font& font)
{
    if (!visible) return;

    if (focused)
    {
        sf::RectangleShape gl(bg.getSize() + sf::Vector2f(6.f, 6.f));
        gl.setPosition(bg.getPosition() - sf::Vector2f(3.f, 3.f));
        gl.setFillColor({ 0,0,0,0 });
        gl.setOutlineThickness(3.f);
        gl.setOutlineColor(Pal::glow(Pal::CYAN, 50));
        w.draw(gl);
    }
    w.draw(bg);

    auto pos = bg.getPosition(); auto sz = bg.getSize();
    float ty = pos.y + sz.y / 2.f - (float)charSz / 2.f - 1.f;
    float tx = pos.x + 12.f;

    if (value.empty() && !focused)
    {
        sf::Text ph(font, placeholder, charSz);
        ph.setFillColor(Pal::TXT_DIM);
        ph.setPosition({ tx,ty });
        w.draw(ph);
    }
    else
    {
        std::string shown = value + (focused && cursor.visible() ? "|" : "");
        sf::Text disp(font, shown, charSz);
        disp.setFillColor(Pal::TXT_HI);
        disp.setPosition({ tx,ty });
        w.draw(disp);
    }
}

// ═══════════════════════════════════════════════════════════════════
//  GRAPH
// ═══════════════════════════════════════════════════════════════════
// ═══════════════════════════════════════════════════════════════════
//  GRAPH  — complete replacement for CalcUI.cpp Graph section
// ═══════════════════════════════════════════════════════════════════

float Graph::mapX(double x) const
{
    return area.position.x + (float)((x - xMin) / (xMax - xMin)) * area.size.x;
}

float Graph::mapY(double y, double yMn, double yMx) const
{
    return area.position.y + area.size.y - (float)((y - yMn) / (yMx - yMn)) * area.size.y;
}

void Graph::computeYRange(double& yMn, double& yMx)
{
    int N = 500;
    double step = (xMax - xMin) / N;
    yMn = 1e18; yMx = -1e18;

    auto sample = [&](ASTNode* n) {
        if (!n) return;
        for (int i = 0; i <= N; ++i) {
            double x = xMin + i * step;
            try {
                double y = Evaluator(n, x).Result();
                if (std::isfinite(y)) { yMn = std::min(yMn, y); yMx = std::max(yMx, y); }
            }
            catch (...) {}
        }
        };

    sample(fxNode);
    if (showDeriv) sample(dxNode);

    if (!std::isfinite(yMn) || !std::isfinite(yMx)) { yMn = -10; yMx = 10; }
    if (yMn == yMx) { yMn -= 1; yMx += 1; }
    double pad = (yMx - yMn) * 0.12;
    yMn -= pad; yMx += pad;
}

void Graph::plotCurve(sf::RenderWindow& w, ASTNode* node,
    sf::Color col, double yMn, double yMx)
{
    if (!node) return;
    int N = 700;
    double step = (xMax - xMin) / N;

    std::vector<sf::Vertex> pts;
    bool prev = false;
    double prevY = 1e18;

    for (int i = 0; i <= N; ++i)
    {
        double x = xMin + i * step;
        try {
            double y = Evaluator(node, x).Result();
            if (!std::isfinite(y)) { prev = false; prevY = 1e18; continue; }

            if (prev && std::abs(y - prevY) > (yMx - yMn) * 2.0) {
                if (pts.size() > 1)
                    w.draw(pts.data(), pts.size(), sf::PrimitiveType::LineStrip);
                pts.clear(); prev = false;
            }

            float px = mapX(x), py = mapY(y, yMn, yMx);
            if (py < area.position.y - 2.f || py > area.position.y + area.size.y + 2.f)
            {
                prev = false; prevY = y; continue;
            }

            pts.push_back({ {px, py}, col });
            prev = true; prevY = y;
        }
        catch (...) { prev = false; prevY = 1e18; }
    }
    if (pts.size() > 1)
        w.draw(pts.data(), pts.size(), sf::PrimitiveType::LineStrip);

    // glow pass
    std::vector<sf::Vertex> gpts;
    prev = false; prevY = 1e18;
    for (int i = 0; i <= N; ++i)
    {
        double x = xMin + i * step;
        try {
            double y = Evaluator(node, x).Result();
            if (!std::isfinite(y)) { prev = false; prevY = 1e18; continue; }
            float px = mapX(x), py = mapY(y, yMn, yMx);
            if (py < area.position.y - 2.f || py > area.position.y + area.size.y + 2.f)
            {
                prev = false; continue;
            }
            gpts.push_back({ {px, py}, Pal::glow(col, 35) });
            prev = true; prevY = y;
        }
        catch (...) { prev = false; }
    }
    for (float off : {-2.f, -1.f, 1.f, 2.f}) {
        std::vector<sf::Vertex> ov;
        for (auto& v : gpts)
            ov.push_back({ {v.position.x, v.position.y + off}, v.color });
        if (ov.size() > 1)
            w.draw(ov.data(), ov.size(), sf::PrimitiveType::LineStrip);
    }
}

void Graph::drawIntegRegion(sf::RenderWindow& w, const sf::Font& font,
    double yMn, double yMx)
{
    if (!showIntegRegion || !fxNode) return;

    // clamp a and b to visible range for drawing
    double a = std::max(integA, xMin);
    double b = std::min(integB, xMax);
    if (a >= b) return;

    int    N = 400;
    double step = (b - a) / N;
    float  yZero = mapY(0.0, yMn, yMx);

    // clamp yZero to area bounds
    yZero = std::max(yZero, area.position.y);
    yZero = std::min(yZero, area.position.y + area.size.y);

    // shaded triangle strip from baseline to curve
    std::vector<sf::Vertex> strip;
    sf::Color fillCol = Pal::glow(Pal::PINK, 50);

    for (int i = 0; i <= N; ++i)
    {
        double x = a + i * step;
        try {
            double y = Evaluator(fxNode, x).Result();
            if (!std::isfinite(y)) continue;
            float px = mapX(x);
            float py = mapY(y, yMn, yMx);
            py = std::max(py, area.position.y);
            py = std::min(py, area.position.y + area.size.y);
            strip.push_back({ {px, yZero}, fillCol });
            strip.push_back({ {px, py},    fillCol });
        }
        catch (...) {}
    }
    if (strip.size() > 2)
        w.draw(strip.data(), strip.size(), sf::PrimitiveType::TriangleStrip);

    // vertical limit lines at a and b
    float xA = mapX(integA);
    float xB = mapX(integB);

    // dashed effect — draw segments
    float top = area.position.y;
    float bot = area.position.y + area.size.y;
    int   segs = 20;
    float segH = (bot - top) / (segs * 2.f);

    for (int i = 0; i < segs; ++i)
    {
        float y0 = top + i * segH * 2.f;
        float y1 = y0 + segH;
        sf::Vertex la[2] = { {{xA, y0}, Pal::PINK}, {{xA, y1}, Pal::PINK} };
        sf::Vertex lb[2] = { {{xB, y0}, Pal::PINK}, {{xB, y1}, Pal::PINK} };
        w.draw(la, 2, sf::PrimitiveType::Lines);
        w.draw(lb, 2, sf::PrimitiveType::Lines);
    }

    // a and b labels — place above the line, avoid going off-screen
    float labelY = area.position.y + 6.f;

    float axLabelX = xA + 5.f;
    if (axLabelX + 50.f > area.position.x + area.size.x) axLabelX = xA - 45.f;

    float bxLabelX = xB + 5.f;
    if (bxLabelX + 50.f > area.position.x + area.size.x) bxLabelX = xB - 45.f;

    // label backgrounds
    UI::drawRect(w, { {axLabelX - 2.f, labelY - 1.f}, {46.f, 18.f} },
        Pal::glow(Pal::BG_CARD, 200), Pal::PINK, 1.f);
    UI::drawRect(w, { {bxLabelX - 2.f, labelY - 1.f}, {46.f, 18.f} },
        Pal::glow(Pal::BG_CARD, 200), Pal::PINK, 1.f);

    UI::drawText(w, font, "a=" + UI::fmt(integA, 2), { axLabelX, labelY }, Pal::PINK, 12);
    UI::drawText(w, font, "b=" + UI::fmt(integB, 2), { bxLabelX, labelY }, Pal::PINK, 12);
}

void Graph::handleScroll(float delta)
{
    double mid = (xMin + xMax) / 2.0;
    double half = (xMax - xMin) / 2.0;
    half *= (delta > 0) ? 0.85 : 1.18;
    half = std::max(0.1, std::min(half, 500.0));
    xMin = mid - half;
    xMax = mid + half;
}

void Graph::draw(sf::RenderWindow& w, const sf::Font& font)
{
    UI::drawRect(w, area, Pal::BG_PANEL, Pal::DIVIDER, 1.f);

    if (!fxNode) {
        UI::drawText(w, font,
            "Graph appears here after calculation.",
            { area.position.x + 24.f, area.position.y + area.size.y / 2.f - 10.f },
            Pal::TXT_DIM, 14);
        return;
    }

    double yMn, yMx;
    computeYRange(yMn, yMx);

    // grid
    for (int i = 0; i <= 8; ++i) {
        float gx = area.position.x + (float)i / 8.f * area.size.x;
        float gy = area.position.y + (float)i / 8.f * area.size.y;
        sf::Vertex lx[2] = { {{gx, area.position.y}, Pal::GRID},
                             {{gx, area.position.y + area.size.y}, Pal::GRID} };
        sf::Vertex ly[2] = { {{area.position.x, gy}, Pal::GRID},
                             {{area.position.x + area.size.x, gy}, Pal::GRID} };
        w.draw(lx, 2, sf::PrimitiveType::Lines);
        w.draw(ly, 2, sf::PrimitiveType::Lines);
    }

    // x-axis
    if (yMn < 0 && yMx > 0) {
        float ay = mapY(0, yMn, yMx);
        sf::Vertex v[2] = { {{area.position.x, ay}, Pal::AXIS},
                            {{area.position.x + area.size.x, ay}, Pal::AXIS} };
        w.draw(v, 2, sf::PrimitiveType::Lines);
    }

    // y-axis
    if (xMin < 0 && xMax > 0) {
        float ax = mapX(0);
        sf::Vertex v[2] = { {{ax, area.position.y}, Pal::AXIS},
                            {{ax, area.position.y + area.size.y}, Pal::AXIS} };
        w.draw(v, 2, sf::PrimitiveType::Lines);
    }

    // tick labels
    for (int i = 0; i <= 5; ++i) {
        double xv = xMin + (xMax - xMin) * i / 5.0;
        double yv = yMn + (yMx - yMn) * i / 5.0;
        UI::drawText(w, font, UI::fmt(xv, 1),
            { mapX(xv) - 14.f, area.position.y + area.size.y + 4.f }, Pal::TXT_DIM, 11);
        UI::drawText(w, font, UI::fmt(yv, 1),
            { area.position.x - 46.f, mapY(yv, yMn, yMx) - 8.f }, Pal::TXT_DIM, 11);
    }

    // integration shaded region (draw before curves so curves appear on top)
    drawIntegRegion(w, font, yMn, yMx);

    // curves
    plotCurve(w, fxNode, Pal::CYAN, yMn, yMx);
    if (showDeriv && dxNode)
        plotCurve(w, dxNode, Pal::GOLD, yMn, yMx);

    // crosshair
    if (crosshairActive && area.contains(crosshairPos))
    {
        float cx2 = crosshairPos.x, cy2 = crosshairPos.y;
        sf::Color ch = Pal::glow(Pal::TXT_MID, 100);
        sf::Vertex vx[2] = { {{area.position.x, cy2}, ch},
                             {{area.position.x + area.size.x, cy2}, ch} };
        sf::Vertex vy[2] = { {{cx2, area.position.y}, ch},
                             {{cx2, area.position.y + area.size.y}, ch} };
        w.draw(vx, 2, sf::PrimitiveType::Lines);
        w.draw(vy, 2, sf::PrimitiveType::Lines);

        sf::CircleShape dot(3.f);
        dot.setFillColor(Pal::CYAN);
        dot.setPosition({ cx2 - 3.f, cy2 - 3.f });
        w.draw(dot);

        double wx = xMin + (cx2 - area.position.x) / area.size.x * (xMax - xMin);
        double wy = yMx - (cy2 - area.position.y) / area.size.y * (yMx - yMn);
        std::string tip = "x=" + UI::fmt(wx, 3) + "  y=" + UI::fmt(wy, 3);

        float tx2 = cx2 + 8.f, ty2 = cy2 - 22.f;
        if (tx2 + 136.f > area.position.x + area.size.x) tx2 = cx2 - 144.f;
        if (ty2 < area.position.y) ty2 = cy2 + 6.f;

        UI::drawRect(w, { {tx2 - 4.f, ty2 - 2.f}, {142.f, 20.f} },
            Pal::BG_CARD, Pal::DIVIDER, 1.f);
        UI::drawText(w, font, tip, { tx2, ty2 }, Pal::CYAN, 12);
    }

    // legend
    float lx2 = area.position.x + 10.f, ly2 = area.position.y + 10.f;
    float lh = (showDeriv && dxNode) ? 48.f : 26.f;
    if (showIntegRegion) lh += 22.f;

    UI::drawRect(w, { {lx2, ly2}, {100.f, lh} },
        Pal::glow(Pal::BG_CARD, 200), Pal::DIVIDER, 1.f);

    float legendY = ly2;

    sf::RectangleShape l1({ 12.f, 2.f });
    l1.setFillColor(Pal::CYAN);
    l1.setPosition({ lx2 + 6.f, legendY + 10.f });
    w.draw(l1);
    UI::drawText(w, font, "f(x)", { lx2 + 22.f, legendY + 3.f }, Pal::CYAN, 12);
    legendY += 22.f;

    if (showDeriv && dxNode) {
        sf::RectangleShape l2({ 12.f, 2.f });
        l2.setFillColor(Pal::GOLD);
        l2.setPosition({ lx2 + 6.f, legendY + 10.f });
        w.draw(l2);
        UI::drawText(w, font, "f'(x)", { lx2 + 22.f, legendY + 3.f }, Pal::GOLD, 12);
        legendY += 22.f;
    }

    if (showIntegRegion) {
        sf::RectangleShape l3({ 12.f, 10.f });
        l3.setFillColor(Pal::glow(Pal::PINK, 120));
        l3.setPosition({ lx2 + 6.f, legendY + 4.f });
        w.draw(l3);
        UI::drawText(w, font, "area", { lx2 + 22.f, legendY + 3.f }, Pal::PINK, 12);
    }

    UI::drawText(w, font, "scroll to zoom",
        { area.position.x + area.size.x - 96.f,
         area.position.y + area.size.y + 4.f },
        Pal::TXT_DIM, 11);
}

// ═══════════════════════════════════════════════════════════════════
//  CALC APP
// ═══════════════════════════════════════════════════════════════════
CalcApp::CalcApp()
    : window(sf::VideoMode({ W,H }), "Calculus Engine ", sf::Style::Close)
{
    window.setFramerateLimit(60);
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
        throw std::runtime_error("Font not found.");
    buildWidgets();
}

void CalcApp::buildWidgets()
{
    float cy = 80.f;

    eqBox = InputBox("e.g.  x^2 + sin(x)", { PAD,cy }, { LW - PAD,48.f }, 18);
    cy += 64.f;

    float bw = (LW - PAD - 30.f) / 4.f;
    btnEval = Button("Evaluate", { PAD,           cy }, { bw,40.f }, Pal::GREEN, 13);
    btnDeriv = Button("Derive", { PAD + bw + 10,     cy }, { bw,40.f }, Pal::CYAN, 13);
    btnLimit = Button("Limit", { PAD + (bw + 10) * 2, cy }, { bw,40.f }, Pal::GOLD, 13);
    btnInteg = Button("Integrate", { PAD + (bw + 10) * 3, cy }, { bw,40.f }, Pal::PINK, 13);
    cy += 54.f;

    float hw = (LW - PAD - 10.f) / 2.f;
    xBox = InputBox("x value", { PAD,      cy }, { hw,40.f }, 15);
    aBox = InputBox("a  (lower limit)", { PAD,      cy }, { hw,40.f }, 15);
    bBox = InputBox("b  (upper limit)", { PAD + hw + 10,cy }, { hw,40.f }, 15);
    cy += 54.f;

    btnCalc = Button("CALCULATE", { PAD,cy }, { LW - PAD,42.f }, Pal::GREEN, 15);
    cy += 56.f;
    btnToggleDeriv = Button("Show  f'(x)  on graph", { PAD,cy }, { LW - PAD,36.f }, Pal::CYAN, 13);
    cy += 50.f;

    float hw2 = (LW - PAD - 10.f) / 2.f;
    xMinBox = InputBox("x-min  (-5)", { PAD,       cy }, { hw2,34.f }, 13);
    xMaxBox = InputBox("x-max   (5)", { PAD + hw2 + 10,cy }, { hw2,34.f }, 13);

    tabOrder = { &eqBox,&xBox,&aBox,&bBox,&xMinBox,&xMaxBox };

    graph.area = { {GX + 50.f,GY},{(float)W - GX - 50.f - PAD,(float)H - GY - PAD - 22.f} };
}

void CalcApp::run()
{
    while (window.isOpen()) { handleEvents(); updateVisibility(); draw(); }
}

void CalcApp::handleEvents()
{
    sf::Vector2i mouse = sf::Mouse::getPosition(window);
    bool clicked = false;

    while (const std::optional ev = window.pollEvent())
    {
        if (ev->is<sf::Event::Closed>()) window.close();

        if (const auto* k = ev->getIf<sf::Event::KeyPressed>())
        {
            if (k->code == sf::Keyboard::Key::Escape) window.close();

            if (k->code == sf::Keyboard::Key::Tab) {
                for (auto* ib : tabOrder) ib->setFocus(false);
                tabIdx = (tabIdx + 1) % (int)tabOrder.size();
                int tries = 0;
                while (!tabOrder[tabIdx]->visible && tries < (int)tabOrder.size())
                {
                    tabIdx = (tabIdx + 1) % (int)tabOrder.size(); ++tries;
                }
                tabOrder[tabIdx]->setFocus(true);
            }
            if (k->code == sf::Keyboard::Key::Enter && step == Step::ENTER_PARAMS)
                runCalculation();
        }

        if (const auto* t = ev->getIf<sf::Event::TextEntered>())
        {
            eqBox.handleText(t->unicode);
            if (xBox.visible)   xBox.handleText(t->unicode);
            if (aBox.visible)   aBox.handleText(t->unicode);
            if (bBox.visible)   bBox.handleText(t->unicode);
            xMinBox.handleText(t->unicode);
            xMaxBox.handleText(t->unicode);
        }

        if (const auto* mb = ev->getIf<sf::Event::MouseButtonPressed>())
            if (mb->button == sf::Mouse::Button::Left) clicked = true;

        if (const auto* sw = ev->getIf<sf::Event::MouseWheelScrolled>())
            if (graph.area.contains(sf::Vector2f(sf::Mouse::getPosition(window))))
                graph.handleScroll(sw->delta);

        if (const auto* mm = ev->getIf<sf::Event::MouseMoved>())
        {
            graph.crosshairPos = { sf::Vector2f(mm->position) };
            graph.crosshairActive = graph.area.contains(graph.crosshairPos);
        }
    }

    if (!xMinBox.value.empty()) graph.xMin = UI::safeStod(xMinBox.value, -5.0);
    if (!xMaxBox.value.empty()) graph.xMax = UI::safeStod(xMaxBox.value, 5.0);
    if (graph.xMin >= graph.xMax) graph.xMax = graph.xMin + 1.0;

    btnEval.update(mouse);  btnDeriv.update(mouse);
    btnLimit.update(mouse); btnInteg.update(mouse);
    btnCalc.update(mouse);  btnToggleDeriv.update(mouse);

    btnEval.active = (mode == Mode::EVAL);
    btnDeriv.active = (mode == Mode::DERIV);
    btnLimit.active = (mode == Mode::LIMIT);
    btnInteg.active = (mode == Mode::INTEG);
    btnToggleDeriv.active = graph.showDeriv;

    eqBox.updateFocus(mouse, clicked);
    xBox.updateFocus(mouse, clicked);
    aBox.updateFocus(mouse, clicked);
    bBox.updateFocus(mouse, clicked);
    xMinBox.updateFocus(mouse, clicked);
    xMaxBox.updateFocus(mouse, clicked);

    if (clicked) handleClick(mouse);
}

void CalcApp::handleClick(sf::Vector2i mouse)
{
    auto sel = [&](Mode m) {
        if (mode != m) { mode = m; step = Step::ENTER_PARAMS; res1 = res2 = res3 = errText = ""; }
        graph.showIntegRegion = false;
        };
    if (btnEval.contains(mouse))  sel(Mode::EVAL);
    else if (btnDeriv.contains(mouse)) sel(Mode::DERIV);
    else if (btnLimit.contains(mouse)) sel(Mode::LIMIT);
    else if (btnInteg.contains(mouse)) sel(Mode::INTEG);

    if (btnCalc.contains(mouse) && step == Step::ENTER_PARAMS)
        runCalculation();

    if (btnToggleDeriv.contains(mouse)) {
        graph.showDeriv = !graph.showDeriv;
        if (graph.showDeriv && !graph.dxNode && graph.fxNode) {
            try {
                TangentToCurve tc(graph.fxNode, UI::safeStod(xBox.value, 1.0));
                graph.dxNode = tc.Derivative();
                derivNode = graph.dxNode;
            }
            catch (...) {}
        }
    }
}

void CalcApp::runCalculation()
{
    if (eqBox.value.empty()) { errText = "Please enter an equation."; return; }
    errText = ""; res1 = res2 = res3 = "";
    rootNode = nullptr; derivNode = nullptr;
    graph.fxNode = nullptr; graph.dxNode = nullptr;

    try
    {
        Tokenizer tk(eqBox.value);
        auto arr = tk.tokenize();
        Parser p(arr);
        rootNode = p.parse();
        if (!rootNode) throw std::runtime_error("Could not parse expression.");
        graph.fxNode = rootNode;

        double xv = UI::safeStod(xBox.value, 1.0);

        if (mode == Mode::EVAL) {
            double r = Evaluator(rootNode, xv).Result();
            res1 = "f(" + UI::fmt(xv, 2) + ")";
            res2 = UI::fmt(r, 6);
            resAccent = Pal::GREEN;
        }
        else if (mode == Mode::DERIV) {
            TangentToCurve tc(rootNode, xv);
            derivNode = tc.Derivative();
            graph.dxNode = derivNode;
            if (!derivNode) throw std::runtime_error("Derivative returned null.");
            res1 = "f'(x) = " + tc.toString(derivNode);
            res2 = "f'(" + UI::fmt(xv, 2) + ") = " + UI::fmt(Evaluator(derivNode, xv).Result(), 6);
            try { res3 = "Numerical slope = " + UI::fmt(tc.Slope(), 6); }
            catch (...) { res3 = "Slope undefined at this x."; }
            resAccent = Pal::CYAN;
        }
        else if (mode == Mode::LIMIT) {
            Limit lim(rootNode, xv);
            bool ex = lim.Existance();
            res1 = ex ? "Limit EXISTS at x = " + UI::fmt(xv, 2)
                : "Limit does NOT exist at x = " + UI::fmt(xv, 2);
            res2 = ex ? "lim f(x) = " + UI::fmt(lim.value(), 6) : "";
            resAccent = Pal::GOLD;
        }
        else if (mode == Mode::INTEG) {
            double a = UI::safeStod(aBox.value, 0.0);
            double b = UI::safeStod(bBox.value, 1.0);
            Area ar(rootNode, a, b);
            res1 = "Definite integral of f(x)";
            res2 = "from " + UI::fmt(a, 2) + " to " + UI::fmt(b, 2);
            res3 = "= " + UI::fmt(ar.AreaUnderCurve(), 6);
            resAccent = Pal::PINK;
            graph.showIntegRegion = true;
            graph.integA = a;
            graph.integB = b;
        }

        step = Step::SHOW_RESULT;

        std::string opStr;
        if (mode == Mode::EVAL)  opStr = "Eval";
        if (mode == Mode::DERIV) opStr = "Deriv";
        if (mode == Mode::LIMIT) opStr = "Limit";
        if (mode == Mode::INTEG) opStr = "Integ";
        history.insert(history.begin(), { eqBox.value,opStr,res2.empty() ? res1 : res2,resAccent });
        if (history.size() > 5) history.pop_back();
    }
    catch (const std::string& s) { errText = s; }
    catch (const char* s) { errText = s; }
    catch (const std::exception& e) { errText = e.what(); }
    catch (...) { errText = "Unknown error."; }
}

void CalcApp::updateVisibility()
{
    bool needX = (mode == Mode::EVAL || mode == Mode::DERIV || mode == Mode::LIMIT);
    bool needAB = (mode == Mode::INTEG);
    bool pp = (step == Step::ENTER_PARAMS || step == Step::SHOW_RESULT);
    xBox.visible = needX && pp;
    aBox.visible = needAB && pp;
    bBox.visible = needAB && pp;
    btnCalc.visible = (step == Step::ENTER_PARAMS && mode != Mode::NONE);
}

void CalcApp::draw()
{
    window.clear(Pal::BG_BASE);
    drawHeader();
    drawLeftPanel();
    UI::drawText(window, font, "GRAPH", { GX + 50.f,(float)GY - 18.f }, Pal::TXT_DIM, 11);
    graph.draw(window, font);
    window.display();
}

void CalcApp::drawHeader()
{
    UI::drawGradientRect(window, { {0,0},{(float)W,62.f} }, { 16,20,40,255 }, { 10,13,25,255 });
    sf::RectangleShape hl({ (float)W,1.f });
    hl.setPosition({ 0,62.f }); hl.setFillColor(Pal::glow(Pal::CYAN, 80));
    window.draw(hl);
    UI::drawRect(window, { {LW + PAD,62.f},{1.f,(float)H - 62.f} }, Pal::DIVIDER);
    UI::drawText(window, font, "CALCULUS ENGINE ", { PAD,14.f }, Pal::CYAN, 28, true);
    UI::drawText(window, font, "\tSymbolic & Numerical Calculus Engine  ·  C++  ·  Phase 1",
        { PAD + 300.f,22.f }, Pal::TXT_DIM, 12);
}

void CalcApp::drawLeftPanel()
{
    drawStepHint();

    UI::drawText(window, font, "EQUATION", { PAD,76.f }, Pal::TXT_DIM, 14);
    eqBox.draw(window, font);

    UI::drawText(window, font, "OPERATION", { PAD,150.f }, Pal::TXT_DIM, 10);
    btnEval.draw(window, font);
    btnDeriv.draw(window, font);
    btnLimit.draw(window, font);
    btnInteg.draw(window, font);

    if (xBox.visible)
        UI::drawText(window, font, "X  VALUE", { PAD,210.f }, Pal::TXT_DIM, 10);
    if (aBox.visible) {
        UI::drawText(window, font, "LOWER  LIMIT  a", { PAD,210.f }, Pal::TXT_DIM, 10);
        UI::drawText(window, font, "UPPER  LIMIT  b", { PAD + (LW - PAD - 10.f) / 2.f + 10.f,210.f }, Pal::TXT_DIM, 10);
    }
    xBox.draw(window, font);
    aBox.draw(window, font);
    bBox.draw(window, font);
    btnCalc.draw(window, font);

    UI::drawRect(window, { {PAD,322.f},{LW - PAD,1.f} }, Pal::DIVIDER);
    btnToggleDeriv.draw(window, font);

    UI::drawText(window, font, "GRAPH  X-RANGE", { PAD,378.f }, Pal::TXT_DIM, 10);
    xMinBox.draw(window, font);
    xMaxBox.draw(window, font);

    UI::drawRect(window, { {PAD,418.f},{LW - PAD,1.f} }, Pal::DIVIDER);
    drawResultCard(424.f);
    drawHistory(600.f);
}

void CalcApp::drawStepHint()
{
    std::string hint; sf::Color hcol = Pal::TXT_DIM;
    if (mode == Mode::NONE) { hint = "①  Select an operation"; }
    else if (step == Step::ENTER_PARAMS) {
        hint = (mode == Mode::INTEG) ? "②  Enter a and b  →  Calculate"
            : "②  Enter x value  →  Calculate";
        hcol = Pal::GOLD;
    }
    else { hint = "✓  Done  —  edit or pick another operation"; hcol = Pal::GREEN; }
    UI::drawText(window, font, hint, { PAD,64.f }, hcol, 11);
}

void CalcApp::drawResultCard(float y)
{
    if (res1.empty() && errText.empty()) return;
    float cardH = (float)H - y - 200.f; if (cardH < 60.f) cardH = 60.f;
    sf::Color border = errText.empty() ? resAccent : Pal::TXT_ERR;
    UI::drawRect(window, { {PAD,y},{LW - PAD,cardH} }, Pal::BG_CARD, border, 1.f);
    UI::drawRect(window, { {PAD,y},{3.f,cardH} }, border);
    UI::drawRect(window, { {PAD,y},{LW - PAD,1.5f} }, Pal::glow(border, 120));
    UI::drawText(window, font, "RESULT", { PAD + 12.f,y + 10.f }, Pal::TXT_DIM, 10);
    float ty = y + 28.f;
    if (!errText.empty())
        UI::drawText(window, font, errText, { PAD + 12.f,ty }, Pal::TXT_ERR, 13);
    else {
        if (!res1.empty()) { UI::drawText(window, font, res1, { PAD + 12.f,ty }, Pal::TXT_MID, 13); ty += 20.f; }
        if (!res2.empty()) { UI::drawText(window, font, res2, { PAD + 12.f,ty }, Pal::TXT_HI, 17, true); ty += 28.f; }
        if (!res3.empty()) UI::drawText(window, font, res3, { PAD + 12.f,ty }, resAccent, 13);
    }
}

void CalcApp::drawHistory(float y)
{
    if (history.empty()) return;
    float hy = y;
    UI::drawText(window, font, "HISTORY", { PAD,hy }, Pal::TXT_DIM, 10);
    hy += 16.f;
    for (int i = 0; i < (int)history.size() && hy < (float)H - 20.f; ++i) {
        auto& h = history[i];
        UI::drawRect(window, { {PAD,hy},{LW - PAD,26.f} }, (i % 2 == 0) ? Pal::BG_CARD : Pal::BG_PANEL);
        sf::CircleShape dot(3.f); dot.setFillColor(h.col);
        dot.setPosition({ PAD + 8.f,hy + 10.f }); window.draw(dot);
        std::string entry = "[" + h.op + "]  " + h.eq + "  =  " + h.result;
        if (entry.size() > 52) entry = entry.substr(0, 49) + "...";
        UI::drawText(window, font, entry, { PAD + 20.f,hy + 5.f }, Pal::TXT_MID, 12);
        hy += 26.f;
    }
}