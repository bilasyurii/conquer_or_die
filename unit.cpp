#include "unit.h"

Unit::Unit(Type t) : Entity()
{
    type = t;
    maxHp = hp = 100; //TODO
    damage = 10;
}

Unit::Unit(const Unit& u) : Entity(u) {}

Unit::~Unit() {}

Entity *Unit::clone() const
{
    return new Unit(*this);
}
