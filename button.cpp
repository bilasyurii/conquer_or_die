#include "button.h"
#include "gui.h"

Button::Button(unsigned _x, unsigned _y, unsigned w, unsigned h,
               std::string s, void (GUI::*a)(void)) :
    Element(_x, _y), width(w), height(h),
    str(s), action(a), enabled(true) {}

Button::Button() : Element(), width(0), height(0), enabled(true) {}

Button::~Button() {}

unsigned Button::getWidth() const
{
    return width;
}

unsigned Button::getHeight() const
{
    return height;
}

std::string Button::getString() const
{
    return str;
}

void Button::setAction(void (GUI::*a)(void))
{
    action = a;
}

void Button::setString(std::string s)
{
    str = s;
}

void Button::activate(GUI *gui) const
{
    if(enabled)
        (gui->*action)();
}

void Button::enable()
{
    enabled = true;
}

void Button::disable()
{
    enabled = false;
}

bool Button::isEnabled() const
{
    return enabled;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    static sf::RectangleShape rect;
    if(enabled)
    {
        GUI::text.setFillColor(sf::Color::Black);
        rect.setFillColor(sf::Color(214, 167, 96));
    }
    else
    {
        GUI::text.setFillColor(sf::Color(50, 50, 50));
        rect.setFillColor(sf::Color(180, 130, 60));
    }
    rect.setPosition(x, y);
    rect.setSize(sf::Vector2f(width, height));
    target.draw(rect, states);
    GUI::text.setString(str);
    GUI::text.setCharacterSize(8);
    GUI::text.setOrigin((int) (GUI::text.getLocalBounds().width * 0.5f),
                        (int) (GUI::text.getLocalBounds().top +
                               GUI::text.getLocalBounds().height * 0.5f));
    GUI::text.setPosition(x + width * 0.5f, y + height * 0.5f);
    target.draw(GUI::text, states);
}

