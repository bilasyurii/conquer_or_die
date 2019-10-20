#include "barracks.h"
#include "world.h"
#include "player.h"
#include "gui.h"

Barracks::Barracks() : Building(10, 2, 10.0f),
        trainQueuesCount(0), trainTime(0.0f)
{
    type = BARRACKS;
    for(unsigned i = 0; i < 5; ++i)
    {
        trainCount[i] = 10;
        progress[i] = 0;
        training[i] = false;
    }
    buildingCosts[0] = 1000;
    buildingCosts[1] = 1000;
}

Barracks::Barracks(const Barracks& b) : Building(b),
        trainQueuesCount(0), trainTime(0.0f)
{
    for(unsigned i = 0; i < 5; ++i)
    {
        trainCount[i] = 10;
        progress[i] = 0;
        training[i] = false;
    }
}

Barracks::~Barracks() {}

Entity *Barracks::clone() const
{
    return new Barracks(*this);
}

void Barracks::startTraining(Entity::Type type)
{
    unsigned index = World::typeToIndex(type);
    training[index] = true;
    ++trainQueuesCount;
    progress[index] = 0;
}

void Barracks::setTrainCount(Entity::Type type, unsigned amount)
{
    trainCount[World::typeToIndex(type)] = amount;
}

void Barracks::increaseTrainCount(Entity::Type type, unsigned amount)
{
    trainCount[World::typeToIndex(type)] += amount;
}

void Barracks::decreaseTrainCount(Entity::Type type, unsigned amount)
{
    trainCount[World::typeToIndex(type)] -= amount;
}

unsigned Barracks::getTrainCount(Entity::Type type)
{
    return trainCount[World::typeToIndex(type)];
}

unsigned Barracks::getProgress(Entity::Type type)
{
    return progress[World::typeToIndex(type)];
}

float Barracks::getFactor(Entity::Type type)
{
    unsigned index = World::typeToIndex(type);
    return progress[index] / (float) trainCount[index];
}

float Barracks::getFactor(unsigned i)
{
    return progress[i] / (float) trainCount[i];
}

unsigned Barracks::getTrainCost(Entity::Type e, ResourceType r)
{
    unsigned index = World::typeToIndex(e);
    return trainingCosts[index][(unsigned) r] * trainCount[index];
}

void Barracks::buildingFinished()
{
    World::getInstance().getPlayer(id)->setBarracks(this);
}

void Barracks::update(float delta)
{
    Building::update(delta);
    if(built && trainQueuesCount > 0)
    {
        trainTime += delta;
        if(trainTime >= 1.0f)
        {
            for(unsigned i = 0; i < 5; ++i) {
                ++progress[i];
                if(training[i])
                {
                    if(progress[i] == trainCount[i])
                    {
                        training[i] = false;
                        --trainQueuesCount;
                        World::getInstance().getGUI(id)->trainFinishedNotify(i);
                    }
                    else
                        World::getInstance().getGUI(id)->trainProgressNotify(i, getFactor(i));
                }

            }
            trainTime -= 1.0f;
        }
    }
}
