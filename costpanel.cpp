#include "costpanel.h"
#include "resourcemanager.h"
#include "world.h"

CostPanel::CostPanel() : Element()
{
    sf::Texture &t = ResourceManager::getInstance().getTexture();
    icons[0] = new ImageElement(0,  0, sf::Sprite(t, sf::IntRect(0, 100, 10, 10)));
    icons[1] = new ImageElement(0, 20, sf::Sprite(t, sf::IntRect(10, 100, 10, 10)));
    icons[2] = new ImageElement(0, 40, sf::Sprite(t, sf::IntRect(20, 100, 10, 10)));
    icons[3] = new ImageElement(0, 60, sf::Sprite(t, sf::IntRect(30, 100, 10, 10)));
    labels[0] = new Label(15,  5, "", Label::LEFT, sf::Color::White);
    labels[1] = new Label(15, 25, "", Label::LEFT, sf::Color::White);
    labels[2] = new Label(15, 45, "", Label::LEFT, sf::Color::White);
    labels[3] = new Label(15, 65, "", Label::LEFT, sf::Color::White);
}

CostPanel::CostPanel(unsigned _x, unsigned _y) : CostPanel()
{
    setX(_x);
    setY(_y);
}

CostPanel::~CostPanel() {}

void CostPanel::setX(unsigned _x)
{
    x = _x;
    for(unsigned i = 0; i < 4; ++i)
    {
        icons[i]->setX(x);
        labels[i]->setX(x + 15);
    }
}

void CostPanel::setY(unsigned _y)
{
    y = _y;
    for(unsigned i = 0, coord; i < 4; ++i)
    {
        coord = y + i * 20;
        icons[i]->setY(coord);
        labels[i]->setY(coord + 5);
    }
}

void CostPanel::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for(unsigned i = 0; i < 4; ++i)
        target.draw(*(icons[i]), states);
    for(unsigned i = 0; i < 4; ++i)
        target.draw(*(labels[i]), states);
}

void CostPanel::setCount(unsigned c, unsigned i)
{
    labels[i]->setString(World::unsignedToString(c));
}

void CostPanel::setResourcesEnough(bool e, unsigned i)
{
    if(e)
        labels[i]->setColor(sf::Color::White);
    else
        labels[i]->setColor(sf::Color::Red);
}
