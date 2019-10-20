#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "progressbar.h"

class HealthBar : public ProgressBar
{
public:
    HealthBar();
    HealthBar(int x, int y, int w, int h, float f = 1.0f);
    virtual ~HealthBar();
    virtual void setFactor(float f);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // HEALTHBAR_H
