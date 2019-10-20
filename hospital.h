#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "building.h"

class Hospital : public Building
{
public:
    Hospital();
    Hospital(const Hospital &b);
    virtual ~Hospital();
    virtual Entity *clone() const;
    virtual void update(float delta);
    virtual void buildingFinished();
};

#endif // HOSPITAL_H
