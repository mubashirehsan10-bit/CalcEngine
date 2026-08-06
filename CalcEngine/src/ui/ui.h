#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdint>

// ======================================================
// Forward Declaration
// ======================================================

class ASTNode;

// ======================================================
// Palette
// ======================================================

namespace Pal
{
    inline const sf::Color BG_BASE{ 8,10,20 };
    inline const sf::Color BG_PANEL{ 14,18,32 };
    inline const sf::Color BG_CARD{ 18,23,42 };
    inline const sf::Color BG_INPUT{ 22,28,50 };

    inline const sf::Color CYAN{ 0,210,255 };
    inline const sf::Color GREEN{ 60,255,160 };
    inline const sf::Color GOLD{ 255,200,70 };
    inline const sf::Color PINK{ 255,100,180 };

    inline const sf::Color TXT_HI{ 225,232,255 };
    inline const sf::Color TXT_MID{ 150,162,200 };
    inline const sf::Color TXT_DIM{ 90,100,140 };
    inline const sf::Color TXT_ERR{ 255,75,75 };

    inline const sf::Color DIVIDER{ 30,38,65 };
    inline const sf::Color GRID{ 22,28,50 };
    inline const sf::Color AXIS{ 50,62,100 };

    inline const sf::Color BTN_IDLE{ 24,30,55 };
    inline const sf::Color BTN_HOV{ 40,52,90 };

    inline sf::Color glow(sf::Color c, std::uint8_t alpha)
    {
        c.a = alpha;
        return c;
    }
}

// ======================================================
// Utility
// ======================================================

namespace UI
{
    std::string fmt(double v, int prec = 4);

    double safeStod(const std::string& s, double def = 0.0);

    void drawRect(
        sf::RenderWindow&,
        sf::FloatRect,
        sf::Color,
        sf::Color outline = sf::Color::Transparent,
        float thickness = 0.f);

    void drawText(
        sf::RenderWindow&,
        const sf::Font&,
        const std::string&,
        sf::Vector2f,
        sf::Color,
        unsigned,
        bool bold = false);

    void drawGradientRect(
        sf::RenderWindow&,
        sf::FloatRect,
        sf::Color,
        sf::Color);
}

// ======================================================
// Animated Cursor
// ======================================================

struct AnimCursor
{
    sf::Clock clock;

    bool visible() const
    {
        return clock.getElapsedTime().asMilliseconds() % 1000 < 500;
    }

    void reset()
    {
        clock.restart();
    }
};

// ======================================================
// Button
// ======================================================

struct Button
{
    sf::RectangleShape bg;

    std::string text;

    unsigned fontSize = 14;

    sf::Color accent;

    bool hovered = false;
    bool active = false;
    bool visible = true;

    Button() = default;

    Button(
        const std::string& text,
        sf::Vector2f pos,
        sf::Vector2f size,
        sf::Color accent,
        unsigned fontSize = 14);

    bool contains(sf::Vector2i mouse) const;

    void update(sf::Vector2i mouse);

    void draw(
        sf::RenderWindow&,
        const sf::Font&);
};

// ======================================================
// Input Box
// ======================================================

struct InputBox
{
    sf::RectangleShape bg;

    std::string value;

    std::string placeholder;

    unsigned charSz = 16;

    bool focused = false;

    bool visible = true;

    AnimCursor cursor;

    InputBox() = default;

    InputBox(
        const std::string& placeholder,
        sf::Vector2f pos,
        sf::Vector2f size,
        unsigned sz = 16);

    bool contains(sf::Vector2i mouse) const;

    void handleText(uint32_t unicode);

    void setFocus(bool focus);

    void updateFocus(
        sf::Vector2i mouse,
        bool clicked);

    void draw(
        sf::RenderWindow&,
        const sf::Font&);
};
// ======================================================
// History Entry
// ======================================================

struct HistEntry
{
    std::string eq;
    std::string op;
    std::string result;

    sf::Color col;
};

// ======================================================
// Graph
// ======================================================

struct Graph
{
    sf::FloatRect area;

    double xMin = -5.0;
    double xMax = 5.0;

    bool showDeriv = false;

    bool   showIntegRegion = false;
    double integA = 0.0;
    double integB = 1.0;

    bool crosshairActive = false;
    sf::Vector2f crosshairPos;

    ASTNode* fxNode = nullptr;
    ASTNode* dxNode = nullptr;

    void handleScroll(float delta);

    void draw(
        sf::RenderWindow&,
        const sf::Font&);

private:

    void drawIntegRegion(sf::RenderWindow& w, const sf::Font& font,
        double yMn, double yMx);

    float mapX(double x) const;

    float mapY(
        double y,
        double yMin,
        double yMax) const;

    void computeYRange(
        double& yMin,
        double& yMax);

    void plotCurve(
        sf::RenderWindow&,
        ASTNode* node,
        sf::Color color,
        double yMin,
        double yMax);
};
// ======================================================
// Calculator Application
// ======================================================

class CalcApp
{
public:

    CalcApp();

    void run();

private:

    //----------------------------------------------------
    // Window
    //----------------------------------------------------

    sf::RenderWindow window;
    sf::Font font;

    //----------------------------------------------------
    // Layout
    //----------------------------------------------------

    static constexpr unsigned W = 1340;
    static constexpr unsigned H = 800;

    static constexpr float PAD = 22.f;
    static constexpr float LW = 400.f;

    static constexpr float GX = LW + PAD * 2.f;
    static constexpr float GY = 68.f;

    //----------------------------------------------------
    // Modes
    //----------------------------------------------------

    enum class Mode
    {
        NONE,
        EVAL,
        DERIV,
        LIMIT,
        INTEG
    };

    enum class Step
    {
        SELECT_OP,
        ENTER_PARAMS,
        SHOW_RESULT
    };

    Mode mode = Mode::NONE;
    Step step = Step::SELECT_OP;

    //----------------------------------------------------
    // Calculation
    //----------------------------------------------------

    ASTNode* rootNode = nullptr;
    ASTNode* derivNode = nullptr;

    std::string res1;
    std::string res2;
    std::string res3;
    std::string errText;

    sf::Color resAccent = Pal::GREEN;

    std::vector<HistEntry> history;

    //----------------------------------------------------
    // Widgets
    //----------------------------------------------------

    InputBox eqBox;

    InputBox xBox;

    InputBox aBox;
    InputBox bBox;

    InputBox xMinBox;
    InputBox xMaxBox;

    Button btnEval;
    Button btnDeriv;
    Button btnLimit;
    Button btnInteg;

    Button btnCalc;
    Button btnToggleDeriv;

    //----------------------------------------------------
    // Graph
    //----------------------------------------------------

    Graph graph;

    //----------------------------------------------------
    // Tab Navigation
    //----------------------------------------------------

    std::vector<InputBox*> tabOrder;

    int tabIdx = 0;

    //----------------------------------------------------
    // Helpers
    //----------------------------------------------------

    void buildWidgets();

    void handleEvents();

    void handleClick(sf::Vector2i mouse);

    void runCalculation();

    void updateVisibility();

    void draw();

    //----------------------------------------------------
    // Drawing Helpers
    //----------------------------------------------------

    void drawHeader();

    void drawLeftPanel();

    void drawResultCard(float y);

    void drawHistory(float y);

    void drawStepHint();
};