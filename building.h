#ifndef BUILDING_H
#define BUILDING_H

#include "entity.h"

class Building : public Entity
{
public:
    Building(float br, unsigned ir, float buildTime);
    Building(const Building &b);
    virtual ~Building();

    void setBuildRadius(float br);
    void setInfluenceRadius(unsigned ir);
    float getBuildRadius() const;
    unsigned getInfluenceRadius() const;
    virtual void update(float delta);
    virtual Entity *clone() const = 0;
    bool isBuilt() const;
    virtual float getBuildFactor() const;
    virtual unsigned getBuildingCost(ResourceType r);
protected:
    float buildRadius;
    unsigned influenceRadius;
    bool built;
    float timeToBuild;
    float buildProgress;
    unsigned buildingCosts[4];

    virtual void buildingFinished() = 0;
};

#endif // BUILDING_H



