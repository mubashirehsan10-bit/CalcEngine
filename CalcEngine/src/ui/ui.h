#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <iomanip>
#include <cmath>

// ═══════════════════════════════════════════════════════════════════
//  FORWARD DECLARATIONS (your calculus classes)
// ═══════════════════════════════════════════════════════════════════
class ASTNode;

// ═══════════════════════════════════════════════════════════════════
//  PALETTE
// ═══════════════════════════════════════════════════════════════════
namespace Pal
{
    // Backgrounds
    inline const sf::Color BG_BASE{ 8,   10,  20 };
    inline const sf::Color BG_PANEL{ 14,  18,  32 };
    inline const sf::Color BG_CARD{ 18,  23,  42 };
    inline const sf::Color BG_INPUT{ 22,  28,  50 };
    inline const sf::Color BG_OVERLAY{ 0,   0,   0, 160 };

    // Accents
    inline const sf::Color CYAN{ 0,   210, 255 };
    inline const sf::Color CYAN_DIM{ 0,   140, 180 };
    inline const sf::Color GOLD{ 255, 200, 70 };
    inline const sf::Color GREEN{ 60,  255, 160 };
    inline const sf::Color PINK{ 255, 100, 180 };
    inline const sf::Color PURPLE{ 180, 120, 255 };

    // Text
    inline const sf::Color TXT_HI{ 225, 232, 255 };
    inline const sf::Color TXT_MID{ 150, 162, 200 };
    inline const sf::Color TXT_DIM{ 90,  100, 140 };
    inline const sf::Color TXT_ERR{ 255, 75,  75 };

    // UI chrome
    inline const sf::Color DIVIDER{ 30,  38,  65 };
    inline const sf::Color GRID{ 22,  28,  50 };
    inline const sf::Color AXIS{ 50,  62,  100 };
    inline const sf::Color BTN_IDLE{ 24,  30,  55 };
    inline const sf::Color BTN_HOVER{ 40,  52,  90 };

    // Glow helpers — same hue, lower alpha
    inline sf::Color glow(sf::Color c, std::uint8_t a)
    {
        c.a = a; return c;
    }
}

// ═══════════════════════════════════════════════════════════════════
//  UTILITY
// ═══════════════════════════════════════════════════════════════════
namespace UI
{
    std::string   fmt(double v, int prec = 4);
    double        safeStod(const std::string& s, double def = 0.0);

    void drawRect(sf::RenderWindow& w, sf::FloatRect r,
        sf::Color fill,
        sf::Color outline = { 0,0,0,0 }, float ot = 0.f);

    void drawText(sf::RenderWindow& w, const sf::Font& f,
        const std::string& str, sf::Vector2f pos,
        sf::Color col, unsigned sz, bool bold = false);

    // Draw a glowing horizontal/vertical line
    void drawGlowLine(sf::RenderWindow& w,
        sf::Vector2f a, sf::Vector2f b,
        sf::Color col, float thickness = 2.f, int glowLayers = 3);

    // Gradient rectangle (top-bottom)
    void drawGradientRect(sf::RenderWindow& w, sf::FloatRect r,
        sf::Color top, sf::Color bottom);
}

// ═══════════════════════════════════════════════════════════════════
//  ANIMATED CURSOR
// ═══════════════════════════════════════════════════════════════════
struct AnimCursor
{
    sf::Clock clock;
    bool visible() const {
        return (int)clock.getElapsedTime().asSeconds() % 2 == 0
            || clock.getElapsedTime().asMilliseconds() % 1000 < 500;
    }
    void reset() { clock.restart(); }
};

// ═══════════════════════════════════════════════════════════════════
//  BUTTON
// ═══════════════════════════════════════════════════════════════════
struct Button
{
    sf::RectangleShape bg;
    sf::Text           lbl;
    sf::Color          accent;
    bool               hovered = false;
    bool               active = false;
    bool               visible = true;

    Button() = default;
    Button(const sf::Font& font, const std::string& text,
        sf::Vector2f pos, sf::Vector2f size,
        sf::Color accentColor, unsigned fontSize = 14);

