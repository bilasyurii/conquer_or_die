#include "element.h"

Element::Element() : x(0), y(0), visible(true) {}

Element::Element(unsigned _x, unsigned _y) : x(_x), y(_y), visible(true) {}

Element::~Element() {}

void Element::setX(unsigned _x)
{
    x = _x;
}

void Element::setY(unsigned _y)
{
    y = _y;
}

unsigned Element::getX() const
{
    return x;
}

unsigned Element::getY() const
{
    return y;
}

bool Element::isVisible() const
{
    return visible;
}

void Element::setVisibility(bool v)
{
    visible = v;
}
