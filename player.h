#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "unitgroup.h"
#include "building.h"
#include "world.h"
#include <vector>
#include <string>

class City;
class Hero;
class Barracks;
class Hospital;
class Tradepost;

class Player
{
public:
    static const unsigned MAX_UNITS = 8;

    Player(unsigned _id);
    virtual ~Player();

    City* getCity() const;
    Entity* getActiveEntity() const;
    void resetUnitSelection();
    void selectSlot(unsigned index);
    void selectHero();
    void addUnit(UnitGroup *u);
    void addUnitsToActiveSlot(UnitGroup *u);
    void addFoodSource(unsigned f);
    void addWoodSource(unsigned w);
    void addStoneSource(unsigned s);
    void addGoldSource(unsigned g);
    unsigned getFood() const;
    unsigned getWood() const;
    unsigned getStone() const;
    unsigned getGold() const;
    std::string getFoodString() const;
    std::string getWoodString() const;
    std::string getStoneString() const;
    std::string getGoldString() const;
    unsigned getFoodPerSec() const;
    unsigned getWoodPerSec() const;
    unsigned getStonePerSec() const;
    unsigned getGoldPerSec() const;
    std::string getFoodPerSecStr() const;
    std::string getWoodPerSecStr() const;
    std::string getStonePerSecStr() const;
    std::string getGoldPerSecStr() const;
    Barracks *getBarracks() const;
    Hospital *getHospital() const;
    Tradepost *getTradepost() const;
    void setBarracks(Barracks *b);
    void setHospital(Hospital *h);
    void setTradepost(Tradepost *t);
    bool spend(unsigned f, unsigned w, unsigned s, unsigned g);
    void update(float delta);
    void checkDeaths();
    bool hasResourceChanges();
    UnitGroup *getUnitSlot(unsigned i);
    UnitGroup *getActiveSlot();
    UnitGroup *getUnit(unsigned i);
    unsigned getUnitCount() const;
    unsigned getUnitsInBarracksCount(unsigned i);
    void addUnitsToBarracks(unsigned type, unsigned count);
    void takeUnitsFromBarracks(unsigned type, unsigned count);
    void swapSlots(unsigned a, unsigned b);
    void clearSlot(unsigned i);
private:
    unsigned id;
    UnitGroup *unitGroups[MAX_UNITS];
    std::vector<Building*> buildings;
    Hero *hero;
    City *city;
    Entity *activeEntity;
    unsigned activeIndex;
    unsigned unitCount;
    unsigned food, wood, stone, gold;
    unsigned fps, wps, sps, gps;
    float incomeTime;
    std::string foodStr, woodStr, stoneStr, goldStr,
                fpsStr, wpsStr, spsStr, gpsStr;
    Barracks *barracks;
    Hospital *hospital;
    Tradepost *tradepost;
    bool resourceChanges;
    unsigned unitsInBarracks[5];
};

#endif // BUILDING_H




