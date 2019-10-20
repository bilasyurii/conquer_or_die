#ifndef ELEMENT_H
#define ELEMENT_H

#include <SFML/Graphics.hpp>

class Element : public sf::Drawable
{
public:
    Element();
    Element(unsigned _x, unsigned _y);
    virtual ~Element();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
    virtual void setX(unsigned _x);
    virtual void setY(unsigned _y);
    virtual unsigned getX() const;
    virtual unsigned getY() const;
    bool isVisible() const;
    void setVisibility(bool v);
protected:
    unsigned x, y;
    bool visible;
};

#endif // ELEMENT_H
