#include "progressbar.h"

ProgressBar::ProgressBar() : Element() {}

ProgressBar::ProgressBar(int x, int y, int w, int h, float f) :
        Element(x, y), width(w), height(h)
{
    background.setPosition(x, y);
    background.setSize(sf::Vector2f(width, height));
    background.setFillColor(sf::Color(50, 50, 50));
    foreground.setFillColor(sf::Color(0, 150, 0));
    foreground.setPosition(x + 2.0f, y + 2.0f);
    setFactor(f);
}

ProgressBar::~ProgressBar() {}

void ProgressBar::setFactor(float f)
{
    factor = f;
    foreground.setSize(sf::Vector2f(factor * (width - 4.0f), height - 4.0f));
}

void ProgressBar::setX(unsigned _x)
{
    x = _x;
    background.setPosition(x, y);
    foreground.setPosition(x + 2.0f, y + 2.0f);
}

void ProgressBar::setY(unsigned _y)
{
    y = _y;
    background.setPosition(x, y);
    foreground.setPosition(x + 2.0f, y + 2.0f);
}

void ProgressBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(background, states);
    target.draw(foreground, states);
}
