#ifndef UNITGROUP_H
#define UNITGROUP_H

#include "controllable.h"
#include <utility>

class Unit;
class World;

class UnitGroup : public Controllable
{
public:
    UnitGroup(unsigned _id, Type t, unsigned c);
    UnitGroup(const UnitGroup &g);
    virtual void merge(const UnitGroup &g);
    virtual ~UnitGroup();
    virtual unsigned getCount() const;
    virtual Entity *clone() const;
    virtual UnitGroup *split(unsigned powerPercent);
    virtual std::pair<UnitGroup*, UnitGroup*> getParts(unsigned powerPercent);
    virtual void setState(UnitState s);
    virtual unsigned heal(unsigned amount);
    virtual unsigned takeDamage(unsigned dmg);
protected:
    std::vector<Unit*> units;
    unsigned count;
    virtual void addUnit(Unit *u);
};

#endif // UNITGROUP_H



