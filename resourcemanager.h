#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SFML/Graphics.hpp>
#include <string>

class ResourceManager
{
public:
    static ResourceManager &getInstance();
    void loadFont(std::string path);
    void loadTexture(std::string path);
    sf::Font &getFont();
    sf::Texture &getTexture();
    virtual ~ResourceManager();
private:
    static bool initialized;
    static ResourceManager *instance;
    sf::Font font;
    sf::Texture texture;

    ResourceManager();
};

#endif // RESOURCEMANAGER_H
