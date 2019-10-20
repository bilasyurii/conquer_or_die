#include "gui.h"
#include "player.h"
#include "city.h"
#include "path.h"
#include "tower.h"
#include "entity.h"
#include "element.h"
#include "label.h"
#include "healthbar.h"
#include "textparticleemitter.h"
#include "barracks.h"
#include "imageelement.h"
#include "resourcepoint.h"
#include "resourcemanager.h"
#include "costpanel.h"
#include "unitlist.h"
#include "slider.h"
#include <string>
#include <deque>

sf::Text GUI::text;
sf::Sprite GUI::sprite;

GUI::GUI(unsigned _id) :
    id(_id), activeButton(0), state(MAIN_MENU),
    selectedEntity(nullptr), selectedIndex(0), buildingType(Entity::TOWER)
{
    static bool initialized = false;
    if(!initialized)
    {
        text.setFont(ResourceManager::getInstance().getFont());
        sprite.setTexture(ResourceManager::getInstance().getTexture());
        initialized = true;
    }
    player = World::getInstance().getPlayer(id);
    selectedX = player->getCity()->getX();
    selectedY = player->getCity()->getY();
    std::vector<Element*> *menu = new std::vector<Element*>();
    menu->push_back(new Button(10, 10, 130, 40, "Юніти", unitActions)); //ok
    menu->push_back(new Button(10, 60, 130, 40, "Герой", selectHero)); //ok
    menu->push_back(new Button(10, 110, 130, 40, "Слоти", slotActions)); //ok
    menu->push_back(new Button(10, 160, 130, 40, "Тренувати", trainUnits)); //ok
    menu->push_back(new Button(10, 210, 130, 40, "Побудувати", selectBuilding));//ok
    menu->push_back(new Button(10, 260, 130, 40, "Торгувати", unitActions));
    elements[MAIN_MENU] = menu;
    menu = new std::vector<Element*>();
    menu->push_back(new Button(10, 10, 130, 40, "Перемістити", moveUnit)); //ok
    menu->push_back(new Button(10, 60, 130, 40, "Атакувати юніт", selectEnemy)); //ok
    menu->push_back(new Button(10, 110, 130, 40, "Атакувати\n   будівлю", unitActions));
    menu->push_back(new Button(10, 160, 130, 40, "Розумна атака", unitActions));
    menu->push_back(new Button(10, 210, 130, 40, "Лікувати", healUnit));
    menu->push_back(new Button(10, 260, 130, 40, "Захист", defend));
    menu->push_back(new Label(75, 335, "   Юніт вже\nвилікований", Label::CENTER, sf::Color::Red));
    menu->back()->setVisibility(false);
    menu->push_back(new Label(75, 335, "Надто далеко\n   від лікарні", Label::CENTER, sf::Color::Red));
    menu->back()->setVisibility(false);
    menu->push_back(new Label(75, 335, "     Оберіть\nвільне місце", Label::CENTER, sf::Color(180, 130, 60)));
    elements[UNIT_ACTIONS] = menu;
    menu = new std::vector<Element*>();
    menu->push_back(new Button(10, 10, 130, 40, "Перемістити\n  в казарми", moveToBarracks)); //ok
    menu->push_back(new Button(10, 60, 130, 40, "Спавн", prepareSpawning)); //ok
    menu->push_back(new Button(10, 110, 130, 40, "Об'єднати", mergeGroups)); //ok
    menu->push_back(new Button(10, 160, 130, 40, "Розділити", splitGroup)); //ok
    menu->push_back(new Button(10, 210, 130, 40, "Змінити слот", changeSlot));//ok
    menu->push_back(new Label(75, 290, "    Оберіть\nновий слот", Label::CENTER, sf::Color(180, 130, 60)));
    menu->back()->setVisibility(false);
    menu->push_back(new Button(10, 210, 130, 40, "Підтвердити", confirmSlot));//ok
    menu->back()->setVisibility(false);
    menu->push_back(new Button(10, 110, 130, 40, "Підтвердити", confirmMerge)); //ok
    menu->back()->setVisibility(false);
    menu->push_back(new Label(75, 290, "Оберіть юніт\n того ж типу", Label::CENTER, sf::Color(180, 130, 60)));
    menu->back()->setVisibility(false);
    menu->push_back(new Label(75, 340, "Цей слот не\n  підходить", Label::CENTER, sf::Color::Red));
    menu->back()->setVisibility(false);
    menu->push_back(new Label(75, 340, "     Цей юніт\nнадто далеко", Label::CENTER, sf::Color::Red));
    menu->back()->setVisibility(false);
    menu->push_back(new Label(75, 300, "Надто далеко\n    від казарм\n     або міста", Label::CENTER, sf::Color::Red));
    menu->back()->setVisibility(false);
    menu->push_back(new Label(75, 285, "Юніт має бути\n  вилікований", Label::CENTER, sf::Color::Red));    menu->back()->setVisibility(false);
    elements[SLOT_ACTIONS] = menu;
    menu = new std::vector<Element*>();
    menu->push_back(new UnitList(10, 10, 5, 5, 5));
    menu->push_back(new Button(10, 52, 60, 40, "", spawningPrev));
    sf::Sprite arrowImage(ResourceManager::getInstance().getTexture(),
                          sf::IntRect(100, 40, 20, 20));
    menu->push_back(new ImageElement(30, 62, arrowImage));
    menu->push_back(new Button(80, 52, 60, 40, "", spawningNext));
    arrowImage.setTextureRect(sf::IntRect(120, 40, -20, 20));
    menu->push_back(new ImageElement(100, 62, arrowImage));
    menu->push_back(new Slider(10, 112, 90, 10, 1000, 10));
    menu->push_back(new Label(107, 113, "10", Label::LEFT, sf::Color::White));
    menu->push_back(new Button(10, 132, 130, 40, "Заспаунити", spawnUnits)); //ok
    menu->push_back(new Label(75, 210, "      Оберіть\nмісце спауну", Label::CENTER, sf::Color(180, 130, 60)));
    menu->back()->setVisibility(false);
    menu->push_back(new Label(75, 330, "Цей слот\nзайнятий", Label::CENTER, sf::Color::Red));
    menu->back()->setVisibility(false);
    elements[SPAWNING_UNITS] = menu;
    menu = new std::vector<Element*>();
    menu->push_back(new Button(10, 10, 130, 40, "Перемістити", moveUnit));
    menu->push_back(new Button(10, 60, 130, 40, "Використати\n    здібність", unitActions));
    elements[HERO_SELECTED] = menu;
    menu = new std::vector<Element*>();
    menu->push_back(new Button(10, 10, 130, 40, "Вежа", unitActions)); //ok
    menu->push_back(new Button(10, 60, 130, 40, "Ресурси", unitActions)); //ok
    menu->push_back(new Button(10, 110, 130, 40, "Казарми", unitActions)); //ok
    menu->push_back(new Button(10, 160, 130, 40, "Лікарня", unitActions)); //ok
    menu->push_back(new Button(10, 210, 130, 40, "Торговий пост", unitActions));
    menu->push_back(new CostPanel(10, 290));
    menu->push_back(new Label(75, 270, "Вартість", Label::CENTER, sf::Color(180, 130, 60)));
    elements[SELECTING_BUILDING] = menu;
    menu = new std::vector<Element*>();
    menu->push_back(new Label(75, 20, "Мечники", Label::CENTER, sf::Color(180, 130, 60)));
    menu->push_back(new Button(10, 35, 130, 40, "Тренувати", trainInfantry)); //ok
    menu->push_back(new ProgressBar(10, 35, 130, 40, 0.0f));
    menu->push_back(new Label(75, 90, "Кавалерія", Label::CENTER, sf::Color(180, 130, 60)));
    menu->push_back(new Button(10, 105, 130, 40, "Тренувати", trainCavalry)); //ok
    menu->push_back(new ProgressBar(10, 105, 130, 40, 0.0f));
    menu->push_back(new Label(75, 160, "Лучники", Label::CENTER, sf::Color(180, 130, 60)));
    menu->push_back(new Button(10, 175, 130, 40, "Тренувати", trainArcher)); //ok
    menu->push_back(new ProgressBar(10, 175, 130, 40, 0.0f));
    menu->push_back(new Label(75, 230, "Осада", Label::CENTER, sf::Color(180, 130, 60)));
    menu->push_back(new Button(10, 245, 130, 40, "Тренувати", trainSiege)); //ok
    menu->push_back(new ProgressBar(10, 245, 130, 40, 0.0f));
    menu->push_back(new Label(75, 300, "Маги", Label::CENTER, sf::Color(180, 130, 60)));
    menu->push_back(new Button(10, 315, 130, 40, "Тренувати", trainMage)); //ok
    menu->push_back(new ProgressBar(10, 315, 130, 40, 0.0f));
    for(unsigned i = 0; i < 5; ++i)
        menu->at(i * 3 + 2)->setVisibility(false);
    elements[TRAINING_UNITS] = menu;
    menu = new std::vector<Element*>();
    menu->push_back(new Button(10, 120, 30, 30, "-", decreaseTrainingCount));
    static_cast<Button*>(menu->back())->disable();
    menu->push_back(new Button(110, 120, 30, 30, "+", increaseTrainingCount));
    menu->push_back(new Button(10, 170, 130, 40, "", startTraining));
    menu->push_back(new Label(75, 135, "10", Label::CENTER, sf::Color::White));
    menu->push_back(new CostPanel(10, 10));
    elements[PAYING_FOR_UNITS] = menu;
    menu = new std::vector<Element*>();
    menu->push_back(new Slider(10, 70, 90, 10, 90, 5));
    menu->push_back(new Label(107, 71, "10", Label::LEFT, sf::Color::White));
    menu->push_back(new Button(10, 250, 130, 40, "Підтвердити", confirmSplit));
    menu->push_back(new Label(75, 330, "       Оберіть\nпорожній слот", Label::CENTER, sf::Color::Red));
    menu->push_back(new Label(75, 30, "   Відсотків\nв новій групі:", Label::CENTER, sf::Color(180, 130, 60)));
    menu->push_back(new Label(75, 100, "Нова група", Label::CENTER, sf::Color(180, 130, 60)));
    menu->push_back(new Label(60, 125, "Сила", Label::RIGHT, sf::Color(180, 130, 60)));
    menu->push_back(new Label(90, 125, "Юніти", Label::LEFT, sf::Color(180, 130, 60)));
    menu->push_back(new Label(60, 150, "", Label::RIGHT, sf::Color::White));
    menu->push_back(new Label(90, 150, "", Label::LEFT, sf::Color::White));
    menu->push_back(new Label(75, 180, "Стара група", Label::CENTER, sf::Color(180, 130, 60)));
    menu->push_back(new Label(60, 205, "Сила", Label::RIGHT, sf::Color(180, 130, 60)));
    menu->push_back(new Label(90, 205, "Юніти", Label::LEFT, sf::Color(180, 130, 60)));
    menu->push_back(new Label(60, 230, "", Label::RIGHT, sf::Color::White));
    menu->push_back(new Label(90, 230, "", Label::LEFT, sf::Color::White));
    elements[SPLITTING_GROUP] = menu;
    sf::Sprite unitIcon(ResourceManager::getInstance().getTexture(),
                        sf::IntRect(0, 100, 10, 10));
    staticElements.push_back(new ImageElement(122, 520, unitIcon));
    staticElements.push_back(new Label(70, 524, "0",
                                       Label::RIGHT, sf::Color::White));
    staticElements.push_back(new Label(72, 524, "+0",
                                       Label::LEFT, sf::Color::Green));
    unitIcon.setTextureRect(sf::IntRect(10, 100, 10, 10));
    staticElements.push_back(new ImageElement(122, 540, unitIcon));
    staticElements.push_back(new Label(70, 544, "0",
                                       Label::RIGHT, sf::Color::White));
    staticElements.push_back(new Label(72, 544, "+0",
                                       Label::LEFT, sf::Color::Green));
    unitIcon.setTextureRect(sf::IntRect(20, 100, 10, 10));
    staticElements.push_back(new ImageElement(122, 560, unitIcon));
    staticElements.push_back(new Label(70, 564, "0",
                                       Label::RIGHT, sf::Color::White));
    staticElements.push_back(new Label(72, 564, "+0",
                                       Label::LEFT, sf::Color::Green));
    unitIcon.setTextureRect(sf::IntRect(30, 100, 10, 10));
    staticElements.push_back(new ImageElement(122, 580, unitIcon));
    staticElements.push_back(new Label(70, 584, "0",
                                       Label::RIGHT, sf::Color::White));
    staticElements.push_back(new Label(72, 584, "+0",
                                       Label::LEFT, sf::Color::Green));
    playerUnits = new UnitList(10, 445, 14, 4, Player::MAX_UNITS);
    staticElements.push_back(playerUnits);
    staticElements.push_back(new HealthBar(56, 370, 84, 20));
    unitIcon.setScale(2.0f, 2.0f);
    unitIcon.setTextureRect(sf::IntRect(0, 20, 20, 20));
    staticElements.push_back(new ImageElement(10, 370, unitIcon));
    staticElements.push_back(new Label(98, 380, "/", Label::CENTER, sf::Color::White, 8));
    staticElements.push_back(new Label(93, 380, "", Label::RIGHT, sf::Color::White, 8));
    staticElements.push_back(new Label(103, 380, "", Label::LEFT, sf::Color::White, 8));
    unitIcon.setScale(1.0f, 1.0f);
    unitIcon.setTextureRect(sf::IntRect(40, 100, 10, 10));
    staticElements.push_back(new ImageElement(56, 395, unitIcon));
    unitIcon.setTextureRect(sf::IntRect(50, 100, 10, 10));
    staticElements.push_back(new ImageElement(56, 410, unitIcon));
    unitIcon.setTextureRect(sf::IntRect(60, 100, 10, 10));
    staticElements.push_back(new ImageElement(56, 425, unitIcon));
    staticElements.push_back(new Label(75, 400, "", Label::LEFT, sf::Color::White, 8));
    staticElements.push_back(new Label(75, 415, "", Label::LEFT, sf::Color::White, 8));
    staticElements.push_back(new Label(75, 430, "", Label::LEFT, sf::Color::White, 8));
    if(id == 2)
    {
        for(auto it = elements.begin(); it != elements.end(); ++it)
            for(Element *e : *(it->second))
                e->setX(e->getX() + World::SCREEN_WIDTH + World::MENU_WIDTH);
        for(Element *e : staticElements)
            e->setX(e->getX() + World::SCREEN_WIDTH + World::MENU_WIDTH);
    }
    elements[SELECTING_TARGET_LOCATION] = elements[UNIT_ACTIONS];
    elements[SELECTING_TARGET_ENEMY] = elements[UNIT_ACTIONS];
    elements[SELECTING_DEFEND_LOCATION] = elements[UNIT_ACTIONS];
    elements[SELECTING_SPAWNPOINT] = elements[SPAWNING_UNITS];
    elements[CHANGING_SLOT] = elements[SLOT_ACTIONS];
    elements[MERGING_GROUPS] = elements[SLOT_ACTIONS];
    makeMenu(state);
}

