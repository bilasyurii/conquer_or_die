#include "tradepost.h"
#include "world.h"
#include "player.h"

Tradepost::Tradepost() : Building(5, 2, 5.0f)//TODO make building time bigger - maybe 30.0f
{
    type = TRADEPOST;
    buildingCosts[0] = 1000;
    buildingCosts[1] = 1000;
    buildingCosts[3] = 150;
}

Tradepost::Tradepost(const Tradepost& b) : Building(b) {}

Tradepost::~Tradepost() {}

Entity *Tradepost::clone() const
{
    return new Tradepost(*this);
}

void Tradepost::update(float delta)
{
    Building::update(delta);
}

void Tradepost::buildingFinished()
{
    World::getInstance().getPlayer(id)->setTradepost(this);
}
