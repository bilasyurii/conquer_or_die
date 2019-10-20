#include "defender.h"
#include "entity.h"
#include "world.h"

Defender::Defender(Entity *p, unsigned r) :
        parent(p), radius(r), state(IDLE),
        attackCoef(0.0f), targetEntity(nullptr) {}

Defender::~Defender() {}

void Defender::update(float delta)
{
    Building *b = dynamic_cast<Building*>(parent);
    if(b && !b->isBuilt())
        return;
    if(state == IDLE)
    {
        targetEntity = World::getInstance().getNeighbour(parent->getX(),
                parent->getY(), radius, parent->getId());
        if(targetEntity != nullptr)
            setState(FIGHT);
    }
    else
    {
        if(!targetEntity->isAlive() ||
           World::ManhattanDistance(targetEntity, parent) > radius)
        {
            targetEntity = World::getInstance().getNeighbour(parent->getX(),
                    parent->getY(), radius, parent->getId());//TODO maybe get rid of getNeighbour and use findClosest
            if(targetEntity == nullptr)
                setState(IDLE);
            else
                setState(FIGHT);
        }
    }
    if(state == FIGHT)
    {
        attackCoef += 100.0f * delta;
        if(attackCoef >= 50.0f)
        {
            targetEntity->takeDamage(parent->getDamage());
            if(targetEntity->isAlive())
                attackCoef -= 50.0f;
            else
                setState(IDLE);
        }
    }
}

void Defender::setState(DefendState s)
{
    state = s;
    attackCoef = 0.0f;
    if(state == IDLE)
        targetEntity = nullptr;
    else
    {
        targetX = targetEntity->getX();
        targetY = targetEntity->getY();
    }
}

unsigned Defender::getRadius() const
{
    return radius;
}