void GUI::healUnit()
{
    UnitGroup *slot = player->getActiveSlot();
    if(slot->isHealthy())
    {
        std::vector<Element*> &menu = *(elements[state]);
        menu[6]->setVisibility(true);
        menu[7]->setVisibility(false);
    }
    else if(World::ManhattanDistance(World::getInstance().findClosest(
                                                slot->getX(), slot->getY(),
                                                World::HOSPITAL, id),
                                     slot) > 3)
    {
        std::vector<Element*> &menu = *(elements[state]);
        menu[6]->setVisibility(false);
        menu[7]->setVisibility(true);
    }
    else
    {
        slot->startHealing();
        hideHealErrors();
    }
}

void GUI::hideHealErrors()
{
    std::vector<Element*> &menu = *(elements[state]);
    menu[6]->setVisibility(false);
    menu[7]->setVisibility(false);
}

void GUI::hideMoveToBarracksErrors()
{
    std::vector<Element*> &menu = *(elements[state]);
    menu[11]->setVisibility(false);
    menu[12]->setVisibility(false);
}

void GUI::moveToBarracks()
{
    UnitGroup *slot = player->getActiveSlot();
    if(!slot->isHealthy())
    {
        std::vector<Element*> &menu = *(elements[state]);
        menu[11]->setVisibility(false);
        menu[12]->setVisibility(true);
    }
    else if(World::ManhattanDistance(World::getInstance().findClosest(
                                                slot->getX(), slot->getY(),
                                                World::SPAWN_POINT, id),
                                     slot) > 3)
    {
        std::vector<Element*> &menu = *(elements[state]);
        menu[11]->setVisibility(true);
        menu[12]->setVisibility(false);
    }
    else
    {
        player->addUnitsToBarracks(World::typeToIndex(slot->getType()), slot->getCount());
        World::getInstance().removeEntity(slot);
        player->clearSlot(playerUnits->getSelection());
        playerUnits->clearSlot(playerUnits->getSelection());
        hideMoveToBarracksErrors();
        updateSelection();
        delete slot;
    }
}

