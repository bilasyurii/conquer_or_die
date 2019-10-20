#include "world.h"
#include "city.h"
#include "tower.h"
#include "player.h"
#include "resourcepoint.h"
#include "building.h"
#include "barracks.h"
#include "hospital.h"
#include "tradepost.h"
#include "path.h"
#include "unit.h"
#include "gui.h"
#include "progressbar.h"
#include "resourcemanager.h"
#include "textparticleemitter.h"
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <sstream>

sf::RectangleShape World::tile;
sf::Sprite World::sprite;
World *World::instance;
bool World::initialized = false;

World &World::getInstance()
{
    if(!initialized)
    {
        instance = new World;
        initialized = true;
    }
    return *instance;
}

void World::setGUIs(GUI *l, GUI *r)
{
    left = l;
    right = r;
}

TextParticleEmitter &World::getEmitter() const
{
    return *emitter;
}

World::World()
{
    emitter = new TextParticleEmitter;
    sprite.setTexture(ResourceManager::getInstance().getTexture());
    srand(time(0));
    seed = rand();
    tiles = new BIOME*[HEIGHT];
    walkable = new bool*[HEIGHT];
    entities = new Entity**[HEIGHT];
    owner = new unsigned short*[HEIGHT];
    for(unsigned i = 0; i < HEIGHT; ++i)
    {
        tiles[i] = new BIOME[WIDTH];
        walkable[i] = new bool[WIDTH];
        entities[i] = new Entity*[WIDTH];
        owner[i] = new unsigned short[WIDTH];
    }
    for(unsigned y = 0; y < HEIGHT; ++y)
        for(unsigned x = 0; x < WIDTH; ++x)
        {
            float height = remap(get(x * 2, y * 2, 5));
            float moisture = remap(get((x + 53) * 1.0f, (y + 71) * 1.0f, 1));
            tiles[y][x] = chooseBiome(height, moisture);
            walkable[y][x] = true;
            owner[y][x] = 0;
            entities[y][x] = nullptr;
        }
    unsigned step = WIDTH / 5, target;
    unsigned point = (rand() % (WIDTH - step * 2)) + step;
    sf::Vector2i pos, targetVec, direction;
    pos.y = 0;
    pos.x = point;
    tiles[pos.y][pos.x] = BIOME::RIVER;
    walkable[pos.y][pos.x] = false;
    for(unsigned i = 0; i < 6; ++i)
    {
        if(point < step * 2)
            target = (rand() % (point + 1)) + step;
        else if(WIDTH - point <= step * 2)
            target = WIDTH - (rand() % (WIDTH - point)) - 1 - step;
        else
            target = (rand() % (step * 2 + 1)) + point - step;
        targetVec.x = target;
        targetVec.y = (i + 1) * HEIGHT / 6 - 1;
        direction = targetVec - pos;
        int stepX = (direction.x >= 0) ? 1 : -1;
        float hypot = std::hypot(direction.x, direction.y);
        sf::Vector2f normDir(direction.x / hypot, direction.y / hypot);
        sf::Vector2f rayStep((normDir.x == 0) ? 9999.0f : direction.x / normDir.x,
                             (normDir.y == 0) ? 9999.0f : direction.y / normDir.y);
        sf::Vector2f delta((normDir.x == 0) ? 9999.0f : TILE_SIZE / normDir.x,
                           (normDir.y == 0) ? 9999.0f : TILE_SIZE / normDir.y);
        do
        {
            if(std::fabs(rayStep.y) < std::fabs(rayStep.x))
            {
                rayStep.y += delta.y;
                ++pos.y;
            }
            else
            {
                rayStep.x += delta.x;
                pos.x += stepX;
            }
            tiles[pos.y][pos.x] = BIOME::RIVER;
            walkable[pos.y][pos.x] = false;
        }
        while(pos != targetVec);
        point = target;
        pos = targetVec;
    }
    for(unsigned y = 0; y < HEIGHT; ++y)
        for(unsigned x = step; x < WIDTH; ++x)
            if(y > 0 && y < HEIGHT - 1 &&
               tiles[y][x] == BIOME::RIVER)
            {
                unsigned neighbourRiverCount = 0;
                if(tiles[y - 1][x] == BIOME::RIVER)
                    ++neighbourRiverCount;
                if(tiles[y + 1][x] == BIOME::RIVER)
                    ++neighbourRiverCount;
                if(tiles[y][x - 1] == BIOME::RIVER)
                    ++neighbourRiverCount;
                if(tiles[y][x + 1] == BIOME::RIVER)
                    ++neighbourRiverCount;
                if(neighbourRiverCount < 2)
                    tiles[y][x] = BIOME::SWAMP;
            }
    unsigned bridge, bridgeLen, neighbourLenT, neighbourLenB;
    for(unsigned i = 0; i < 3; ++i)
    {
        bridge = (HEIGHT - 24) / 2 + i * 9 + (rand() % 6);
        bridgeLen = neighbourLenT = neighbourLenB = 0;
        for(unsigned j = step; j < WIDTH; ++j)
        {
            if(tiles[bridge][j] == BIOME::RIVER)
                ++bridgeLen;
            if(tiles[bridge - 1][j] == BIOME::RIVER)
                ++neighbourLenT;
            if(tiles[bridge + 1][j] == BIOME::RIVER)
                ++neighbourLenB;
        }
        if(bridgeLen < neighbourLenT)
        {
            if(neighbourLenB < bridgeLen)
                ++bridge;
        }
        else
        {
            if(neighbourLenT < neighbourLenB)
                --bridge;
            else
                ++bridge;
        }
        for(unsigned j = step; j < WIDTH; ++j)
            if(tiles[bridge][j] == BIOME::RIVER)
            {
                tiles[bridge][j] = BIOME::BRIDGE;
                walkable[bridge][j] = true;
            }
    }
    for(unsigned y = 0; y < HEIGHT; ++y)
        for(unsigned x = 0; x < WIDTH; ++x)
            if(tiles[y][x] == BIOME::RIVER)
            {
                if(y > 0 &&
                        tiles[y - 1][x] == BIOME::GRASSLAND &&
                        rand() % 4 == 0)
                {
                    tiles[y - 1][x] = BIOME::SWAMP;
                    walkable[y - 1][x] = true;
                }
                if(y < HEIGHT - 1 &&
                        tiles[y + 1][x] == BIOME::GRASSLAND &&
                        rand() % 4 == 0)
                {
                    tiles[y + 1][x] = BIOME::SWAMP;                    walkable[y + 1][x] = true;
                }
                if(tiles[y][x - 1] == BIOME::GRASSLAND &&
                        rand() % 4 == 0)
                {
                    tiles[y][x - 1] = BIOME::SWAMP;
                    walkable[y][x - 1] = true;
                }
                if(tiles[y][x + 1] == BIOME::GRASSLAND &&
                        rand() % 4 == 0)
                {
                    tiles[y][x + 1] = BIOME::SWAMP;                    walkable[y][x + 1] = true;
                }
            }
    playerLeft = new Player(1);
    playerLeft->addUnitsToBarracks(1, 55);//TODO remove this cause its only test here
    playerRight = new Player(2);
    unsigned stepX = step / 3, stepY = World::HEIGHT / 3;
    City *c = playerLeft->getCity();
    addBuilding(c,(rand() % stepX) + stepX,
                (rand() % stepY) + stepY, 1);
    updateBuilding(c);    c = playerRight->getCity();
    addBuilding(c,(rand() % stepX) + stepX + WIDTH - step,
                (rand() % stepY) + stepY, 2);
    updateBuilding(c);
    // creating prototypes
    cityPrototype = new City;
    towerPrototype = new Tower;
    farmPrototype = new ResourcePoint(Entity::FARM, 2);
    lumbermillPrototype = new ResourcePoint(Entity::LUMBERMILL, 2);
    quarryPrototype = new ResourcePoint(Entity::QUARRY, 2);
    goldminePrototype = new ResourcePoint(Entity::GOLDMINE, 2);
    barracksPrototype = new Barracks;
    hospitalPrototype = new Hospital;
    tradepostPrototype = new Tradepost;
    infantryPrototype = new Unit(Entity::INFANTRY);
    cavalryPrototype = new Unit(Entity::CAVALRY);
    archerPrototype = new Unit(Entity::ARCHER);
    siegePrototype = new Unit(Entity::SIEGE);
    magePrototype = new Unit(Entity::MAGE);
    buildingProgress = new ProgressBar(0, 0, 18, 6);
}

