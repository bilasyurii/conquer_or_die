#ifndef IMAGEELEMENT_H
#define IMAGEELEMENT_H

#include "element.h"
#include <SFML/Graphics.hpp>

class ImageElement : public Element
{
protected:
    sf::Sprite img;
public:
    ImageElement();
    ImageElement(unsigned x, unsigned y, sf::Sprite s);
    virtual ~ImageElement();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void setX(unsigned _x);
    virtual void setY(unsigned _y);
    virtual void setSprite(sf::Sprite s);
    virtual void setTextureRect(sf::IntRect r);
};

#endif // IMAGEELEMENT_H