void GUI::checkSlotForMerging()
{
    std::vector<Element*> &menu = *(elements[state]);
    UnitGroup *slot = player->getActiveSlot();
    UnitGroup *current = player->getUnitSlot(selectedSlot);
    if(!slot || selectedSlot == playerUnits->getSelection() ||
       slot->getType() != current->getType())
    {
        menu[9]->setVisibility(true);
        menu[10]->setVisibility(false);\
        buttons[4]->disable();
        enabledButtons = 0;
    }
    else if(World::ChebyshevDistance(slot, current) > 2)
    {
        menu[9]->setVisibility(false);
        menu[10]->setVisibility(true);
        buttons[4]->disable();
        enabledButtons = 0;
    }
    else
    {
        menu[9]->setVisibility(false);
        menu[10]->setVisibility(false);
        buttons[4]->enable();
        updateButtons();
    }
}

void GUI::confirmMerge()
{
    UnitGroup *original = player->getUnitSlot(selectedSlot);
    UnitGroup *group = player->getActiveSlot();
    original->merge(*group);
    World::getInstance().removeEntity(group);
    delete group;
    unsigned removalIndex = playerUnits->getSelection();
    player->clearSlot(removalIndex);
    std::vector<Element*> &menu = *(elements[state]);
    menu[2]->setVisibility(true);
    menu[7]->setVisibility(false);
    menu[8]->setVisibility(false);
    menu[9]->setVisibility(false);
    menu[10]->setVisibility(false);
    makeMenu(SLOT_ACTIONS);
    playerUnits->selectSlot(selectedSlot);
    player->selectSlot(selectedSlot);
}

void GUI::mergeGroups()
{
    selectedSlot = playerUnits->getSelection();
    std::vector<Element*> &menu = *(elements[state]);
    menu[2]->setVisibility(false);
    menu[7]->setVisibility(true);
    menu[8]->setVisibility(true);
    makeMenu(MERGING_GROUPS);
    checkSlotForMerging();
}

void GUI::updateSplittingInfo()
{
    std::vector<Element*> &menu = *(elements[state]);
    Slider *slider = static_cast<Slider*>(menu[0]);
    unsigned splitPercent = slider->getValue();
    UnitGroup *originalGroup = player->getUnitSlot(selectedSlot);
    static_cast<Label*>(menu[1])->setString(World::unsignedToString(splitPercent));
    std::pair<UnitGroup*, UnitGroup*> groups = originalGroup->getParts(splitPercent);
    UnitGroup *newGroup = groups.first;
    originalGroup = groups.second;
    static_cast<Label*>(menu[8])->setString(
            World::unsignedToString(newGroup->getDamage()));
    static_cast<Label*>(menu[9])->setString(
            World::unsignedToString(newGroup->getCount()));
    static_cast<Label*>(menu[13])->setString(
            World::unsignedToString(originalGroup->getDamage()));
    static_cast<Label*>(menu[14])->setString(
            World::unsignedToString(originalGroup->getCount()));
    delete newGroup;
    delete originalGroup;
}

void GUI::increaseSplittingCount(bool faster)
{
    std::vector<Element*> &menu = *(elements[state]);
    Slider *slider = static_cast<Slider*>(menu[0]);
    slider->increase(faster);
    updateSplittingInfo();
}

void GUI::decreaseSplittingCount(bool faster)
{
    std::vector<Element*> &menu = *(elements[state]);
    Slider *slider = static_cast<Slider*>(menu[0]);
    slider->decrease(faster);
    updateSplittingInfo();
}

void GUI::checkSlotForSplitting()
{
    if(player->getActiveSlot())
    {
        buttons[1]->disable();
        (*(elements[state]))[3]->setVisibility(true);
    }
    else
    {
        buttons[1]->enable();
        (*(elements[state]))[3]->setVisibility(false);
    }
}

void GUI::confirmSplit()
{
    std::vector<Element*> &menu = *(elements[state]);
    Slider *slider = static_cast<Slider*>(menu[0]);
    unsigned splitPercent = slider->getValue();
    UnitGroup *group = player->getUnitSlot(selectedSlot);
    sf::Vector2u spawnpoint = World::getInstance().findClosestEmptyTile(
                                            group->getX(), group->getY());
    group = group->split(splitPercent);
    World::getInstance().addUnit(group, spawnpoint.x, spawnpoint.y);
    makeMenu(SLOT_ACTIONS);
}

void GUI::splitGroup()
{
    selectedSlot = playerUnits->getSelection();
    (*(elements[state]))[3]->setVisibility(true);
    makeMenu(SPLITTING_GROUP);
}

void GUI::confirmSlot()
{
    player->swapSlots(selectedSlot, playerUnits->getSelection());
    std::vector<Element*> &menu = *(elements[state]);
    menu[4]->setVisibility(true);
    menu[5]->setVisibility(false);
    menu[6]->setVisibility(false);
    makeMenu(SLOT_ACTIONS);
}

void GUI::changeSlot()
{
    selectedSlot = playerUnits->getSelection();
    std::vector<Element*> &menu = *(elements[state]);
    menu[4]->setVisibility(false);
    menu[5]->setVisibility(true);
    menu[6]->setVisibility(true);
    makeMenu(CHANGING_SLOT);
}


void GUI::prepareSpawning()
{
    makeMenu(SPAWNING_UNITS);
}

void GUI::spawnUnits()
{
    std::vector<Element*> &menu = *(elements[state]);
    menu[8]->setVisibility(true);
    state = SELECTING_SPAWNPOINT;
    selectedEntity = World::getInstance().getPlayer(id)->getCity();
    checkSlotForSpawning();
}

void GUI::increaseSpawningCount(bool faster)
{
    std::vector<Element*> &menu = *(elements[state]);
    Slider *slider = static_cast<Slider*>(menu[5]);
    slider->increase(faster);
    static_cast<Label*>(menu[6])->setString(World::unsignedToString(slider->getValue()));
}

