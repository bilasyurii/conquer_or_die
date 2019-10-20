#include <SFML/Graphics.hpp>
#include "world.h"
#include "textparticleemitter.h"
#include "gui.h"
#include "resourcemanager.h"

int main()
{
    setlocale(LC_ALL, "Ukrainian");
    sf::RenderWindow window(sf::VideoMode(World::SCREEN_WIDTH +
                                          World::MENU_WIDTH * 2,
                                          World::SCREEN_HEIGHT),
                            "Conquer or die!");
    ResourceManager::getInstance().loadFont("res/pixelated.ttf");
    ResourceManager::getInstance().loadTexture("res/spritelist.png");
    GUI *guiLeft = new GUI(1);
    GUI *guiRight = new GUI(2);
    World::getInstance().setGUIs(guiLeft, guiRight);
    World::getInstance().addStartingUnits();
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::KeyPressed)
            {
                guiLeft->processKeyPress(event.key);
                guiRight->processKeyPress(event.key);
            }
        }
        World::getInstance().update();
        guiLeft->update(World::getInstance().getDelta());
        guiRight->update(World::getInstance().getDelta());        window.clear(sf::Color(50, 30, 10));
        window.draw(World::getInstance());
        window.draw(*guiLeft);
        window.draw(*guiRight);
        window.display();
    }
    delete guiLeft;
    delete guiRight;
    return 0;
}
