#include "imageelement.h"

ImageElement::ImageElement() : Element() {}

ImageElement::ImageElement(unsigned x, unsigned y, sf::Sprite s) :
        Element(x, y), img(s)
{
    img.setPosition(x, y);
}

void ImageElement::setX(unsigned _x)
{
    x = _x;
    img.setPosition(x, y);
}

void ImageElement::setY(unsigned _y)
{
    y = _y;
    img.setPosition(x, y);
}

void ImageElement::setSprite(sf::Sprite s)
{
    img = s;
}

void ImageElement::setTextureRect(sf::IntRect r)
{
    img.setTextureRect(r);
}

ImageElement::~ImageElement() {}

void ImageElement::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(img, states);
}