void GUI::decreaseSpawningCount(bool faster)
{
    std::vector<Element*> &menu = *(elements[state]);
    Slider *slider = static_cast<Slider*>(menu[5]);
    slider->decrease(faster);
    static_cast<Label*>(menu[6])->setString(World::unsignedToString(slider->getValue()));
}

void GUI::spawningNext()
{
    static_cast<UnitList*>((*(elements[state]))[0])->next();
    updateSpawningInfo();
}

void GUI::spawningPrev()
{
    static_cast<UnitList*>((*(elements[state]))[0])->prev();
    updateSpawningInfo();
}

void GUI::startTraining()
{
    if(!isEnoughResources)
        return;
    player->getBarracks()->startTraining(trainingType);
    //TODO add units to barracks
    player->spend(costs[0], costs[1], costs[2], costs[3]);
    std::vector<Element*> &menu = *(elements[TRAINING_UNITS]);
    unsigned index = 0;
    switch(trainingType)
    {
    case Entity::INFANTRY: index = 0; break;
    case Entity::CAVALRY: index = 1; break;
    case Entity::ARCHER: index = 2; break;
    case Entity::SIEGE: index = 3; break;
    case Entity::MAGE: index = 4; break;
    default: break;
    }
    static_cast<Button*>(menu[index * 3 + 1])->disable();
    --enabledButtons;
    menu[index * 3 + 2]->setVisibility(true);
    static_cast<ProgressBar*>(menu[index * 3 + 2])->setFactor(0.0f);
    makeMenu(TRAINING_UNITS);
}

void GUI::updateBuildingType() {
    switch(activeButton)
    {
    case 0:
        buildingType = Entity::TOWER;
        break;
    case 1:
        switch(World::getInstance().getBiome(selectedX, selectedY))
        {
        case World::GRASSLAND:
            buildingType = Entity::FARM;
            break;
        case World::SWAMP:
            buildingType = Entity::FARM;
            break;
        case World::DESERT:
            buildingType = Entity::GOLDMINE;
            break;
        case World::FOREST:
            buildingType = Entity::LUMBERMILL;
            break;
        case World::MOUNTAINS:
            buildingType = Entity::QUARRY;
            break;
        default:
            buildingType = Entity::FARM;
            break;
        }
        break;
    case 2:
        buildingType = Entity::BARRACKS;
        break;
    case 3:
        buildingType = Entity::HOSPITAL;
        break;
    case 4:
        buildingType = Entity::TRADEPOST;
        break;
    }
}

void GUI::updateBuildDataLabels()
{
    updateBuildingType();
    std::vector<Element*> &menu = *(elements[state]);
    Building *b = static_cast<Building*>(World::getInstance().getPrototype(buildingType));
    costs[0] = b->getBuildingCost(Entity::FOOD);
    costs[1] = b->getBuildingCost(Entity::WOOD);
    costs[2] = b->getBuildingCost(Entity::STONE);
    costs[3] = b->getBuildingCost(Entity::GOLD);
    CostPanel *costPanel = static_cast<CostPanel*>((*(elements[state]))[5]);
    for(unsigned i = 0; i < 4; ++i)
        costPanel->setCount(costs[i], i);
}

void GUI::updateCostColors(unsigned index)
{
    isEnoughResources = true;
    CostPanel *costPanel = static_cast<CostPanel*>((*(elements[state]))[index]);
    if(costs[0] > player->getFood())
    {
        costPanel->setResourcesEnough(false, 0);
        isEnoughResources = false;
    }
    else
        costPanel->setResourcesEnough(true, 0);
    if(costs[1] > player->getWood())
    {
        costPanel->setResourcesEnough(false, 1);
        isEnoughResources = false;
    }
    else
        costPanel->setResourcesEnough(true, 1);
    if(costs[2] > player->getStone())
    {
        costPanel->setResourcesEnough(false, 2);
        isEnoughResources = false;
    }
    else
        costPanel->setResourcesEnough(true, 2);
    if(costs[3] > player->getGold())
    {
        costPanel->setResourcesEnough(false, 3);
        isEnoughResources = false;
    }
    else
        costPanel->setResourcesEnough(true, 3);
}

void GUI::updateTrainDataLabels()
{
    std::vector<Element*> &menu = *(elements[state]);
    static_cast<Label*>(menu[3])->setString(World::unsignedToString(
            player->getBarracks()->getTrainCount(trainingType)));
    costs[0] = player->getBarracks()->getTrainCost(trainingType, ResourcePoint::WOOD);
    costs[1] = player->getBarracks()->getTrainCost(trainingType, ResourcePoint::FOOD);
    costs[2] = player->getBarracks()->getTrainCost(trainingType, ResourcePoint::STONE);
    costs[3] = player->getBarracks()->getTrainCost(trainingType, ResourcePoint::GOLD);
    CostPanel *costPanel = static_cast<CostPanel*>((*(elements[state]))[4]);
    for(unsigned i = 0; i < 4; ++i)
        costPanel->setCount(costs[i], i);
}

void GUI::increaseTrainingCount()
{
    player->getBarracks()->increaseTrainCount(trainingType, 10);
    updateTrainDataLabels();
    updateCostColors(4);
    buttons[0]->enable();
    if(player->getBarracks()->getTrainCount(trainingType) >= 1000) {
        player->getBarracks()->setTrainCount(trainingType, 1000);
        buttons[1]->disable();
    }
}

void GUI::decreaseTrainingCount()
{
    player->getBarracks()->decreaseTrainCount(trainingType, 10);
    updateTrainDataLabels();
    updateCostColors(4);
    buttons[1]->enable();
    if(player->getBarracks()->getTrainCount(trainingType) <= 10) {
        player->getBarracks()->setTrainCount(trainingType, 10);
        buttons[0]->disable();
    }
}

void GUI::updateButtonsArray()
{
    std::vector<Element*> &menu = *(elements[state]);
    buttons = std::vector<Button*>();
    for(Element *e : menu)
        if(e->isVisible() && dynamic_cast<Button*>(e))
            buttons.push_back(static_cast<Button*>(e));
    buttonCount = buttons.size();
}

void GUI::checkSlotForSpawning()
{
    std::vector<Element*> &menu = *(elements[state]);
    UnitGroup *slot = player->getActiveSlot();
    slotOk = !slot;
    menu[9]->setVisibility(!slotOk);
}

void GUI::updateSpawningInfo()
{
    std::vector<Element*> &menu = *(elements[state]);
    UnitList *unitList = static_cast<UnitList*>(menu[0]);
    unsigned unitCount = player->getUnitsInBarracksCount(unitList->getSelection());
    Slider *slider = static_cast<Slider*>(menu[5]);
    slider->setMinMax(10, unitCount);
    static_cast<Label*>(menu[6])->setString(
            World::unsignedToString(slider->getValue()));
    for(unsigned i = 0; i < 5; ++i)
        unitList->setSlot(i, i, player->getUnitsInBarracksCount(i));
    if(unitCount)
        static_cast<Button*>(menu[7])->enable();
    else
        static_cast<Button*>(menu[7])->disable();
    checkSlotForSpawning();
}

void GUI::makeMenu(State s)
{
    activeButton = 0;
    state = s;
    updateButtonsArray();
    updateMenu();
    if(state == PAYING_FOR_UNITS)
    {
        updateTrainDataLabels();
        updateCostColors(4);
        activeButton = 1;
    }
    else if(state == SELECTING_BUILDING)
    {
        updateBuildDataLabels();
        updateCostColors(5);
    }
    else if(state == UNIT_ACTIONS || state == SLOT_ACTIONS)
        updateSelection();
    else if(state == SPAWNING_UNITS)
        updateSpawningInfo();
    else if(state == CHANGING_SLOT)
    {
        for(Button *b : buttons)
            b->disable();
        buttons[4]->enable();
    }
    else if(state == SPLITTING_GROUP)
    {
        updateSplittingInfo();
        checkSlotForSplitting();
    }
    else if(state == MERGING_GROUPS)
    {
        for(Button *b : buttons)
            b->disable();
        buttons[4]->enable();
    }
    updateButtons();
}

