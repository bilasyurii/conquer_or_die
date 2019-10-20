#ifndef BARRACKS_H
#define BARRACKS_H

#include "building.h"

class Barracks : public Building
{
protected:
    bool training[5];
    unsigned trainQueuesCount;
    float trainTime;
    unsigned trainCount[5];
    unsigned progress[5];
    unsigned trainingCosts[5][4] =
    {
        //F=2  W=3  S=4  G=5
        { 110,  60,   0,   0 }, // I = 400
        { 140,  40,   0,  20 }, // C = 500
        {  90,  60,  10,   0 }, // A = 400
        { 110,  70,  30,   0 }, // S = 550
        {  90,   0,  30,  60 }  // M = 600
    };
    // it isn't const 'cause i would like
    // to have different training costs for diff. heroes
public:
    Barracks();
    Barracks(const Barracks &b);
    virtual ~Barracks();
    virtual Entity *clone() const;
    virtual void setTrainCount(Entity::Type type, unsigned amount);
    virtual void increaseTrainCount(Entity::Type type, unsigned amount);
    virtual void decreaseTrainCount(Entity::Type type, unsigned amount);
    virtual unsigned getTrainCount(Entity::Type type);
    virtual unsigned getProgress(Entity::Type type);
    virtual float getFactor(Entity::Type type);
    virtual float getFactor(unsigned i);
    virtual unsigned getTrainCost(Entity::Type e, ResourceType r);
    virtual void update(float delta);
    virtual void buildingFinished();
    virtual void startTraining(Entity::Type type);
};

#endif // BARRACKS_H
