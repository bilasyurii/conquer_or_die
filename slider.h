#ifndef SLIDER_H
#define SLIDER_H

#include "button.h"
#include <SFML/Graphics.hpp>

class Slider : public Button
{
public:
    Slider(unsigned _x, unsigned _y, unsigned _width,
           unsigned _min, unsigned _max, unsigned _step);
    virtual ~Slider();

    virtual void setX(unsigned _x);
    virtual void setY(unsigned _y);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void reset();
    virtual void increase(bool faster = false);
    virtual void decrease(bool faster = false);
    virtual void setValue(unsigned v);
    virtual unsigned getValue() const;
    virtual void activate(GUI* gui) const;
    virtual void setAction(void(GUI::* a)(void));
    virtual void setString(std::string s);
    virtual void setMinMax(unsigned _min, unsigned _max);
    virtual std::string getString() const;
private:
    unsigned lineX, lineY, lineWidth, min, max, step;
    int value;
    sf::RectangleShape line;
    sf::RectangleShape indicator;
};

#endif // SLIDER_H
