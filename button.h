#ifndef BUTTON_H
#define BUTTON_H

#include <vector>
#include <string>
#include "element.h"
#include <SFML/Graphics.hpp>

class GUI;

class Button : public Element
{
public:
    Button();
    Button(unsigned _x, unsigned _y, unsigned w, unsigned h,
           std::string s, void (GUI::*a)(void));
    virtual ~Button();

    unsigned getWidth() const;
    unsigned getHeight() const;
    virtual std::string getString() const;
    virtual void setAction(void (GUI::*a)(void));
    virtual void setString(std::string s);
    virtual void activate(GUI *gui) const;
    void enable();
    void disable();
    bool isEnabled() const;
protected:
    unsigned width, height;
    std::string str;
    void (GUI::*action)(void);
    bool enabled;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // BUTTON_H




