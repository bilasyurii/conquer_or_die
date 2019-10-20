#include "label.h"
#include "gui.h"
#include "resourcemanager.h"

Label::Label() : Label(0, 0, "", CENTER, sf::Color::Black) {}

Label::Label(unsigned _x, unsigned _y, std::string s, TextAlignment t,
             sf::Color c, unsigned size) : Element(_x, _y), alignment(t)
{
    text.setFont(ResourceManager::getInstance().getFont());
    text.setPosition(x, y);
    text.setString(s);
    text.setFillColor(c);
    text.setCharacterSize(size);
    switch(alignment)
    {
    case LEFT:
        text.setOrigin(0, (int) (text.getLocalBounds().top +
                                 text.getLocalBounds().height * 0.5f));
        break;
    case CENTER:
        text.setOrigin((int) (text.getLocalBounds().width * 0.5f),
                       (int) (text.getLocalBounds().top +
                              text.getLocalBounds().height * 0.5f));
        break;
    case RIGHT:
        text.setOrigin((int) (text.getLocalBounds().width),
                       (int) (text.getLocalBounds().top +
                              text.getLocalBounds().height * 0.5f));
        break;
    }
}

Label::~Label() {}

void Label::setX(unsigned _x)
{
    x = _x;
    text.setPosition(x, y);
}

void Label::setY(unsigned _y)
{
    y = _y;
    text.setPosition(x, y);
}

void Label::setSize(unsigned size)
{
    text.setCharacterSize(size);
    switch(alignment)
    {
    case LEFT:
        text.setOrigin(0, (int) (text.getLocalBounds().top +
                                 text.getLocalBounds().height * 0.5f));
        break;
    case CENTER:
        text.setOrigin((int) (text.getLocalBounds().width * 0.5f),
                       (int) (text.getLocalBounds().top +
                              text.getLocalBounds().height * 0.5f));
        break;
    case RIGHT:
        text.setOrigin((int) (text.getLocalBounds().width),
                       (int) (text.getLocalBounds().top +
                              text.getLocalBounds().height * 0.5f));
        break;
    }
}

void Label::setColor(sf::Color c)
{
    text.setFillColor(c);
}

void Label::setAlignment(TextAlignment t)
{
    alignment = t;
    switch(alignment)
    {
    case LEFT:
        text.setOrigin(0, (int) (text.getLocalBounds().top +
                                 text.getLocalBounds().height * 0.5f));
        break;
    case CENTER:
        text.setOrigin((int) (text.getLocalBounds().width * 0.5f),
                       (int) (text.getLocalBounds().top +
                              text.getLocalBounds().height * 0.5f));
        break;
    case RIGHT:
        text.setOrigin((int) (text.getLocalBounds().width),
                       (int) (text.getLocalBounds().top +
                              text.getLocalBounds().height * 0.5f));
        break;
    }
}

void Label::setString(std::string s)
{
    text.setString(s);
    switch(alignment)
    {
    case LEFT:
        text.setOrigin(0, (int) (text.getLocalBounds().top +
                                 text.getLocalBounds().height * 0.5f));
        break;
    case CENTER:
        text.setOrigin((int) (text.getLocalBounds().width * 0.5f),
                       (int) (text.getLocalBounds().top +
                              text.getLocalBounds().height * 0.5f));
        break;
    case RIGHT:
        text.setOrigin((int) (text.getLocalBounds().width),
                       (int) (text.getLocalBounds().top +
                              text.getLocalBounds().height * 0.5f));
        break;
    }
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(text, states);
}