unsigned short World::getOwner(unsigned x, unsigned y) const
{
    return owner[y][x];
}

World::BIOME World::getBiome(unsigned x, unsigned y) const
{
    return tiles[y][x];
}

bool World::getWalkable(unsigned x, unsigned y) const
{
    return walkable[y][x];
}

Entity *World::getEntity(unsigned x, unsigned y) const
{
    return entities[y][x];
}

Player *World::getPlayer(unsigned id) const
{
    if(id == 1)
        return playerLeft;
    return playerRight;
}

GUI *World::getGUI(unsigned id) const
{
    if(id == 1)
        return left;
    return right;
}

void World::addUnit(UnitGroup *unit, unsigned x, unsigned y)
{
    unit->setX(x);
    unit->setY(y);
    units.push_back(unit);
    entities[y][x] = unit;
    walkable[y][x] = false;
    if(unit->getId() == 1)
    {
        playerLeft->addUnitsToActiveSlot(unit);
        left->updateSelection();
    }
    else
    {
        playerRight->addUnitsToActiveSlot(unit);
        right->updateSelection();
    }
}

void World::removeEntity(Entity *e)
{
    entities[e->getY()][e->getX()] = nullptr;
    walkable[e->getY()][e->getX()] = true;
    for(auto unit = units.begin(); unit != units.end(); ++unit)
        if(*unit == e)
        {
            units.erase(unit);
            break;
        }
}

