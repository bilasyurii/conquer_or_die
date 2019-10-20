#include "resourcemanager.h"

bool ResourceManager::initialized = false;

ResourceManager *ResourceManager::instance;

ResourceManager &ResourceManager::getInstance()
{
    if(!initialized)
    {
        instance = new ResourceManager;
        initialized = true;
    }
    return *instance;
}

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {}

void ResourceManager::loadFont(std::string path)
{
    font.loadFromFile(path);
}

void ResourceManager::loadTexture(std::string path)
{
    texture.loadFromFile(path);
}

sf::Font &ResourceManager::getFont()
{
    return font;
}

sf::Texture &ResourceManager::getTexture()
{
    return texture;
}
