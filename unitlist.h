#ifndef UNITLIST_H
#define UNITLIST_H

#include "element.h"
#include "label.h"
#include "entity.h"
#include <SFML/Graphics.hpp>

class UnitList : public Element
{
public:
    UnitList(unsigned _x, unsigned _y, unsigned _interval,
             unsigned _columns, unsigned _count);
    virtual ~UnitList();
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void setX(unsigned _x);
    virtual void setY(unsigned _y);
    virtual void selectSlot(unsigned i);
    virtual unsigned getSelection() const;
    virtual void hideSelection();
    virtual void setSlot(unsigned i, Entity::Type t, unsigned _count);
    virtual void setSlot(unsigned i, unsigned t, unsigned _count);
    virtual void clearSlot(unsigned i);
    virtual void next();
    virtual void prev();
    virtual void nextNonempty();
    virtual sf::Vector2f getSlotCoords(unsigned index) const;
private:
    unsigned interval;
    unsigned columns;
    unsigned count;
    sf::RectangleShape *frames;
    sf::Sprite *slots;
    unsigned *unitCount;
    Label *labels;
    sf::Sprite selection;
    unsigned activeSlot;
    bool hasSelection;
    //TODO i can have rect for blinking red square on top of slot to show that it gained damage
};

#endif // UNITLIST_H