void World::addStartingUnits()
{
    //TODO it's only a test down here
    addUnit(new UnitGroup(1, Entity::CAVALRY, 30), 10, 15);
    playerLeft->selectSlot(1);
    addUnit(new UnitGroup(1, Entity::MAGE, 5), 7, 15);
    addUnit(new UnitGroup(2, Entity::ARCHER, 15), 11, 9);
    playerRight->selectSlot(1);
    addUnit(new UnitGroup(2, Entity::SIEGE, 15), 13, 16);
    playerRight->selectSlot(2);
    addUnit(new UnitGroup(2, Entity::CAVALRY, 15), 9, 20);
}

void World::updateBuilding(Building *b)
{
    unsigned id = b->getId();
    Player *p = getPlayer(id);
    unsigned x = b->getX();
    unsigned y = b->getY();
    int ir = b->getInfluenceRadius();
    for(int i = -ir; i <= ir; ++i)
        for(int j = -ir; j <= ir; ++j)
            if(i + y >= 0 && i + y < HEIGHT &&
               j + x >= 0 && j + x < WIDTH &&
               owner[i + y][j + x] == 0)
            {
                owner[i + y][j + x] = id;
                switch(tiles[i + y][j + x])
                {
                case GRASSLAND:
                    p->addFoodSource(3);
                    break;
                case SWAMP:
                    p->addFoodSource(1);
                    break;
                case DESERT:
                    p->addGoldSource(1);
                    break;
                case FOREST:
                    p->addWoodSource(2);
                    p->addFoodSource(1);
                    break;
                case MOUNTAINS:
                    p->addStoneSource(2);
                    p->addFoodSource(1);
                    break;
                case BRIDGE:
                    p->addGoldSource(1);
                    break;
                default:
                    break;
                }
            }
    if(b->getType() == Entity::FARM)
        p->addFoodSource(static_cast<ResourcePoint*>(b)->getResourceIncome());
    else if(b->getType() == Entity::LUMBERMILL)
        p->addWoodSource(static_cast<ResourcePoint*>(b)->getResourceIncome());
    else if(b->getType() == Entity::QUARRY)
        p->addStoneSource(static_cast<ResourcePoint*>(b)->getResourceIncome());
    else if(b->getType() == Entity::GOLDMINE)
        p->addGoldSource(static_cast<ResourcePoint*>(b)->getResourceIncome());
}

