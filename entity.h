#ifndef ENTITY_H
#define ENTITY_H

#include "aura.h"
#include <vector>

class World;

class Entity
{
public:
    enum Type
    {
        INFANTRY,
        CAVALRY,
        ARCHER,
        SIEGE,
        MAGE,
        HERO,
        CITY,
        FARM,
        LUMBERMILL,
        QUARRY,
        GOLDMINE,
        BARRACKS,
        TOWER,
        HOSPITAL,
        TRADEPOST,
        VILLAGE,
        MONSTER,
        CORPSE
    };

    enum ResourceType
    {
        FOOD,
        WOOD,
        STONE,
        GOLD
    };

    Entity();
    Entity(const Entity &e);
    virtual ~Entity();

    virtual void update(float delta);
    unsigned getX() const;
    unsigned getY() const;
    Type getType() const;
    unsigned getId() const;
    unsigned getDamage() const;
    unsigned getHp() const;
    unsigned getMaxHp() const;
    float getHpFactor() const;
    bool isAlive() const;
    void setX(unsigned _x);
    void setY(unsigned _y);
    void setId(unsigned _id);
    virtual unsigned takeDamage(unsigned dmg);
    bool isHealthy() const;
    bool isHealing() const;
    void startHealing();
    void stopHealing();
    virtual unsigned heal(unsigned amount);
    virtual Entity *clone() const = 0;
protected:
    bool alive;
    unsigned hp;
    unsigned maxHp;
    unsigned damage;
    Type type;
    unsigned x, y;
    unsigned id;
    bool healing;
    unsigned healingSpeed;
    float healTimeout;
    //TODO unsigned armor;
    std::vector<Aura*> auras;
};

#endif // ENTITY_H
