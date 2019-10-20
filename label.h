#ifndef LABEL_H
#define LABEL_H

#include <string>
#include "element.h"
#include <SFML/Graphics.hpp>

class Label : public Element
{
public:
    enum TextAlignment
    {
        LEFT, CENTER, RIGHT
    };
    Label();
    Label(unsigned _x, unsigned _y, std::string s, TextAlignment t,
          sf::Color c, unsigned size = 10);
    virtual ~Label();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void setString(std::string s);
    virtual void setColor(sf::Color c);
    virtual void setAlignment(TextAlignment t);
    virtual void setSize(unsigned size);
    virtual void setX(unsigned _x);
    virtual void setY(unsigned _y);
private:
    TextAlignment alignment;
    sf::Text text;
};

#endif // LABEL_H