void GUI::updateButtons()
{
    enabledButtons = 0;
    for(unsigned i = 0; i < buttonCount; ++i)
        if(buttons[i]->isEnabled())
            ++enabledButtons;
    if(enabledButtons == 0)
        return;
    while(!(buttons[activeButton]->isEnabled()))
    {
        ++activeButton;
        if(activeButton >= buttonCount)
            activeButton = 0;
    }
}

void GUI::trainInfantry()
{
    trainingType = Entity::INFANTRY;
    makeMenu(PAYING_FOR_UNITS);
    buttons[2]->setString("Тренувати\n   мечників");
}

void GUI::trainCavalry()
{
    trainingType = Entity::CAVALRY;
    makeMenu(PAYING_FOR_UNITS);
    buttons[2]->setString("Тренувати\n кавалерію");
}

void GUI::trainArcher()
{
    trainingType = Entity::ARCHER;
    makeMenu(PAYING_FOR_UNITS);
    buttons[2]->setString("Тренувати\n   лучників");
}

void GUI::trainSiege()
{
    trainingType = Entity::SIEGE;
    makeMenu(PAYING_FOR_UNITS);
    buttons[2]->setString("Тренувати\n     осаду");}

void GUI::trainMage()
{
    trainingType = Entity::MAGE;
    makeMenu(PAYING_FOR_UNITS);
    buttons[2]->setString("Тренувати\n       магів");
}

void GUI::trainUnits()
{
    makeMenu(TRAINING_UNITS);
}

void GUI::unitActions()
{
    makeMenu(UNIT_ACTIONS);
}

void GUI::slotActions()
{
    makeMenu(SLOT_ACTIONS);
}

void GUI::selectHero()
{
    player->selectHero();
    makeMenu(HERO_SELECTED);
}

void GUI::selectBuilding()
{
    makeMenu(SELECTING_BUILDING);
}

void GUI::defend()
{
    state = SELECTING_DEFEND_LOCATION;
    (*(elements[state]))[8]->setVisibility(true);
}

void GUI::moveUnit()
{
    state = SELECTING_TARGET_LOCATION;
    (*(elements[state]))[8]->setVisibility(true);
}

void GUI::selectEnemy()
{
    state = SELECTING_TARGET_ENEMY;
    selectedIndex = 0;
    Player *p = World::getInstance().getPlayer(3 - id);
    if(p->getUnitCount() == 0)
        selectedEntity = nullptr;
    else
        selectedEntity = p->getUnit(0);
}

GUI::~GUI()
{
    elements.erase(SELECTING_TARGET_LOCATION);
    elements.erase(SELECTING_TARGET_ENEMY);
    elements.erase(SELECTING_DEFEND_LOCATION);
    elements.erase(SELECTING_SPAWNPOINT);
    elements.erase(CHANGING_SLOT);
    elements.erase(MERGING_GROUPS);
    for(auto it = elements.begin(); it != elements.end(); ++it)
    {
        for(Element *e : *(it->second))
            delete e;
        delete it->second;
    }
    for(Element *e : staticElements)
        delete e;
}

void GUI::updateMenu() {
    if(state == MAIN_MENU)
    {
        if(player->getBarracks())
            buttons[3]->enable();
        else
            buttons[3]->disable();
        if(player->getTradepost())
            buttons[5]->enable();
        else
            buttons[5]->disable();
    }
    else if(state == UNIT_ACTIONS || state == SELECTING_TARGET_LOCATION ||
            state == SELECTING_TARGET_ENEMY || state == SELECTING_DEFEND_LOCATION)
    {
        if(player->getHospital())
            buttons[4]->enable();
        else
            buttons[4]->disable();
    }
}
//TODO bug when everyone is killed but game want to select someone - it crashes
void GUI::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    static sf::RectangleShape rect;
    rect.setFillColor(sf::Color(246, 226, 165));
    //drawing elements
    Element *e;
    unsigned buttonIndex = 0;
    std::vector<Element*> *menu = elements.at(state);
    for(unsigned i = 0; i < menu->size(); ++i)
    {
        e = menu->at(i);
        if(!(e->isVisible()))
            continue;
        if(buttons[buttonIndex] == e)
        {
            if(enabledButtons != 0 && buttonIndex == activeButton)
            {
                Button *b = static_cast<Button*>(e);
                rect.setSize(sf::Vector2f(b->getWidth() + 4.0f, b->getHeight() + 4.0f));
                rect.setPosition(b->getX() - 2.0f, b->getY() - 2.0f);
                target.draw(rect, states);
            }
            ++buttonIndex;
        }
        target.draw(*e, states);
    }
    for(unsigned i = 0; i < staticElements.size(); ++i)
    {
        e = staticElements[i];
        if(!(e->isVisible()))
            continue;
        target.draw(*e, states);
    }
    if(state == CHANGING_SLOT || state == SPLITTING_GROUP || state == MERGING_GROUPS)
    {
        sprite.setTextureRect(sf::IntRect(60, 30, 20, 10));
        sprite.setPosition(playerUnits->getSlotCoords(selectedSlot));
        sprite.move(0, -10.0f);
        target.draw(sprite, states);
    }
    //drawing selector of building
    if(state == SELECTING_BUILDING)
    {
        if(!buildingOk)
        {
            sprite.setTextureRect(sf::IntRect(100, 0, 20, 20));
            sprite.setPosition(selectedX * World::TILE_SIZE + World::MENU_WIDTH,
                               selectedY * World::TILE_SIZE);
            target.draw(sprite, states);
            if(badBuilding != nullptr)
            {
                sprite.setTextureRect(sf::IntRect(100, 0, 20, 20));
                sprite.setPosition(badBuilding->getX() * World::TILE_SIZE + World::MENU_WIDTH,
                                   badBuilding->getY() * World::TILE_SIZE);
                target.draw(sprite, states);
            }
        }
        else
        {
            int ir = static_cast<Building*>(World::getInstance().getPrototype(
                    buildingType))->getInfluenceRadius();
            World::getInstance().drawBuilding(target, states,
                    selectedX, selectedY, buildingType, id);
            rect.setSize(sf::Vector2f(World::TILE_SIZE, World::TILE_SIZE));
            if(id == 1)
                rect.setFillColor(sf::Color(255, 0, 0, 100));
            else
                rect.setFillColor(sf::Color(0, 0, 255, 100));
            for(int i = -ir; i <= ir; ++i)
                for(int j = -ir; j <= ir; ++j)
                    if(selectedY + i >= 0 && selectedY + i < World::HEIGHT &&
                       selectedX + j >= 0 && selectedX + j < World::WIDTH &&
                       World::getInstance().getOwner(selectedX + j, selectedY + i) == 0)
                    {
                        rect.setPosition((selectedX + j) * World::TILE_SIZE + World::MENU_WIDTH,
                                         (selectedY + i) * World::TILE_SIZE);
                        target.draw(rect, states);
                    }
        }
    }
    //drawing selector of position
    if(state == SELECTING_TARGET_LOCATION || state == SELECTING_BUILDING ||
       state == SELECTING_DEFEND_LOCATION)
    {
        sprite.setTextureRect(sf::IntRect(id * 20, 20, 20, 20));
        sprite.setPosition(selectedX * World::TILE_SIZE + World::MENU_WIDTH,
                           selectedY * World::TILE_SIZE);
        target.draw(sprite, states);
    }
    if(state == SELECTING_TARGET_LOCATION || state == SELECTING_TARGET_ENEMY ||
       state == UNIT_ACTIONS || state == SELECTING_DEFEND_LOCATION)
        World::getInstance().drawPath(target, states, id);
    if((state == SELECTING_TARGET_ENEMY) &&
       selectedEntity)
    {
        sprite.setTextureRect(sf::IntRect(40 + id * 20,
                              (selectedEntity->getY())?30:20, 20, 10));
        sprite.setPosition(selectedEntity->getX() * World::TILE_SIZE + World::MENU_WIDTH,
                           selectedEntity->getY() * (int) World::TILE_SIZE +
                           ((selectedEntity->getY() == 0)?(20):(-10)));
        target.draw(sprite, states);
    }
    sprite.setTextureRect(sf::IntRect(0, 20, 20, 20));
    Entity *entity;
    if(state == SELECTING_SPAWNPOINT)
        entity = selectedEntity;
    else
        entity = player->getActiveEntity();
    if(entity)
    {
        sprite.setPosition(entity->getX() * World::TILE_SIZE + World::MENU_WIDTH,
                           entity->getY() * World::TILE_SIZE);
        target.draw(sprite, states);
    }
}

