#include "tower.h"

Tower::Tower() : Building(2, 3, 30.0f), Defender(this, 1)
{
    type = TOWER;
    hp = 500;
    damage = 500;//TODO
    buildingCosts[0] = 1500;
    buildingCosts[1] = 2000;
    buildingCosts[2] = 500;
}

Tower::Tower(const Tower& t) : Building(t),
    Defender(this, t.getRadius())
{
    type = TOWER;
}

Tower::~Tower() {}

Entity *Tower::clone() const
{
    return new Tower(*this);
}

void Tower::update(float delta)
{
    Building::update(delta);
    Defender::update(delta);
}

void Tower::buildingFinished()
{
    //TODO
}
