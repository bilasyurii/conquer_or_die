#include "resourcepoint.h"

ResourcePoint::ResourcePoint(Type t, unsigned i) :
    Building(1, 2, 10.0f), income(i)
{
    type = t;
    switch(type)
    {
    case Entity::FARM:
        hp = 200;
        buildingCosts[0] = 500;
        buildingCosts[1] = 1000;//total 4000
        break;
    case Entity::LUMBERMILL:
        hp = 300;
        buildingCosts[0] = 900;
        buildingCosts[1] = 1400;//total 6000
        break;
    case Entity::QUARRY:
        hp = 400;
        buildingCosts[0] = 1000;
        buildingCosts[1] = 2000;//total 8000
        break;
    case Entity::GOLDMINE:
        hp = 500;
        buildingCosts[0] = 1000;
        buildingCosts[1] = 2400;
        buildingCosts[2] = 200;//total 10000
        break;
    default:
        break;
    }
}

ResourcePoint::ResourcePoint(const ResourcePoint& r) :
    Building(r), income(r.income) {}

ResourcePoint::~ResourcePoint() {}

unsigned ResourcePoint::getResourceIncome() const
{
    return income;
}

Entity *ResourcePoint::clone() const
{
    return new ResourcePoint(*this);
}

void ResourcePoint::update(float delta)
{
    Building::update(delta);
}

void ResourcePoint::buildingFinished()
{
    //TODO
}