void World::addBuilding(Building *b, unsigned x,
                        unsigned y, unsigned id)
{
    b->setId(id);
    b->setX(x);
    b->setY(y);
    buildings.push_back(b);
    walkable[y][x] = false;
    entities[y][x] = b;
}

void World::finishBuilding(Building *b)
{
    updateBuilding(b);
    if(b->getId() == 1)
        left->updateMenu();
    else
        right->updateMenu();
}

void World::updatePlayerBuildings(unsigned id)
{
    Building *b;
    for(unsigned i = 0; i < buildings.size(); ++i)
    {
        b = buildings[i];
        if(b->getId() == id)
            updateBuilding(b);
    }
}

bool World::registerPath(Entity* e, unsigned eX, unsigned eY, Controllable::UnitState s)
{
    Controllable *c = static_cast<Controllable*>(e);
    if((e->getX() == eX && e->getY() == eY) || !walkable[eY][eX])
        c->setState(Controllable::STAND);
    else
    {
        c->getPath()->setEnd(eX, eY);
        c->getPath()->setStart(e->getX(), e->getY());
        c->setState(s);
        if(c->getState() == Controllable::STAND)
            return false;
    }
    return true;
}

bool World::registerPath(Entity *e, Entity *t, Controllable::UnitState s)
{
    Controllable *c = static_cast<Controllable*>(e);
    c->setTarget(t);
    c->setState(s);
    return c->getState() != Controllable::STAND;//TODO use this
}

World::BIOME World::chooseBiome(float height, float moisture)
{
    if(height < -0.1f)
    {
        if(moisture < -0.3f)
            return BIOME::DESERT;
        else if(moisture > 0.2)
            return BIOME::SWAMP;
        else
            return BIOME::GRASSLAND;
    }
    if(height < 0.0f)
    {
        if(moisture < -0.3f)
            return BIOME::DESERT;
        else
            return BIOME::GRASSLAND;
    }
    else if(height < 0.05f)
        return BIOME::GRASSLAND;
    else if(height < 0.25f)
    {
        if(moisture < -0.2f)
            return BIOME::GRASSLAND;
        else
            return BIOME::FOREST;
    }
    else if(height < 0.3f)
        return BIOME::FOREST;
    else
        return BIOME::MOUNTAINS;
}

World::~World()
{
    for(unsigned i = 0; i < HEIGHT; ++i)
    {
        delete tiles[i];
        delete owner[i];
        delete walkable[i];
        delete entities[i];
    }
    delete tiles;
    delete owner;
    delete walkable;
    delete playerLeft;
    delete entities;
    delete playerRight;
    delete cityPrototype;
    delete towerPrototype;
    //TODO delete other prototypes
    delete buildingProgress;
    delete emitter;
}

bool World::isWalkable(unsigned x, unsigned y) const
{
    return walkable[y][x];
}

void World::drawConstruction(sf::RenderTarget &target, sf::RenderStates states,
                             Building *b) const
{
    tile.setPosition(b->getX() * TILE_SIZE + MENU_WIDTH, b->getY() * TILE_SIZE);
    tile.setFillColor((b->getId() == 1)?sf::Color::Red:sf::Color::Blue);
    target.draw(tile, states);
    sprite.setTextureRect(sf::IntRect(100, 20, 20, 20));
    sprite.setPosition(b->getX() * TILE_SIZE + MENU_WIDTH, b->getY() * TILE_SIZE);
    target.draw(sprite, states);
    buildingProgress->setFactor(b->getBuildFactor());
    buildingProgress->setX(b->getX() * TILE_SIZE + MENU_WIDTH + 1.0f);
    buildingProgress->setY(b->getY() * TILE_SIZE + 13.0f);
    target.draw(*buildingProgress, states);
}

