#include "controllable.h"
#include "path.h"
#include "world.h"
#include "textparticleemitter.h"

Controllable::Controllable() : Entity(), state(STAND), targetEntity(nullptr),
        moving(false), defending(false), actionCoef(0.0f)
{
    path = new Path();
}

Controllable::Controllable(const Controllable& c) : Entity(c),
    state(STAND), targetEntity(nullptr), moving(false), actionCoef(0.0f) {}

Controllable::~Controllable() {}

Path* Controllable::getPath() const
{
    return path;
}

bool Controllable::isMoving() const
{
    return moving;
}

void Controllable::setMoving(bool m)
{
    moving = m;
    if(!moving)
        actionCoef = 0.0f;
}

void Controllable::update(float delta)
{
    Entity::update(delta);
    static sf::Vector2u nextNode;
    if(defending && state == STAND)
    {
        targetEntity = World::getInstance().findClosest(x, y, World::ENEMY_UNIT, id);
        if(World::ChebyshevDistance(this, targetEntity) > 3) //TODO maybe use Manhattan?
            targetEntity = nullptr;
        else
            setState(MOVE_TO_TARGET_ENTITY, true);
    }
    else if((state == MOVE_TO_TARGET_ENTITY || state == FIGHT) && !targetEntity->isAlive())
        setState(STAND, defending);
    actionCoef += 100.0f * delta; //1s=100.0f
    if(actionCoef >= 50.0f)
    {
        if(moving)
        {
            if(state == MOVE_TO_TARGET_ENTITY)
                updatePath();
            nextNode = path->next();
            if(!World::getInstance().isWalkable(nextNode.x, nextNode.y))
            {
                path->setStart(x, y);
                if(!path->recalculate(state == MOVE_TO_TARGET_ENTITY))
                    setState(STAND, defending);
                else
                {
                    nextNode = path->next();
                    if(state == MOVE_TO_TARGET_ENTITY &&
                       nextNode.x == targetX && nextNode.y == nextNode.y)
                        setState(FIGHT, defending);
                    else
                    {
                        x = nextNode.x;
                        y = nextNode.y;
                    }
                }
            }
            else
            {
                x = nextNode.x;
                y = nextNode.y;
            }
            if(path->isEmpty() && state != FIGHT)
                setState(STAND, defending);
            else
                actionCoef -= 50.0f;
        }
        else
        {
            if(state == FIGHT)
            {
                if(targetEntity->getX() == targetX &&
                   targetEntity->getY() == targetY)
                {
                    World::getInstance().getEmitter().emitDamage(World::unsignedToString(damage),
                            targetX * World::TILE_SIZE + World::MENU_WIDTH,
                            targetY * World::TILE_SIZE);
                    targetEntity->takeDamage(damage);//TODO
                    if(targetEntity->isAlive())
                        actionCoef -= 50.0f;
                    else
                    {
                        World::getInstance().getGUI(3 - id)->deathNotify(targetEntity);
                        World::getInstance().getGUI(id)->deathNotify(targetEntity);
                        setState(STAND, defending);
                    }
                }
                else
                    setState(MOVE_TO_TARGET_ENTITY, defending);
            }
        }
    }
}

void Controllable::setState(UnitState s, bool d)
{
    state = s;
    actionCoef = 0.0f;
    defending = d;
    if(state == STAND)
    {
        path->reset();
        setMoving(false);
        targetEntity = nullptr;
    }
    else if(state == MOVE_TO_TARGET_AREA)
    {
        targetEntity = nullptr;
        setMoving(path->recalculate());
        if(!moving)
            setState(STAND);
    }
    else if(state == DEFEND)
        setState(MOVE_TO_TARGET_AREA, true);
    else if(state == MOVE_TO_TARGET_ENTITY)
    {
        moving = true;
        targetX = targetEntity->getX() + 1;
        targetY = targetEntity->getY() + 1;
        updatePath();
    }
    else if(state == FIGHT)
        setMoving(false);
}

void Controllable::updatePath()
{
    if(targetEntity->getX() != targetX || targetEntity->getY() != targetY)
    {
        targetX = targetEntity->getX();
        targetY = targetEntity->getY();
        path->setEnd(targetX, targetY);
        path->setStart(x, y);
        if(!path->recalculate(true))
        {
            setMoving(false);
            path->reset();
        }
    }
}

Controllable::UnitState Controllable::getState() const
{
    if(defending)
        return DEFEND;
    return state;
}

void Controllable::setTarget(Entity* t)
{
    targetEntity = t;
}

Entity *Controllable::clone() const
{
    return new Controllable(*this);
}
