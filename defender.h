#ifndef DEFENDER_H
#define DEFENDER_H

class Entity;

class Defender
{
public:
    enum DefendState
    {
        FIGHT,
        IDLE
    };
    Defender(Entity *p, unsigned r);
    virtual ~Defender();
    virtual void update(float delta);
    void setState(DefendState s);
    unsigned getRadius() const;
private:
    Entity *parent;
    unsigned radius;
    DefendState state;
    float attackCoef;
    Entity *targetEntity;
    unsigned targetX, targetY;
};

#endif // DEFENDER_H
