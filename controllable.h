#ifndef CONTROLLABLE_H
#define CONTROLLABLE_H

#include "entity.h"
#include <SFML/Graphics.hpp>

class World;
class Path;
class GUI;

class Controllable : public Entity
{
public:
    enum UnitState
    {
        STAND,
        MOVE_TO_TARGET_ENTITY,
        MOVE_TO_TARGET_AREA,
        FIGHT,
        DEFEND
    };

    Controllable();
    Controllable(const Controllable &c);
    virtual ~Controllable();

    virtual void update(float delta);
    Path* getPath() const;
    bool isMoving() const;
    void setMoving(bool m);
    UnitState getState() const;
    void setTarget(Entity *t);
    virtual void setState(UnitState s, bool d = false);
    virtual Entity *clone() const;
private:
    UnitState state;
    Entity *targetEntity;
    Path *path;
    bool moving;
    bool defending;
    float actionCoef;
    unsigned targetX, targetY;

    void updatePath();
};

#endif // CONTROLLABLE_H


