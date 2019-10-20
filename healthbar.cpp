#include "healthbar.h"

HealthBar::HealthBar() : ProgressBar() {}

HealthBar::HealthBar(int x, int y, int w, int h, float f) : ProgressBar(x, y, w, h, f) {}

HealthBar::~HealthBar() {}

void HealthBar::setFactor(float f)
{
    ProgressBar::setFactor(f);
    if(f >= 0.5f)
        foreground.setFillColor(sf::Color(0, 150, 0));
    else if(f >= 0.25)
        foreground.setFillColor(sf::Color(200, 150, 0));
    else
        foreground.setFillColor(sf::Color(200, 0, 0));
}

void HealthBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    ProgressBar::draw(target, states);
}
