#ifndef TOWER_H
#define TOWER_H

#include "building.h"
#include "defender.h"

class Tower : public Building, public Defender
{
public:
    Tower();
    Tower(const Tower &t);
    virtual ~Tower();
    virtual Entity *clone() const;
    virtual void update(float delta);
    virtual void buildingFinished();
};

#endif // TOWER_H
