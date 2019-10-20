#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include "element.h"

class ProgressBar : public Element
{
public:
    ProgressBar();
    ProgressBar(int x, int y, int w, int h, float f = 1.0f);
    virtual ~ProgressBar();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void setFactor(float f);
    virtual void setX(unsigned _x);
    virtual void setY(unsigned _y);
protected:
    int width, height;
    float factor;
    sf::RectangleShape background;
    sf::RectangleShape foreground;
};

#endif // PROGRESSBAR_H
