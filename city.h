#ifndef CITY_H
#define CITY_H

#include "building.h"
#include "defender.h"

class City : public Building, public Defender
{
public:
    City();
    City(const City &c);
    virtual ~City();
    virtual Entity *clone() const;
    virtual void update(float delta);
    virtual void buildingFinished();
};

#endif // CITY_H



