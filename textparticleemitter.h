#ifndef TEXTPARTICLEEMITTER_H
#define TEXTPARTICLEEMITTER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "gui.h"

class Entity;

class TextParticle : public sf::Drawable
{
public:
    TextParticle(std::string t, unsigned X, unsigned Y, float l);
    void update(float delta);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool isAlive() const;
private:
    std::string text;
    unsigned x, y;
    bool alive;
    float life;
    float maxLife;
};

class TextParticleEmitter : public sf::Drawable
{
public:
    TextParticleEmitter();
    virtual ~TextParticleEmitter();

    void update(float delta);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void emitDamage(std::string d, unsigned x, unsigned y);
private:
    std::vector<TextParticle*> particles;
};

#endif // TEXTPARTICLEEMITTER_H
