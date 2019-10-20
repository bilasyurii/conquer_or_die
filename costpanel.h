#ifndef COSTPANEL_H
#define COSTPANEL_H

#include "element.h"
#include "imageelement.h"
#include "label.h"

class CostPanel : public Element
{
public:
    CostPanel();
    CostPanel(unsigned _x, unsigned _y);
    virtual ~CostPanel();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void setX(unsigned _x);
    virtual void setY(unsigned _y);
    virtual void setCount(unsigned c, unsigned i);
    virtual void setResourcesEnough(bool e, unsigned i);
protected:
    ImageElement *icons[4];
    Label *labels[4];
};

#endif // COSTPANEL_H
