#include "player.h"
#include "city.h"
#include "hero.h"
#include "gui.h"

Player::Player(unsigned _id) : id(_id), activeEntity(nullptr), activeIndex(0),
        unitCount(0), food(1000), wood(1000), stone(500), gold(200),
        fps(0), wps(0), sps(0), gps(0), incomeTime(0.0f),
        foodStr("1000"), woodStr("1000"), stoneStr("500"), goldStr("200"),
        fpsStr("+0"), wpsStr("+0"), spsStr("+0"), gpsStr("+0"),
        barracks(nullptr), hospital(nullptr), tradepost(nullptr), resourceChanges(true) {
    city = new City;
    buildings.push_back(city);
    for(unsigned i = 0; i < MAX_UNITS; ++i)
        unitGroups[i] = nullptr;
    for(unsigned i = 0; i < 5; ++i)
        unitsInBarracks[i] = 0;
}

bool Player::hasResourceChanges()
{
    if(resourceChanges) {
        resourceChanges = false;
        return true;
    } else
        return false;
}

void Player::checkDeaths()
{
    for(unsigned i = 0; i < MAX_UNITS; ++i)
        if(unitGroups[i] && !(unitGroups[i]->isAlive()))
        {
            --unitCount;
            unitGroups[i] = nullptr;
            if(unitCount == 0)
                World::getInstance().getGUI(id)->lastEntityDeathNotify();
        }
    if(activeEntity && !activeEntity->isAlive())
        activeEntity = nullptr;
}

Player::~Player() {}

City* Player::getCity() const
{
    return city;
}

Entity* Player::getActiveEntity() const
{
    return activeEntity;
}

void Player::resetUnitSelection()
{
    activeIndex = 0;
    if(unitCount == 0)
        activeEntity = nullptr;
    else
    {
        while(!(unitGroups[activeIndex]))
            ++activeIndex;
        activeEntity = unitGroups[activeIndex];
    }
}

void Player::addUnit(UnitGroup *u)
{
    unsigned i = 0;
    while(unitGroups[i])
        ++i;
    unitGroups[i] = u;
    ++unitCount;
}

void Player::addUnitsToActiveSlot(UnitGroup* u)
{
    UnitGroup *slot = unitGroups[activeIndex];
    if(slot)
        slot->merge(*u);
    else
    {
        unitGroups[activeIndex] = u;
        ++unitCount;
    }
}

unsigned Player::getUnitCount() const
{
    return unitCount;
}

UnitGroup *Player::getUnitSlot(unsigned i)
{
    return unitGroups[i];
}

UnitGroup *Player::getActiveSlot()
{
    return unitGroups[activeIndex];
}

UnitGroup *Player::getUnit(unsigned i)
{
    unsigned index = 0;
    for(unsigned j = 0; j < MAX_UNITS; ++j)
        if(unitGroups[j])
        {
            if(index == i)
                return unitGroups[j];
            ++index;
        }
    return nullptr;
}

void Player::selectSlot(unsigned index)
{
    activeIndex = index;
    activeEntity = unitGroups[activeIndex];
}

void Player::selectHero()
{
    activeEntity = hero;
}

void Player::addFoodSource(unsigned f)
{
    fps += f;
    fpsStr = "+" + World::makeString(fps);
}

void Player::addWoodSource(unsigned w)
{
    wps += w;
    wpsStr = "+" + World::makeString(wps);
}


void Player::addStoneSource(unsigned s)
{
    sps += s;
    spsStr = "+" + World::makeString(sps);
}


void Player::addGoldSource(unsigned g)
{
    gps += g;
    gpsStr = "+" + World::makeString(gps);
}

unsigned Player::getFood() const
{
    return food;
}

unsigned Player::getWood() const
{
    return wood;
}

unsigned Player::getStone() const
{
    return stone;
}

unsigned Player::getGold() const
{
    return gold;
}

std::string Player::getFoodString() const
{
    return foodStr;
}

std::string Player::getWoodString() const
{
    return woodStr;
}

std::string Player::getStoneString() const
{
    return stoneStr;
}

std::string Player::getGoldString() const
{
    return goldStr;
}

unsigned Player::getFoodPerSec() const
{
    return fps;
}

unsigned Player::getWoodPerSec() const
{
    return wps;
}

unsigned Player::getStonePerSec() const
{
    return sps;
}

unsigned Player::getGoldPerSec() const
{
    return gps;
}

std::string Player::getFoodPerSecStr() const
{
    return fpsStr;
}

std::string Player::getWoodPerSecStr() const
{
    return wpsStr;
}

std::string Player::getStonePerSecStr() const
{
    return spsStr;
}

std::string Player::getGoldPerSecStr() const
{
    return gpsStr;
}

Barracks *Player::getBarracks() const
{
    return barracks;
}

Hospital *Player::getHospital() const
{
    return hospital;
}

Tradepost *Player::getTradepost() const
{
    return tradepost;
}

void Player::setBarracks(Barracks *b)
{
    barracks = b;
}

void Player::setHospital(Hospital *h)
{
    hospital = h;
}

void Player::setTradepost(Tradepost *t)
{
    tradepost = t;
}

bool Player::spend(unsigned f, unsigned w,
                 unsigned s, unsigned g)
{
    if(f > food || w > wood || s > stone || g > gold)
        return false;
    food -= f;
    wood -= w;
    stone -= s;
    gold -= g;
    foodStr = World::makeString(food);
    woodStr = World::makeString(wood);
    stoneStr = World::makeString(stone);
    goldStr = World::makeString(gold);
    resourceChanges = true;
    return true;
}

unsigned Player::getUnitsInBarracksCount(unsigned i)
{
    return unitsInBarracks[i];
}

void Player::addUnitsToBarracks(unsigned type, unsigned count)
{
    unitsInBarracks[type] += count;
}

void Player::takeUnitsFromBarracks(unsigned type, unsigned count)
{
    unitsInBarracks[type] -= count;
}

void Player::update(float delta)
{
    incomeTime += delta;
    if(incomeTime >= 1.0f)
    {
        food += fps;
        wood += wps;
        stone += sps;
        gold += gps;
        foodStr = World::makeString(food);
        woodStr = World::makeString(wood);
        stoneStr = World::makeString(stone);
        goldStr = World::makeString(gold);
        //TODO save previous parts of string to minimize string conversion
        incomeTime -= 1.0f;
        resourceChanges = true;
    }
    checkDeaths();
}

void Player::swapSlots(unsigned a, unsigned b)
{
    UnitGroup *temp = unitGroups[a];
    unitGroups[a] = unitGroups[b];
    unitGroups[b] = temp;
}

void Player::clearSlot(unsigned i)
{
    if(activeEntity == unitGroups[i])
        activeEntity = nullptr;
    unitGroups[i] = nullptr;
    --unitCount;
}
