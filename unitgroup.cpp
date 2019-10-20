#include "unitgroup.h"
#include "unit.h"
#include "world.h"
#include <math.h>

UnitGroup::UnitGroup(unsigned _id, Type t, unsigned c) : Controllable(), count(c)
{
    id = _id;
    type = t;
    for(unsigned i = 0; i < count; ++i)
        units.push_back(new Unit(t));
    maxHp = hp = World::getInstance().getPrototype(t)->getHp() * count;
    damage = World::getInstance().getPrototype(t)->getDamage() * count;
}

UnitGroup::UnitGroup(const UnitGroup& g) : Controllable(g), count(g.count)
{
    for(unsigned i = 0; i < count; ++i)
        units.push_back(static_cast<Unit*>(g.units[i]->clone()));
}

UnitGroup::~UnitGroup()
{
    for(unsigned i = 0; i < units.size(); ++i)
        delete units[i];
}

void UnitGroup::merge(const UnitGroup& g)
{
    unsigned i = 0, j = 0;
    std::vector<Unit*> current = std::vector<Unit*>(units);
    units = std::vector<Unit*>();
    while(i < count && j < g.count)
    {
        if(current[i]->getHp() >= g.units[j]->getHp())
        {
            units.push_back(current[i]);
            ++i;
        }
        else
        {
            units.push_back(static_cast<Unit*>(g.units[j]->clone()));
            ++j;
        }
    }
    while(i < count)
    {
        units.push_back(current[i]);
        ++i;
    }
    while(j < g.count)
    {
        units.push_back(static_cast<Unit*>(g.units[j]->clone()));
        ++j;
    }
    count += g.count;
    hp += g.getHp();
    maxHp += g.getMaxHp();
    damage += g.getDamage();
}

unsigned UnitGroup::getCount() const
{
    return count;
}

Entity *UnitGroup::clone() const
{
    return new UnitGroup(*this);
}

void UnitGroup::addUnit(Unit* u)
{
    units.push_back(u);
    ++count;
    hp += u->getHp();
    maxHp += u->getMaxHp();
    damage += u->getDamage();
}

std::pair<UnitGroup*, UnitGroup*> UnitGroup::getParts(unsigned powerPercent)
{
    UnitGroup *group = new UnitGroup(id, type, 0);
    UnitGroup *original = new UnitGroup(id, type, 0);
    unsigned power = 0;
    unsigned desiredPower = round(((float)(damage * powerPercent)) * 0.01f);
    unsigned newPower;
    for(auto unit = units.begin(); unit != units.end(); ++unit)
    {
        newPower = power + (*unit)->getDamage();
        if(newPower > desiredPower)
            original->addUnit(static_cast<Unit*>((*unit)->clone()));
        else
        {
            group->addUnit(static_cast<Unit*>((*unit)->clone()));
            power = newPower;
        }
    }
    return std::make_pair(group, original);
}

UnitGroup *UnitGroup::split(unsigned powerPercent)
{
    UnitGroup *group = new UnitGroup(id, type, 0);
    unsigned power = 0;
    unsigned desiredPower = round(((float)(damage * powerPercent)) * 0.01f);
    unsigned newPower;
    Unit *u;
    for(auto unit = units.begin(); unit != units.end();)
    {
        newPower = power + (*unit)->getDamage();
        if(newPower <= desiredPower)
        {
            u = *unit;
            damage -= u->getDamage();
            hp -= u->getHp();
            maxHp -= u->getMaxHp();
            group->addUnit(u);
            unit = units.erase(unit);
            --count;
            power = newPower;
        }
        else
            ++unit;
    }
    return group;
}

void UnitGroup::setState(UnitState s)
{
    if(healing && s != STAND)
        stopHealing();
    Controllable::setState(s);
}

unsigned UnitGroup::heal(unsigned amount)
{
    unsigned part, groups, healed, toHeal = amount;
    for(unsigned i = 0; i < count; ++i)
    {
        groups = count - i;
        part = (toHeal + groups - 1) / groups; //ceiling division: toHeal / groups
        if(part == 0)
            break;
        healed = units[i]->heal(part);
        toHeal -= healed;
        hp += healed;
    }
    return amount - toHeal;
}

unsigned UnitGroup::takeDamage(unsigned dmg)
{
    if(dmg >= hp)
    {
        alive = false;
        return hp;
    }
    unsigned part, groups, damaged, toDamage = dmg;
    Unit *u;
    for(int i = count - 1; i >= 0; --i)
    {
        groups = i + 1;
        part = (toDamage + groups - 1) / groups; //ceiling division: toDamage / groups
        if(part == 0)
            break;
        u = units[i];
        damaged = u->takeDamage(part);
        toDamage -= damaged;
        hp -= damaged;
        if(!(u->isAlive()))
        {
            maxHp -= u->getMaxHp();
            delete u;
            units.erase(units.begin() + i);
        }
    }
    count = units.size();
    return dmg - toDamage;
}
