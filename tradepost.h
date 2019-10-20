#ifndef TRADEPOST_H
#define TRADEPOST_H

#include "building.h"

class Tradepost : public Building
{
public:
    Tradepost();
    Tradepost(const Tradepost &b);
    virtual ~Tradepost();
    virtual Entity *clone() const;
    virtual void update(float delta);
    virtual void buildingFinished();
};

#endif // TRADEPOST_H
