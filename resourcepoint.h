#ifndef RESOURCEPOINT_H
#define RESOURCEPOINT_H

#include "building.h"

class ResourcePoint : public Building
{
public:
    ResourcePoint(Type t, unsigned i);
    ResourcePoint(const ResourcePoint &r);
    virtual ~ResourcePoint();
    unsigned getResourceIncome() const;
    virtual Entity *clone() const;
    virtual void update(float delta);
    virtual void buildingFinished();
private:
    unsigned income;
};

#endif // RESOURCEPOINT_H
