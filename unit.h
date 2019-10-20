#ifndef UNIT_H
#define UNIT_H

#include "entity.h"

class Unit : public Entity
{
public:
    Unit(Type t);
    Unit(const Unit &u);
    virtual ~Unit();

    virtual Entity *clone() const;
private:
};

#endif // UNIT_H