void World::drawBuilding(sf::RenderTarget& target, sf::RenderStates states,
                unsigned x, unsigned y, Entity::Type t, unsigned id) const
{
    tile.setPosition(x * TILE_SIZE + MENU_WIDTH, y * TILE_SIZE);
    tile.setFillColor((id == 1)?sf::Color::Red:sf::Color::Blue);
    target.draw(tile, states);
    switch(t)
    {
    case Entity::CITY:
        sprite.setTextureRect(sf::IntRect(id * 20 - 20, 80, 20, 20));
        break;
    case Entity::FARM:
        sprite.setTextureRect(sf::IntRect(0, 40, 20, 20));
        break;
    case Entity::LUMBERMILL:
        sprite.setTextureRect(sf::IntRect(20, 40, 20, 20));
        break;
    case Entity::QUARRY:
        sprite.setTextureRect(sf::IntRect(40, 40, 20, 20));
        break;
    case Entity::GOLDMINE:
        sprite.setTextureRect(sf::IntRect(60, 40, 20, 20));
        break;
    case Entity::BARRACKS:
        sprite.setTextureRect(sf::IntRect(id * 20 - 20, 60, 20, 20));
        break;
    case Entity::TOWER:
        sprite.setTextureRect(sf::IntRect(80, 40, 20, 20));
        break;
    case Entity::HOSPITAL:
        sprite.setTextureRect(sf::IntRect(80, 60, 20, 20));
        break;
    case Entity::TRADEPOST:
        sprite.setTextureRect(sf::IntRect(40, 80, 20, 20));
        break;
    case Entity::VILLAGE:
        sprite.setTextureRect(sf::IntRect(60, 80, 20, 20));
        break;
    default:
        break;
    }
    sprite.setPosition(x * TILE_SIZE + MENU_WIDTH, y * TILE_SIZE);
    target.draw(sprite, states);
}

void World::drawPath(sf::RenderTarget &target, sf::RenderStates states, unsigned id)
{
    if(id == 1)
        tile.setFillColor(sf::Color(255, 0, 0, 150));
    else
        tile.setFillColor(sf::Color(0, 0, 255, 150));
    tile.setSize(sf::Vector2f(World::TILE_SIZE * 0.5f, World::TILE_SIZE * 0.5f));
    //TODO draw triangles, not squares
    //TODO fix "entity has died but selection is still set" bug
    Path *p;
    Controllable *u;
    // drawing path
    for(unsigned i = 0; i < units.size(); ++i)
    {
        u = units[i];
        p = u->getPath();
        if(u == getPlayer(id)->getActiveEntity() && p->wasFound())
        {
            for(unsigned i = 0; i < p->nodes.size(); ++i)
            {
                tile.setPosition(p->nodes[i].x * TILE_SIZE +
                                 TILE_SIZE * 0.25f + MENU_WIDTH,
                                 p->nodes[i].y * TILE_SIZE +
                                 TILE_SIZE * 0.25f);
                target.draw(tile, states);
            }
        }
    }
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    tile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    for(unsigned y = 0; y < HEIGHT; ++y)
        for(unsigned x = 0; x < WIDTH; ++x)
        {
            switch(tiles[y][x])
            {
            case GRASSLAND:
                tile.setFillColor(sf::Color(0, 150, 20));
                break;
            case FOREST:
                tile.setFillColor(sf::Color(0, 120, 50));
                break;
            case MOUNTAINS:
                tile.setFillColor(sf::Color(120, 120, 120));
                break;
            case DESERT:
                tile.setFillColor(sf::Color(255, 255, 50));
                break;
            case SWAMP:
                tile.setFillColor(sf::Color(50, 120, 20));
                break;
            case RIVER:
                tile.setFillColor(sf::Color(0, 150, 255));
                break;
            case BRIDGE:
                tile.setFillColor(sf::Color(80, 40, 0));
            }
            tile.setPosition(x * TILE_SIZE + MENU_WIDTH, y * TILE_SIZE);
            target.draw(tile, states);
        }
    //drawing borders
    tile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    for(unsigned y = 0; y < HEIGHT; ++y)
        for(unsigned x = 0; x < WIDTH; ++x)
        {
            if(owner[y][x] == 0)
                continue;
            if(owner[y][x] == 1)
                tile.setFillColor(sf::Color(255, 0, 0, 100));
            else
                tile.setFillColor(sf::Color(0, 0, 255, 100));
            tile.setPosition(x * TILE_SIZE + MENU_WIDTH, y * TILE_SIZE);
            target.draw(tile, states);
        }
    //drawing buildings
    tile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    Building *b;
    for(unsigned i = 0; i < buildings.size(); ++i)
    {
        b = buildings[i];
        if(b->isBuilt())
            drawBuilding(target, states, b->getX(), b->getY(),
                         b->getType(), b->getId());
        else
            drawConstruction(target, states, b);
    }
    //drawing units
    Controllable *u;
    unsigned x, y;
    tile.setSize(sf::Vector2f(20.0f, 20.0f));
    for(unsigned i = 0; i < units.size(); ++i)
    {
        u = units[i];
        x = u->getX();
        y = u->getY();
        if(u->getId() == 1)
            tile.setFillColor(sf::Color::Red);
        else
            tile.setFillColor(sf::Color::Blue);
        tile.setPosition(x * TILE_SIZE + MENU_WIDTH, y * TILE_SIZE);
        target.draw(tile, states);
        sprite.setTextureRect(sf::IntRect(((unsigned) u->getType()) * 20,
                                          0, 20, 20));
        sprite.setPosition(x * TILE_SIZE + MENU_WIDTH, y * TILE_SIZE);
        target.draw(sprite, states);
        if(u->isHealing())
        {
            sprite.setTextureRect(sf::IntRect(70, 100, 10, 10));
            sprite.setPosition(x * TILE_SIZE + MENU_WIDTH + 2, y * TILE_SIZE + 2);
            target.draw(sprite, states);
        }
    }
    target.draw(*emitter, states);
}

