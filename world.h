#ifndef WORLD_H
#define WORLD_H

#include "controllable.h"
#include "building.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class Player;
class City;
class Tower;
class ResourcePoint;
class Unit;
class TextParticleEmitter;
class GUI;
class Barracks;
class Hospital;
class Tradepost;
class ProgressBar;
class UnitGroup;

class World : public sf::Drawable
{
public:
    enum BIOME {
        GRASSLAND,
        SWAMP,
        DESERT,
        FOREST,
        MOUNTAINS,
        RIVER,
        BRIDGE
    };

    enum SelectType
    {
        ENEMY_UNIT,
        ENEMY_BUILDING,
        OWN_UNIT,
        OWN_BUILDING,
        SPAWN_POINT,
        HOSPITAL
    };

    //TODO big variant of map with 15 tile size
    static const unsigned TILE_SIZE = 20,
            SCREEN_WIDTH = 1000,
            SCREEN_HEIGHT = 600,
            WIDTH = SCREEN_WIDTH / TILE_SIZE,
            HEIGHT = SCREEN_HEIGHT / TILE_SIZE,
            MENU_WIDTH = 150;

    virtual ~World();

    static World &getInstance();
    void update();
    Player *getPlayer(unsigned id) const;
    GUI *getGUI(unsigned id) const;
    bool isWalkable(unsigned x, unsigned y) const;
    Controllable *getNeighbour(unsigned x, unsigned y, unsigned r, unsigned i);
    Building *isGoodForBuilding(unsigned x, unsigned y, Entity::Type t) const;
    float getDelta() const;
    Entity *getPrototype(Entity::Type t);
    static int fastFloor(float x);
    static unsigned uabs(unsigned a, unsigned b);
    static unsigned umax(unsigned a, unsigned b);
    static std::string unsignedToString(unsigned u);
    static std::string makeString(unsigned num);
    static unsigned typeToIndex(Entity::Type type);
    static Entity::Type unsignedToType(unsigned type);
    static unsigned ManhattanDistance(Entity *a, Entity *b);
    static unsigned ManhattanDistance(unsigned x, unsigned y, unsigned x2, unsigned y2);
    static unsigned ChebyshevDistance(Entity *a, Entity *b);
    static unsigned ChebyshevDistance(unsigned x, unsigned y, unsigned x2, unsigned y2);
    void addBuilding(Building *b, unsigned x, unsigned y, unsigned id);
    void finishBuilding(Building *b);
    bool registerPath(Entity *e, unsigned eX, unsigned eY, Controllable::UnitState s);
    bool registerPath(Entity *e, Entity *t, Controllable::UnitState s);
    unsigned short getOwner(unsigned x, unsigned y) const;
    BIOME getBiome(unsigned x, unsigned y) const;
    Entity *getEntity(unsigned x, unsigned y) const;
    bool getWalkable(unsigned x, unsigned y) const;
    void addStartingUnits();//TODO remove cause its for testing only
    void drawPath(sf::RenderTarget &target, sf::RenderStates states, unsigned id);
    void drawBuilding(sf::RenderTarget &target, sf::RenderStates states,
                      unsigned x, unsigned y, Entity::Type t, unsigned id) const;//TODO make it private
    void setGUIs(GUI *l, GUI *r);
    void addUnit(UnitGroup *unit, unsigned x, unsigned y);
    TextParticleEmitter &getEmitter() const;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void removeEntity(Entity *e);
    sf::Vector2u findClosestEmptyTile(unsigned x, unsigned y);
    Entity *findClosest(unsigned x, unsigned y, SelectType type, unsigned id);
private:
    TextParticleEmitter *emitter;
    GUI *left, *right;
    BIOME **tiles;
    unsigned short **owner;
    bool **walkable;
    Entity ***entities;
    unsigned long seed;
    std::vector<Controllable*> units;
    std::vector<Building*> buildings;
    Player *playerLeft, *playerRight;
    sf::Clock clock;
    float delta;
    City *cityPrototype;
    Tower *towerPrototype;
    ResourcePoint *farmPrototype;
    ResourcePoint *lumbermillPrototype;
    ResourcePoint *quarryPrototype;
    ResourcePoint *goldminePrototype;
    Barracks *barracksPrototype;
    Hospital *hospitalPrototype;
    Tradepost *tradepostPrototype;
    Unit *infantryPrototype;
    Unit *cavalryPrototype;
    Unit *archerPrototype;
    Unit *siegePrototype;
    Unit *magePrototype;
    static sf::RectangleShape tile;
    static sf::Sprite sprite;
    std::vector<Entity*> toDelete;
    ProgressBar *buildingProgress;

    static World *instance;
    static bool initialized;

    World();
    float get(float x, float y, unsigned octaves) const;
    double value(long x, long y) const;
    double value2(long x, long y) const;
    float LinearInterpolate(float a, float b, float c) const;
    inline float dot(float gx, float gy, float x, float y) const;
    float interpolatedNoise(float x, float y) const;
    static float remap(float height);
    static BIOME chooseBiome(float height, float moisture);
    void updateBuilding(Building *b);
    void updatePlayerBuildings(unsigned id);
    void drawConstruction(sf::RenderTarget &target, sf::RenderStates states,
                          Building *b) const;
};

#endif // WORLD_H
