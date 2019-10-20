#include "city.h"

City::City() : Building(0, 3, 0.0f), Defender(this, 2)
{
    type = CITY;
    hp = 2000;//TODO another value
    //or value which depends on another factor or can be upgraded
    //or ability to set some troops to defend city
    damage = 1000;
    built = true;
}

City::City(const City& c) : Building(c),
    Defender(this, c.getRadius()) {}

City::~City() {}

void City::update(float delta)
{
    Defender::update(delta);
}

Entity *City::clone() const
{
    return new City(*this);
}

void City::buildingFinished() {}