void World::update()
{
    delta = clock.restart().asSeconds();
    Controllable *u;    for(unsigned i = 0; i < units.size(); ++i)
    {
        u = units[i];
        walkable[u->getY()][u->getX()] = true;
        entities[u->getY()][u->getX()] = nullptr;
        u->update(delta);
        walkable[u->getY()][u->getX()] = false;
        entities[u->getY()][u->getX()] = u;
    }
    for(unsigned i = 0; i < buildings.size(); ++i)
        buildings[i]->update(delta);
    for(unsigned i = 0; i < units.size();)
    {
        u = units[i];
        if(!u->isAlive())
        {
            toDelete.push_back(u);
            units.erase(units.begin() + i);
            walkable[u->getY()][u->getX()] = true;
            entities[u->getY()][u->getX()] = nullptr;
        }
        else
            ++i;
    }
    playerLeft->update(delta);
    playerRight->update(delta);
    for(Entity *e : toDelete)
        delete e;
    toDelete = std::vector<Entity*>();
    emitter->update(delta);
}

unsigned World::ManhattanDistance(Entity *a, Entity *b)
{
    return uabs(a->getX(), b->getX()) + uabs(a->getY(), b->getY());
}

unsigned World::ManhattanDistance(unsigned x, unsigned y, unsigned x2, unsigned y2)
{
    return uabs(x, x2) + uabs(y, y2);
}

unsigned World::ChebyshevDistance(Entity* a, Entity* b)
{
    return umax(uabs(a->getX(), b->getX()), uabs(a->getY(), b->getY()));
}

unsigned World::ChebyshevDistance(unsigned x, unsigned y, unsigned x2, unsigned y2)
{
    return umax(uabs(x, x2), uabs(y, y2));
}

unsigned World::uabs(unsigned a, unsigned b)
{
    return (a > b)?(a - b):(b - a);
}

unsigned World::umax(unsigned a, unsigned b)
{
    return (a > b)?a:b;
}

std::string World::unsignedToString(unsigned u)
{
    static std::stringstream ss;
    static std::string str;
    ss << u;
    ss >> str;
    ss.clear();
    return str;
}

std::string World::makeString(unsigned num)
{
    if(num < 10000)
        return World::unsignedToString(num);
    if(num < 100000)
        return World::unsignedToString(num / 1000) + "." +
               World::unsignedToString(num / 100 % 10) + "K";
    return World::unsignedToString(num / 1000) + "K";
    //TODO million
}

unsigned World::typeToIndex(Entity::Type type)
{
    return (unsigned) type;
}

Entity::Type World::unsignedToType(unsigned type)
{
    return (Entity::Type) type;
}

float World::getDelta() const
{
    return delta;
}

