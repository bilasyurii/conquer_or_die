#include "unitlist.h"
#include "resourcemanager.h"
#include "world.h"

UnitList::UnitList(unsigned _x, unsigned _y, unsigned _interval,
                   unsigned _columns, unsigned _count) :
    interval(_interval + 22), columns(_columns), count(_count)
{
    x = _x;
    y = _y;
    sf::Texture &t = ResourceManager::getInstance().getTexture();
    frames = new sf::RectangleShape[count];
    slots = new sf::Sprite[count];
    unitCount = new unsigned[count];
    labels = new Label[count];
    unsigned newX, newY;
    for(unsigned i = 0; i < count; ++i)
    {
        newX = x + interval * (i % columns);
        newY = y + interval * (i / columns);
        labels[i].setString("0");
        labels[i].setSize(5);
        labels[i].setColor(sf::Color::White);
        labels[i].setX(newX + 11);
        labels[i].setY(newY + 26);
        frames[i].setFillColor(sf::Color(120, 90, 60));
        frames[i].setSize(sf::Vector2f(22, 22));
        frames[i].setPosition(newX, newY);
        slots[i].setTexture(t);
        slots[i].setTextureRect(sf::IntRect(0, 0, 0, 0));
        slots[i].setPosition(newX + 1, newY + 1);
    }
    selection.setTexture(t);
    selection.setTextureRect(sf::IntRect(0, 20, 20, 20));
    selectSlot(0);
}

UnitList::~UnitList()
{
    delete frames;
    delete slots;
    delete unitCount;
    delete labels;
}

void UnitList::setX(unsigned _x)
{
    x = _x;
    unsigned newX, newY;
    for(unsigned i = 0; i < count; ++i)
    {
        newX = x + interval * (i % columns);
        newY = y + interval * (i / columns);
        frames[i].setPosition(newX, newY);
        slots[i].setPosition(newX + 1, newY + 1);
        selection.setPosition(x + 1 + interval * (activeSlot % columns),
                              y + 1 + interval * (activeSlot / columns));
        labels[i].setX(newX + 11);
    }
}

void UnitList::setY(unsigned _y)
{
    y = _y;
    unsigned newX, newY;
    for(unsigned i = 0; i < count; ++i)
    {
        newX = x + interval * (i % columns);
        newY = y + interval * (i / columns);
        frames[i].setPosition(newX, newY);
        slots[i].setPosition(newX + 1, newY + 1);
        selection.setPosition(x + 1 + interval * (activeSlot % columns ),
                              y + 1 + interval * (activeSlot / columns));
        labels[i].setY(newY + 26);
    }
}

void UnitList::hideSelection()
{
    hasSelection = false;
}

void UnitList::selectSlot(unsigned i)
{
    hasSelection = true;
    activeSlot = i;
    selection.setPosition(x + 1 + interval * (activeSlot % columns),
                          y + 1 + interval * (activeSlot / columns));
}

unsigned UnitList::getSelection() const
{
    return activeSlot;
}

void UnitList::next()
{
    ++activeSlot;
    if(activeSlot == count)
        activeSlot = 0;
    selectSlot(activeSlot);
}

void UnitList::prev()
{
    if(activeSlot == 0)
        activeSlot = count - 1;
    else
        --activeSlot;
    selectSlot(activeSlot);
}

void UnitList::nextNonempty()
{
    for(unsigned i = 0; i < count; ++i)
    {
        next();
        if(unitCount[activeSlot] != 0)
            break;
    }
}

void UnitList::clearSlot(unsigned i)
{
    slots[i].setTextureRect(sf::IntRect(0, 0, 0, 0));
    unitCount[i] = 0;
    labels[i].setString("0");
}

void UnitList::setSlot(unsigned i, Entity::Type t, unsigned _count)
{
    unitCount[i] = _count;
    labels[i].setString(World::unsignedToString(_count));
    switch(t)
    {
    case Entity::INFANTRY: slots[i].setTextureRect(sf::IntRect(0, 0, 20, 20)); break;
    case Entity::CAVALRY: slots[i].setTextureRect(sf::IntRect(20, 0, 20, 20)); break;
    case Entity::ARCHER: slots[i].setTextureRect(sf::IntRect(40, 0, 20, 20)); break;
    case Entity::SIEGE: slots[i].setTextureRect(sf::IntRect(60, 0, 20, 20)); break;
    case Entity::MAGE: slots[i].setTextureRect(sf::IntRect(80, 0, 20, 20)); break;
    default: return;
    }
}

void UnitList::setSlot(unsigned i, unsigned t, unsigned _count)
{
    unitCount[i] = _count;
    labels[i].setString(World::unsignedToString(_count));
    slots[i].setTextureRect(sf::IntRect(t * 20, 0, 20, 20));
}

void UnitList::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for(unsigned i = 0; i < count; ++i)
    {
        target.draw(frames[i], states);
        target.draw(slots[i], states);
    }
    if(hasSelection)
        target.draw(selection, states);
    for(unsigned i = 0; i < count; ++i)
        target.draw(labels[i]);
}

sf::Vector2f UnitList::getSlotCoords(unsigned index) const
{
    return sf::Vector2f(x + 1 + interval * (index % columns),
                        y + 1 + interval * (index / columns));
}
