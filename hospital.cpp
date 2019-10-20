#include "hospital.h"
#include "world.h"
#include "player.h"

Hospital::Hospital() : Building(10, 2, 25.0f)
{
    type = HOSPITAL;
    buildingCosts[0] = 2000;
    buildingCosts[1] = 1000;
    buildingCosts[2] = 500;
}

Hospital::Hospital(const Hospital& b) : Building(b) {}

Hospital::~Hospital() {}

Entity *Hospital::clone() const
{
    return new Hospital(*this);
}

void Hospital::update(float delta)
{
    Building::update(delta);
}

void Hospital::buildingFinished()
{
    World::getInstance().getPlayer(id)->setHospital(this);
}