void GUI::updateResourcesLabels()
{
    static_cast<Label*>(staticElements[1])->setString(player->getFoodString());
    static_cast<Label*>(staticElements[2])->setString(player->getFoodPerSecStr());
    static_cast<Label*>(staticElements[4])->setString(player->getWoodString());
    static_cast<Label*>(staticElements[5])->setString(player->getWoodPerSecStr());
    static_cast<Label*>(staticElements[7])->setString(player->getStoneString());
    static_cast<Label*>(staticElements[8])->setString(player->getStonePerSecStr());
    static_cast<Label*>(staticElements[10])->setString(player->getGoldString());
    static_cast<Label*>(staticElements[11])->setString(player->getGoldPerSecStr());
}

void GUI::update(float delta)
{
    bool playerChanges = player->hasResourceChanges();
    if(playerChanges)
        updateResourcesLabels();
    if(state == SELECTING_BUILDING) {
        updateBuildingType();
        if(playerChanges)
            updateCostColors(5);
        if(!isEnoughResources)
        {
            badBuilding = nullptr;
            buildingOk = false;
        }
        else if(World::getInstance().getBiome(selectedX, selectedY) == World::RIVER ||
           World::getInstance().getBiome(selectedX, selectedY) == World::BRIDGE ||
           !World::getInstance().getWalkable(selectedX, selectedY) ||
           World::getInstance().getOwner(selectedX, selectedY) != id)
        {
            badBuilding = nullptr;
            buildingOk = false;
        }
        else
        {
            badBuilding = World::getInstance().isGoodForBuilding(selectedX, selectedY, buildingType);
            if(badBuilding == nullptr)
                buildingOk = true;
            else
                buildingOk = false;
        }
    }
    else if(state == PAYING_FOR_UNITS)
    {
        if(playerChanges)
            updateCostColors(4);
    }
    if(player->getActiveEntity())
    {
        static_cast<HealthBar*>(staticElements[13])->setFactor(
                player->getActiveEntity()->getHpFactor());
        static_cast<Label*>(staticElements[16])->setString(
                World::makeString(player->getActiveEntity()->getHp()));
        static_cast<Label*>(staticElements[17])->setString(
                World::makeString(player->getActiveEntity()->getMaxHp()));
        static_cast<Label*>(staticElements[21])->setString(
                World::makeString(player->getActiveEntity()->getDamage()));
        static_cast<Label*>(staticElements[22])->setString("150");//TODO armor
        static_cast<Label*>(staticElements[23])->setString("234");//TODO speed
    }
    updatePlayerUnitList();
}

void GUI::deathNotify(Entity *e)
{
    if(player->getActiveEntity() == e) {
        World::getInstance().getPlayer(id)->checkDeaths();
        updateSelection();
        if(state == SELECTING_TARGET_LOCATION || state == SELECTING_TARGET_ENEMY ||
           state == SELECTING_DEFEND_LOCATION)
            makeMenu(UNIT_ACTIONS);
        else if(state == UNIT_ACTIONS)
            ;//TODO
    }
    else if(selectedEntity == e)
    {
        selectedIndex = 0;
        Player *p = World::getInstance().getPlayer(3 - id);
        if(p->getUnitCount() == 0)
            selectedEntity = nullptr;
        else
            selectedEntity = p->getUnit(0);
    }
}

void GUI::lastEntityDeathNotify()
{
    if(state == UNIT_ACTIONS || state == SELECTING_TARGET_LOCATION ||
       state == SELECTING_TARGET_ENEMY || state == SELECTING_DEFEND_LOCATION)
        makeMenu(MAIN_MENU);
}

void GUI::trainFinishedNotify(unsigned i)
{
    std::vector<Element*> &menu = *(elements[TRAINING_UNITS]);
    static_cast<Button*>(menu[i * 3 + 1])->enable();
    if(state == TRAINING_UNITS) {
        if(enabledButtons == 0)
        {
            activeButton = 0;
            while(activeButton < buttonCount &&
                  !(buttons[activeButton]->isEnabled()))
                ++activeButton;
        }
        ++enabledButtons;
    }
    menu[i * 3 + 2]->setVisibility(false);
    updateButtonsArray();
}

void GUI::trainProgressNotify(unsigned i, float f)
{
    std::vector<Element*> &menu = *(elements[TRAINING_UNITS]);
    static_cast<ProgressBar*>(menu[i * 3 + 2])->setFactor(f);
}

void GUI::menuNext()
{
    if(enabledButtons == 0)
        return;
    ++activeButton;
    if(activeButton == buttonCount)
        activeButton = 0;
    if(!buttons[activeButton]->isEnabled())
        menuNext();
    if(state == SELECTING_BUILDING)
    {
        updateBuildDataLabels();
        updateCostColors(5);
    }
    else if(state == UNIT_ACTIONS)
        hideHealErrors();
    else if(state == SLOT_ACTIONS)
        hideMoveToBarracksErrors();
}

void GUI::menuPrev()
{
    if(enabledButtons == 0)
        return;
    if(activeButton > 0)
        --activeButton;
    else
        activeButton = buttonCount - 1;
    if(!buttons[activeButton]->isEnabled())
        menuPrev();
    if(state == SELECTING_BUILDING)
    {
        updateBuildDataLabels();
        updateCostColors(5);
    }
    else if(state == UNIT_ACTIONS)
        hideHealErrors();
    else if(state == SLOT_ACTIONS)
        hideMoveToBarracksErrors();
}

