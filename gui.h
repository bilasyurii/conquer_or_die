#ifndef GUI_H
#define GUI_H

#include "button.h"
#include "entity.h"
#include "world.h"
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

class TextParticleEmitter;
class Player;
class Entity;
class Building;
class Element;
class UnitList;

class GUI : public sf::Drawable
{
public:
    enum State
    {
        MAIN_MENU,
        UNIT_ACTIONS,
        SLOT_ACTIONS,
        HERO_SELECTED,
        SELECTING_BUILDING,
        SELECTING_TARGET_LOCATION,
        SELECTING_TARGET_ENEMY,
        TRAINING_UNITS,
        PAYING_FOR_UNITS,
        SPAWNING_UNITS,
        SELECTING_SPAWNPOINT,
        CHANGING_SLOT,
        SPLITTING_GROUP,
        MERGING_GROUPS,
        SELECTING_DEFEND_LOCATION
    };
    enum SelectDirection
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    GUI(unsigned _id);
    virtual ~GUI();
    void update(float delta);
    void processKeyPress(sf::Event::KeyEvent e);
    void deathNotify(Entity *e);
    void updateMenu();
    void trainFinishedNotify(unsigned i);
    void trainProgressNotify(unsigned i, float f);
    void lastEntityDeathNotify();
    void updateSpawningInfo();
    void updatePlayerUnitList();
    void updateSelection();
private:
    unsigned id;
    unsigned activeButton;
    State state;
    Player *player;
    unsigned buttonCount;
    Entity *selectedEntity;
    unsigned selectedIndex;
    unsigned selectedX, selectedY, areaRadius;
    bool buildingOk;
    Entity::Type buildingType;
    Building *badBuilding;
    std::map<State, std::vector<Element*>* > elements;
    std::vector<Button*> buttons;
    Entity::Type trainingType;
    bool isEnoughResources;
    unsigned costs[4];
    unsigned enabledButtons;
    std::vector<Element*> staticElements;
    UnitList *playerUnits;
    bool slotOk;
    unsigned selectedSlot;

    static sf::Text text;
    static sf::Sprite sprite;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void unitActions();
    void slotActions();
    void selectEnemy();
    void selectHero();
    void moveUnit();
    void selectBuilding();
    void trainUnits();
    void makeMenu(State s);
    void trainInfantry();
    void trainCavalry();
    void trainArcher();
    void trainSiege();
    void trainMage();
    void startTraining();
    void increaseTrainingCount();
    void decreaseTrainingCount();
    void updateTrainDataLabels();
    void updateBuildDataLabels();
    void updateCostColors(unsigned panelIndex);
    void updateResourcesLabels();
    void menuNext();
    void menuPrev();
    void updateBuildingType();
    void updateButtonsArray();
    void prepareSpawning();
    void spawnUnits();
    void increaseSpawningCount(bool faster = false);
    void decreaseSpawningCount(bool faster = false);
    void spawningNext();
    void spawningPrev();
    void selectEntity(unsigned x, unsigned y,
                      SelectDirection dir, World::SelectType type);
    void updateButtons();
    void checkSlotForSpawning();
    void changeSlot();
    void confirmSlot();
    void splitGroup();
    void confirmSplit();
    void checkSlotForSplitting();
    void increaseSplittingCount(bool faster = false);
    void decreaseSplittingCount(bool faster = false);
    void updateSplittingInfo();
    void mergeGroups();
    void confirmMerge();
    void checkSlotForMerging();
    void moveToBarracks();
    void hideMoveToBarracksErrors();
    void healUnit();
    void hideHealErrors();
    void defend();

    friend class Button;
    friend class Label;
    friend class TextParticle;
};

#endif // GUI_H