    bool contains(sf::Vector2i p) const;
    void update(sf::Vector2i mouse);
    void draw(sf::RenderWindow& w);

    sf::Vector2f getPos()  const { return bg.getPosition(); }
    sf::Vector2f getSize() const { return bg.getSize(); }
};

// ═══════════════════════════════════════════════════════════════════
//  INPUT BOX  (supports Tab cycling)
// ═══════════════════════════════════════════════════════════════════
struct InputBox
{
    sf::RectangleShape bg;
    sf::Text           disp, ph;
    std::string        value;
    bool               focused = false;
    bool               visible = true;
    AnimCursor         cursor;

    InputBox() = default;
    InputBox(const sf::Font& font, const std::string& placeholder,
        sf::Vector2f pos, sf::Vector2f size, unsigned charSz = 16);

    bool contains(sf::Vector2i p) const;
    void handleText(uint32_t unicode);
    void setFocus(bool f);
    void updateFocus(sf::Vector2i mouse, bool clicked);
    void draw(sf::RenderWindow& w);
};

// ═══════════════════════════════════════════════════════════════════
//  HISTORY ENTRY
// ═══════════════════════════════════════════════════════════════════
struct HistEntry
{
    std::string eq;
    std::string op;
    std::string result;
    sf::Color   col;
};

// ═══════════════════════════════════════════════════════════════════
//  GRAPH
// ═══════════════════════════════════════════════════════════════════
struct Graph
{
    sf::FloatRect area;
    double        xMin = -5.0, xMax = 5.0;
    bool          showDeriv = false;

    // crosshair state
    bool          crosshairActive = false;
    sf::Vector2f  crosshairPos;

    ASTNode* fxNode = nullptr;
    ASTNode* dxNode = nullptr;

    void handleScroll(float delta);
    void draw(sf::RenderWindow& w, const sf::Font& font);

private:
    // helpers
    float mapX(double x) const;
    float mapY(double y, double yMn, double yMx) const;
    void  plotCurve(sf::RenderWindow& w, ASTNode* node,
        sf::Color col, double yMn, double yMx,
        bool glow);
    void  computeYRange(double& yMn, double& yMx);
};

// ═══════════════════════════════════════════════════════════════════
//  CALC APP
// ═══════════════════════════════════════════════════════════════════
class CalcApp
{
public:
    CalcApp();
    void run();

private:
    // window & font
    sf::RenderWindow window;
    sf::Font         font;

    // layout constants
    static constexpr unsigned W = 1340;
    static constexpr unsigned H = 800;
    static constexpr float    PAD = 22.f;
    static constexpr float    LW = 400.f;   // left panel width
    static constexpr float    GX = LW + PAD * 2.f;
    static constexpr float    GY = 68.f;

    // mode / step
    enum class Mode { NONE, EVAL, DERIV, LIMIT, INTEG };
    enum class Step { SELECT_OP, ENTER_PARAMS, SHOW_RESULT };
    Mode mode = Mode::NONE;
    Step step = Step::SELECT_OP;

    // calc state
    ASTNode* rootNode = nullptr;
    ASTNode* derivNode = nullptr;

    std::string res1, res2, res3, errText;
    std::vector<HistEntry> history;
    sf::Color   resAccent = Pal::GREEN;

    // widgets
    InputBox eqBox, xBox, aBox, bBox, xMinBox, xMaxBox;
    Button   btnEval, btnDeriv, btnLimit, btnInteg;
    Button   btnCalc, btnToggleDeriv;

    // graph
    Graph graph;

    // tab order for inputs
    std::vector<InputBox*> tabOrder;
    int                    tabIdx = 0;

    // helpers
    void buildWidgets();
    void handleEvents();
    void handleClick(sf::Vector2i mouse);
    void runCalculation();
    void updateVisibility();
    void draw();

    // sub-draw
    void drawHeader();
    void drawLeftPanel();
    void drawResultCard(float y);
    void drawHistory(float y);
    void drawStepHint();
};