Entity *World::getPrototype(Entity::Type t)
{
    switch(t)
    {
    case Entity::CITY:
        return cityPrototype;
    case Entity::FARM:
        return farmPrototype;
    case Entity::LUMBERMILL:
        return lumbermillPrototype;
    case Entity::QUARRY:
        return quarryPrototype;
    case Entity::GOLDMINE:
        return goldminePrototype;
    case Entity::BARRACKS:
        return barracksPrototype;
    case Entity::TOWER:
        return towerPrototype;
    case Entity::HOSPITAL:
        return hospitalPrototype;
    case Entity::TRADEPOST:
        return tradepostPrototype;
    case Entity::VILLAGE:
        return nullptr;//TODO
    case Entity::INFANTRY:
        return infantryPrototype;
    case Entity::CAVALRY:
        return cavalryPrototype;
    case Entity::ARCHER:
        return archerPrototype;
    case Entity::SIEGE:
        return siegePrototype;
    case Entity::MAGE:
        return magePrototype;
    default:
        return nullptr;
    }
}

Building *World::isGoodForBuilding(unsigned x, unsigned y, Entity::Type t) const
{
    unsigned r;
    for(Building *b: buildings)
        if(b->isAlive() && (x != b->getX() || y != b->getY()))
        {
            r = ChebyshevDistance(x, y, b->getX(), b->getY());
            if(r < 2 || (t == b->getType() && r <= b->getBuildRadius()))
                return b;
        }
    return nullptr;
}

Controllable* World::getNeighbour(unsigned x, unsigned y, unsigned r, unsigned i)
{
    for(Controllable *c : units)
        if(c->isAlive() && c->getId() != i &&
           (x != c->getX() || y != c->getY()) &&
           uabs(x, c->getX()) + uabs(y, c->getY()) <= r)
            return c;
    return nullptr;
}


Entity *World::findClosest(unsigned x, unsigned y, SelectType type, unsigned id)
{
    Entity *entity = nullptr;
    bool **visited = new bool*[HEIGHT];
    for(unsigned i = 0; i < HEIGHT; ++i)
    {
        visited[i] = new bool[WIDTH];
        for(unsigned j = 0; j < WIDTH; ++j)
            visited[i][j] = false;
    }
    std::deque<sf::Vector2u> toCheck;
    sf::Vector2u vec(x, y);
    toCheck.push_back(vec);
    while(!toCheck.empty())
    {
        vec = toCheck.front();
        toCheck.pop_front();
        if(vec.x < 0 || vec.x >= WIDTH ||
           vec.y < 0 || vec.y >= HEIGHT || visited[vec.y][vec.x])
            continue;
        entity = entities[vec.y][vec.x];
        if(entity != nullptr &&
           ((type == ENEMY_UNIT && entity->getId() == 3 - id &&
                dynamic_cast<UnitGroup*>(entity)) ||
            (type == ENEMY_BUILDING && entity->getId() == 3 - id &&
                dynamic_cast<Building*>(entity) &&
                dynamic_cast<Building*>(entity)->isBuilt()) ||
            (type == OWN_UNIT && entity->getId() == id &&
                dynamic_cast<UnitGroup*>(entity)) ||
            (type == OWN_BUILDING && entity->getId() == id &&
                dynamic_cast<Building*>(entity) &&
                dynamic_cast<Building*>(entity)->isBuilt()) ||
            (type == SPAWN_POINT && entity->getId() == id && (
               entity->getType() == Entity::BARRACKS ||
               entity->getType() == Entity::CITY) &&
               dynamic_cast<Building*>(entity)->isBuilt()) ||
            (type == HOSPITAL && entity->getId() == id &&
                entity->getType() == Entity::HOSPITAL &&
                dynamic_cast<Building*>(entity)->isBuilt())))
            break;
        visited[vec.y][vec.x] = true;
        if(vec.y > 0)
            toCheck.push_back(sf::Vector2u(vec.x, vec.y - 1));
        if(vec.y < HEIGHT - 1)
            toCheck.push_back(sf::Vector2u(vec.x, vec.y + 1));
        if(vec.x > 0)
            toCheck.push_back(sf::Vector2u(vec.x - 1, vec.y));
        if(vec.x  < WIDTH - 1)
            toCheck.push_back(sf::Vector2u(vec.x + 1, vec.y));
    }
    for(unsigned i = 0; i < HEIGHT; ++i)
        delete visited[i];
    delete visited;
    return entity;
}