void GUI::selectEntity(unsigned x, unsigned y,
                       SelectDirection dir, World::SelectType type)
{
    unsigned steps, imaginedX, imaginedY, imaginedW, imaginedH, calcX, calcY;
    int signOfDir;
    Entity *e;
    bool upOrDown = (dir == UP || dir == DOWN);
    if(upOrDown)
    {
        imaginedX = x;
        imaginedY = y;
        imaginedW = World::WIDTH;
        imaginedH = World::HEIGHT;
    }
    else
    {
        imaginedX = y;
        imaginedY = x;
        imaginedW = World::HEIGHT;
        imaginedH = World::WIDTH;
    }
    steps = imaginedY;
    signOfDir = -1;
    if(dir == DOWN || dir == RIGHT)
    {
        steps = imaginedH - steps - 1;
        signOfDir = 1;
    }
    for(unsigned i = 1; i <= steps; ++i)
    {
        calcY = (unsigned) (imaginedY + signOfDir * i);
        for(int j = 0; j <= (int) i; ++j)
        {
            for(int signPerpendicular = -1; signPerpendicular < 2;
                signPerpendicular += 2)
            {
                calcX = imaginedX + signPerpendicular * j;
                if(calcX < 0 || calcX >= imaginedW)
                    continue;
                if(upOrDown)
                    e = World::getInstance().getEntity(calcX, calcY);
                else
                    e = World::getInstance().getEntity(calcY, calcX);
                if(e)
                {
                    switch(type)
                    {
                    case World::ENEMY_UNIT:
                        if(e->getId() == 3 - id &&
                           dynamic_cast<UnitGroup*>(e))
                        {
                            selectedEntity = e;
                            return;
                        }
                        break;
                    case World::ENEMY_BUILDING:
                        if(e->getId() == 3 - id && dynamic_cast<Building*>(e) &&
                           dynamic_cast<Building*>(e)->isBuilt())
                        {
                            selectedEntity = e;
                            return;
                        }
                        break;
                    case World::OWN_UNIT:
                        if(e->getId() == id &&
                           dynamic_cast<UnitGroup*>(e))
                        {
                            selectedEntity = e;
                            return;
                        }
                        break;
                    case World::OWN_BUILDING:
                        if(e->getId() == id && dynamic_cast<Building*>(e) &&
                           dynamic_cast<Building*>(e)->isBuilt())
                        {
                            selectedEntity = e;
                            return;
                        }
                        break;
                    case World::SPAWN_POINT:
                        if(e->getId() == id && (
                           e->getType() == Entity::BARRACKS ||
                           e->getType() == Entity::CITY) &&
                           dynamic_cast<Building*>(e)->isBuilt())
                        {
                            selectedEntity = e;
                            return;
                        }
                        break;
                    case World::HOSPITAL:
                        if(e->getId() == id && (
                           e->getType() == Entity::HOSPITAL &&
                           dynamic_cast<Building*>(e)->isBuilt()))
                        {
                            selectedEntity = e;
                            return;
                        }
                        break;
                    }
                }
            }
        }
    }
}

void GUI::updatePlayerUnitList()
{
    UnitGroup *unitGroup;
    for(unsigned i = 0; i < Player::MAX_UNITS; ++i)
    {
        unitGroup = player->getUnitSlot(i);
        if(unitGroup)
            playerUnits->setSlot(i, unitGroup->getType(), unitGroup->getCount());
        else
            playerUnits->clearSlot(i);
    }
}

void GUI::updateSelection()
{
    player->selectSlot(playerUnits->getSelection());
    ImageElement *unitImg = static_cast<ImageElement*>(staticElements[14]);
    if(player->getActiveSlot())
    {
        unitImg->setTextureRect(sf::IntRect(20 * (unsigned)
                player->getActiveSlot()->getType(), 0, 20, 20));
        if(state == UNIT_ACTIONS || state == SELECTING_TARGET_LOCATION ||
           state == SELECTING_TARGET_ENEMY || state == SLOT_ACTIONS ||
           state == SELECTING_DEFEND_LOCATION)
        {
            for(Button *b : buttons)
                b->enable();
            if(state == SLOT_ACTIONS)
            {
                buttons[1]->disable();
                if(player->getUnitCount() >= Player::MAX_UNITS ||
                   player->getActiveSlot()->getCount() < 20)
                    buttons[3]->disable();
            }
            else
                updateMenu();
        }
        for(unsigned i = 13; i <= 23; ++i)
            static_cast<Label*>(staticElements[i])->setVisibility(true);
    }
    else
    {
        unitImg->setTextureRect(sf::IntRect(0, 20, 20, 20));
        if(state == UNIT_ACTIONS || state == SLOT_ACTIONS)
        {
            for(Button *b : buttons)
                b->disable();
            if(state == SLOT_ACTIONS)
                buttons[1]->enable();
        }
        for(unsigned i = 13; i <= 23; ++i)
            static_cast<Label*>(staticElements[i])->setVisibility(false);
    }
    if(state == SPAWNING_UNITS || state == SELECTING_SPAWNPOINT)
        checkSlotForSpawning();
    else if(state == SPLITTING_GROUP)
        checkSlotForSplitting();
    else if(state == MERGING_GROUPS)
        checkSlotForMerging();
    else if(state == SLOT_ACTIONS)
        hideMoveToBarracksErrors();
    else if(state == UNIT_ACTIONS)
    {
        hideHealErrors();
        (*(elements[state]))[8]->setVisibility(false);
    }
    updateButtons();
}

