#include "building.h"
#include "world.h"

Building::Building(float br, unsigned ir, float buildTime) :
    Entity(), buildRadius(br), influenceRadius(ir), built(false),
    timeToBuild(buildTime), buildProgress(0.0f) {
    for(unsigned i = 0; i < 4; ++i)
        buildingCosts[i] = 0;
}

Building::Building(const Building& b) : Entity(b), buildRadius(b.buildRadius),
    influenceRadius(b.influenceRadius), built(b.built),
    timeToBuild(b.timeToBuild), buildProgress(b.buildProgress)
{
    for(unsigned i = 0; i < 4; ++i)
        buildingCosts[i] = b.buildingCosts[i];
}

Building::~Building() {}

unsigned Building::getBuildingCost(ResourceType r)
{
    return buildingCosts[(unsigned) r];
}

void Building::setBuildRadius(float br)
{
    buildRadius = br;
}

void Building::setInfluenceRadius(unsigned ir)
{
    influenceRadius = ir;
}

float Building::getBuildRadius() const
{
    return buildRadius;
}

unsigned Building::getInfluenceRadius() const
{
    return influenceRadius;
}

float Building::getBuildFactor() const
{
    return buildProgress / timeToBuild;
}

bool Building::isBuilt() const
{
    return built;
}

void Building::update(float delta)
{
    if(built)
        return;
    buildProgress += delta;
    if(buildProgress >= timeToBuild)
    {
        built = true;
        buildingFinished();
        World::getInstance().finishBuilding(this);
    }
}