sf::Vector2u World::findClosestEmptyTile(unsigned x, unsigned y)
{
    bool **visited = new bool*[HEIGHT];
    for(unsigned i = 0; i < HEIGHT; ++i)
    {
        visited[i] = new bool[WIDTH];
        for(unsigned j = 0; j < WIDTH; ++j)
            visited[i][j] = false;
    }
    std::deque<sf::Vector2u> toCheck;
    sf::Vector2u vec(x, y);
    toCheck.push_back(vec);
    while(!toCheck.empty())
    {
        vec = toCheck.front();
        toCheck.pop_front();
        if(vec.x < 0 || vec.x >= WIDTH ||
           vec.y < 0 || vec.y >= HEIGHT || visited[vec.y][vec.x])
            continue;
        if(walkable[vec.y][vec.x])
            break;
        visited[vec.y][vec.x] = true;
        if(vec.y > 0)
            toCheck.push_back(sf::Vector2u(vec.x, vec.y - 1));
        if(vec.y < HEIGHT - 1)
            toCheck.push_back(sf::Vector2u(vec.x, vec.y + 1));
        if(vec.x > 0)
            toCheck.push_back(sf::Vector2u(vec.x - 1, vec.y));
        if(vec.x  < WIDTH - 1)
            toCheck.push_back(sf::Vector2u(vec.x + 1, vec.y));
    }
    for(unsigned i = 0; i < HEIGHT; ++i)
        delete visited[i];
    delete visited;
    return vec;
}

float World::get(float x, float y, unsigned octaves) const
{
    float frequency = 0.05f, amplitude = 1.0f, scale = 0.0f, total = 0.0f;
    for(unsigned i = 0; i < octaves; ++i)
    {
        total += interpolatedNoise(x * frequency, y * frequency) * amplitude;
        scale += amplitude;
        frequency *= 2.0f;
        amplitude *= 0.5f;
    }
    return total / scale;
}

double World::value(long x, long y) const
{
    unsigned long n = x + y * 563;
    n = ((n + seed) << 13) ^ n;
    return (1.0 - ((n * (n * n * 15731 + 789221) + seed) & 0x7fffffff) / 1073741824.0);
}

double World::value2(long x, long y) const
{
    unsigned long n = y + x * 367;
    n = ((n + seed) << 11) ^ n;
    return (1.0 - ((n * (n * n * 20183 + 815279) + seed) & 0x7fffffff) / 1073741824.0);
}

float World::LinearInterpolate(float a, float b, float c) const
{
    return a + c * (b - a);
}

inline int World::fastFloor(float x)
{
    return x > 0 ? (int) x : (int) x - 1;
}

inline float World::dot(float gx, float gy, float x, float y) const
{
    return gx * x + gy * y;
}

float World::interpolatedNoise(float x, float y) const
{
    long integerX = fastFloor(x);
    long integerY = fastFloor(y);
    float fx = x - integerX;
    float fy = y - integerY;
    float tx = fx * fx * fx * (fx * (fx * 6 - 15) + 10);
    float ty = fy * fy * fy * (fy * (fy * 6 - 15) + 10);
    return LinearInterpolate(LinearInterpolate(dot(value(integerX, integerY),
                                                   value2(integerX, integerY),
                                                   fx    , fy),
                                               dot(value(integerX + 1, integerY),
                                                   value2(integerX + 1, integerY),
                                                   fx - 1, fy),
                                               tx),
                             LinearInterpolate(dot(value(integerX, integerY + 1),
                                                   value2(integerX, integerY + 1),
                                                   fx    , fy - 1),
                                               dot(value(integerX + 1, integerY + 1),
                                                   value2(integerX + 1, integerY + 1),
                                                   fx - 1, fy - 1),
                                               tx),
                             ty);
}

float World::remap(float height)
{
    if(height <= 0.5f && height >= -0.5)
        return height * 1.8f;
    else if(height > 0)
        return (height - 0.5f) * 0.2f + 0.9f;
    else
        return (height + 0.5f) * 0.2f - 0.9f;
    return height;
}