void GUI::processKeyPress(sf::Event::KeyEvent e)
{
    sf::Keyboard::Key code = e.code;
    if((code == sf::Keyboard::S    && id == 1) ||
       (code == sf::Keyboard::Down && id == 2))
    {
        if(state == SELECTING_TARGET_LOCATION ||
           state == SELECTING_BUILDING || state == SELECTING_DEFEND_LOCATION)
        {
            if(selectedY < World::HEIGHT - 1) {
                ++selectedY;
                if(state == SELECTING_BUILDING)
                {
                    updateBuildDataLabels();
                    updateCostColors(5);
                }
            }
        }
        else if(state == SELECTING_TARGET_ENEMY)
        {
            if(selectedEntity)
                selectEntity(selectedEntity->getX(), selectedEntity->getY(),
                             DOWN, World::ENEMY_UNIT);
        }
        else if(state == SELECTING_SPAWNPOINT)
        {
            selectEntity(selectedEntity->getX(), selectedEntity->getY(),
                         DOWN, World::SPAWN_POINT);
            checkSlotForSpawning();
        }
        else
            menuNext();
    }
    else if((code == sf::Keyboard::W  && id == 1) ||
            (code == sf::Keyboard::Up && id == 2))
    {
        if(state == SELECTING_TARGET_LOCATION ||
           state == SELECTING_BUILDING || state == SELECTING_DEFEND_LOCATION)
        {
            if(selectedY > 0) {
                --selectedY;
                if(state == SELECTING_BUILDING)
                {
                    updateBuildDataLabels();
                    updateCostColors(5);
                }
            }
        }
        else if(state == SELECTING_TARGET_ENEMY)
        {
            if(selectedEntity)
                selectEntity(selectedEntity->getX(), selectedEntity->getY(),
                             UP, World::ENEMY_UNIT);
        }
        else if(state == SELECTING_SPAWNPOINT)
        {
            selectEntity(selectedEntity->getX(), selectedEntity->getY(),
                         UP, World::SPAWN_POINT);
            checkSlotForSpawning();
        }
        else
            menuPrev();
    }
    else if((code == sf::Keyboard::A    && id == 1) ||
            (code == sf::Keyboard::Left && id == 2))
    {
        if(state == SELECTING_TARGET_LOCATION ||
           state == SELECTING_BUILDING || state == SELECTING_DEFEND_LOCATION)
        {
            if(selectedX > 0) {
                --selectedX;
                if(state == SELECTING_BUILDING)
                {
                    updateBuildDataLabels();
                    updateCostColors(5);
                }
            }
        }
        else if(state == PAYING_FOR_UNITS)
            buttons[0]->activate(this);
        else if(state == SPAWNING_UNITS && activeButton == 2)
        {
            if(id == 1)
                decreaseSpawningCount(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));
            else
                decreaseSpawningCount(sf::Keyboard::isKeyPressed(sf::Keyboard::RShift));
        }
        else if(state == SPLITTING_GROUP && activeButton == 0)
        {
            if(id == 1)
                decreaseSplittingCount(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));
            else
                decreaseSplittingCount(sf::Keyboard::isKeyPressed(sf::Keyboard::RShift));
        }
        else if(state == SELECTING_TARGET_ENEMY)
        {
            if(selectedEntity)
                selectEntity(selectedEntity->getX(), selectedEntity->getY(),
                             LEFT, World::ENEMY_UNIT);
        }
        else if(state == SELECTING_SPAWNPOINT)
        {
            selectEntity(selectedEntity->getX(), selectedEntity->getY(),
                         LEFT, World::SPAWN_POINT);
            checkSlotForSpawning();
        }
    }
    else if((code == sf::Keyboard::D     && id == 1) ||
            (code == sf::Keyboard::Right && id == 2))
    {
        if(state == SELECTING_TARGET_LOCATION ||
           state == SELECTING_BUILDING || state == SELECTING_DEFEND_LOCATION)
        {
            if(selectedX < World::WIDTH - 1) {
                ++selectedX;
                if(state == SELECTING_BUILDING)
                {
                    updateBuildDataLabels();
                    updateCostColors(5);
                }
            }
        }
        else if(state == PAYING_FOR_UNITS)
            buttons[1]->activate(this);
        else if(state == SPAWNING_UNITS && activeButton == 2)
        {
            if(id == 1)
                increaseSpawningCount(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));
            else
                increaseSpawningCount(sf::Keyboard::isKeyPressed(sf::Keyboard::RShift));
        }
        else if(state == SPLITTING_GROUP && activeButton == 0)
        {
            if(id == 1)
                increaseSplittingCount(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift));
            else
                increaseSplittingCount(sf::Keyboard::isKeyPressed(sf::Keyboard::RShift));
        }
        else if(state == SELECTING_TARGET_ENEMY)
        {
            if(selectedEntity)
                selectEntity(selectedEntity->getX(), selectedEntity->getY(),
                             RIGHT, World::ENEMY_UNIT);
        }
        else if(state == SELECTING_SPAWNPOINT)
        {
            selectEntity(selectedEntity->getX(), selectedEntity->getY(),
                         RIGHT, World::SPAWN_POINT);
            checkSlotForSpawning();
        }
    }
    else if((code == sf::Keyboard::E      && id == 1) ||
            (code == sf::Keyboard::Return && id == 2))
    {
        if(state == SELECTING_TARGET_LOCATION)
        {
            if(player->getActiveEntity())
                World::getInstance().registerPath(player->getActiveEntity(),
                        selectedX, selectedY, Controllable::MOVE_TO_TARGET_AREA);
        }
        else if(state == SELECTING_TARGET_ENEMY)
        {
            if(player->getActiveEntity() && selectedEntity)
                World::getInstance().registerPath(player->getActiveEntity(),
                        selectedEntity, Controllable::MOVE_TO_TARGET_ENTITY);
        }
        else if(state == SELECTING_BUILDING)
        {
            if(buildingOk)
            {
                World::getInstance().addBuilding(static_cast<Building*>(
                        World::getInstance().getPrototype(buildingType)->clone()),
                        selectedX, selectedY, id);
                player->spend(costs[0], costs[1], costs[2], costs[3]);
            }
        }
        else if(state == SELECTING_SPAWNPOINT)
        {
            checkSlotForSpawning();
            if(!slotOk)
                return;
            std::vector<Element*> &menu = *(elements[state]);
            UnitList *unitList = static_cast<UnitList*>(menu[0]);
            unsigned unitType = unitList->getSelection();
            unsigned unitCount = static_cast<Slider*>(menu[5])->getValue();
            player->takeUnitsFromBarracks(unitType, unitCount);
            updateSpawningInfo();
            sf::Vector2u pos = World::getInstance().findClosestEmptyTile(
                    selectedEntity->getX(), selectedEntity->getY());
            World::getInstance().addUnit(new UnitGroup(id,
                    World::unsignedToType(unitType), unitCount), pos.x, pos.y);
            state = SPAWNING_UNITS;
            (*(elements[state]))[8]->setVisibility(false);
        }
        else if(state == SELECTING_DEFEND_LOCATION)
        {
            if(player->getActiveEntity())
                World::getInstance().registerPath(player->getActiveEntity(),
                        selectedX, selectedY, Controllable::DEFEND);
        }
        else if(enabledButtons != 0)
            buttons[activeButton]->activate(this);
    }
    else if((code == sf::Keyboard::Tab       && id == 1) ||
            (code == sf::Keyboard::BackSpace && id == 2))
    {
        if((id == 1 && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) ||
           (id == 2 && sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)))
        {
            playerUnits->nextNonempty();
            updateSelection();
        }
        else
        {
            if(state == SELECTING_TARGET_ENEMY)
            {
                Player *p = World::getInstance().getPlayer(3 - id);
                if(p->getUnitCount() == 0)
                    selectedEntity = nullptr;
                else
                {
                    ++selectedIndex;
                    if(selectedIndex >= p->getUnitCount())
                        selectedIndex = 0;
                    selectedEntity = p->getUnit(selectedIndex);
                }
            }
            else if(state != SELECTING_TARGET_LOCATION &&
                    state != SELECTING_SPAWNPOINT &&
                    state != SELECTING_DEFEND_LOCATION)
            {
                if((id == 1 && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) ||
                   (id == 2 && sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)))
                    menuPrev();
                else
                    menuNext();
            }
        }
    }
    else if((code == sf::Keyboard::Q      && id == 1) ||
            (code == sf::Keyboard::Delete && id == 2))
    {
        if(state == UNIT_ACTIONS || state == HERO_SELECTED ||
           state == SELECTING_BUILDING || state == TRAINING_UNITS ||
           state == SLOT_ACTIONS)
            makeMenu(MAIN_MENU);
        else if(state == SELECTING_TARGET_LOCATION ||
                state == SELECTING_TARGET_ENEMY ||
                state == SELECTING_DEFEND_LOCATION)
        {
            state = UNIT_ACTIONS;
            (*(elements[state]))[8]->setVisibility(false);
        }
        else if(state == PAYING_FOR_UNITS)
            makeMenu(TRAINING_UNITS);
        else if(state == SPAWNING_UNITS)
            makeMenu(SLOT_ACTIONS);
        else if(state == SELECTING_SPAWNPOINT)
        {
            state = SPAWNING_UNITS;
            std::vector<Element*> &menu = *(elements[state]);
            menu[8]->setVisibility(false);
            menu[9]->setVisibility(false);
        }
        else if(state == CHANGING_SLOT)
        {
            std::vector<Element*> &menu = *(elements[state]);
            menu[4]->setVisibility(true);
            menu[5]->setVisibility(false);
            menu[6]->setVisibility(false);
            playerUnits->selectSlot(selectedSlot);
            player->selectSlot(selectedSlot);
            makeMenu(SLOT_ACTIONS);
        }
        else if(state == SPLITTING_GROUP)
        {
            playerUnits->selectSlot(selectedSlot);
            player->selectSlot(selectedSlot);
            makeMenu(SLOT_ACTIONS);
        }
        else if(state == MERGING_GROUPS)
        {
            std::vector<Element*> &menu = *(elements[state]);
            menu[2]->setVisibility(true);
            menu[7]->setVisibility(false);
            menu[8]->setVisibility(false);
            menu[9]->setVisibility(false);
            menu[10]->setVisibility(false);
            playerUnits->selectSlot(selectedSlot);
            player->selectSlot(selectedSlot);
            makeMenu(SLOT_ACTIONS);
        }
    }
    else if(id == 1 && code >= sf::Keyboard::Num1 && code <= sf::Keyboard::Num8)
    {
        playerUnits->selectSlot(code - sf::Keyboard::Num1);
        updateSelection();
    }
    else if(id == 2 && code >= sf::Keyboard::Numpad1 && code <= sf::Keyboard::Numpad8)
    {
        playerUnits->selectSlot(code - sf::Keyboard::Numpad1);
        updateSelection();
    }
}
