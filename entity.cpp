#include "entity.h"

Entity::Entity() : alive(true), healing(false) {}

Entity::Entity(const Entity &e) : alive(true), hp(e.hp), maxHp(e.maxHp),
    damage(e.damage), type(e.type), x(e.x), y(e.y), id(e.id),
    healing(e.healing), healingSpeed(0), healTimeout(0.0f) {} //TODO decide if should copy auras

Entity::~Entity() {}

void Entity::update(float delta)
{
    if(healing)
    {
        healTimeout += 100.0f * delta; //1s=100.0f
        if(healTimeout >= 100.0f)
        {
            heal(healingSpeed);
            if(hp == maxHp)
                stopHealing();
            healTimeout -= 100.0f;
        }
    }
}

unsigned Entity::heal(unsigned amount)
{
    unsigned diff = maxHp - hp;
    if(amount >= diff)
    {
        hp = maxHp;
        return diff;
    }
    else
    {
        hp += amount;
        return amount;
    }
}

unsigned Entity::getX() const
{
    return x;
}

unsigned Entity::getY() const
{
    return y;
}

Entity::Type Entity::getType() const
{
    return type;
}

unsigned Entity::getId() const
{
    return id;
}

void Entity::setX(unsigned _x)
{
    x = _x;
}

void Entity::setY(unsigned _y)
{
    y = _y;
}

void Entity::setId(unsigned _id)
{
    id = _id;
}

bool Entity::isAlive() const
{
    return alive;
}

unsigned Entity::getDamage() const
{
    return damage;
}

unsigned Entity::getHp() const
{
    return hp;
}

unsigned Entity::getMaxHp() const
{
    return maxHp;
}

float Entity::getHpFactor() const
{
    return ((float) hp) / maxHp;
}

unsigned Entity::takeDamage(unsigned dmg)
{
    if(dmg >= hp)
    {
        alive = false;
        return hp;
    }
    else
    {
        stopHealing();
        hp -= dmg;
        return dmg;
    }
}

bool Entity::isHealthy() const
{
    return hp == maxHp;
}

bool Entity::isHealing() const
{
    return healing;
}

void Entity::startHealing()
{
    healing = true;
    healTimeout = 0.0f;
}

void Entity::stopHealing()
{
    healing = false;
}
