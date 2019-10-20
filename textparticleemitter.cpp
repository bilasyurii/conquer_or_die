#include "textparticleemitter.h"
#include "entity.h"

TextParticle::TextParticle(std::string t, unsigned X, unsigned Y, float l) :
    text(t), x(X), y(Y), alive(true), life(l), maxLife(l) {}

void TextParticle::update(float delta)
{
    if(!alive)
        return;
    life -= delta;
    if(life <= 0.0f)
        alive = false;
}

void TextParticle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    GUI::text.setString(text);
    GUI::text.setPosition(x, y);
    GUI::text.setFillColor(sf::Color(255, 0, 0, life / maxLife * 255));
    target.draw(GUI::text, states);
}

bool TextParticle::isAlive() const
{
    return alive;
}

TextParticleEmitter::TextParticleEmitter() {}

TextParticleEmitter::~TextParticleEmitter()
{
    for(TextParticle *p : particles)
        delete p;
}

void TextParticleEmitter::update(float delta)
{
    for(std::vector<TextParticle*>::iterator it = particles.begin();
        it != particles.end();)
    {
        (*it)->update(delta);
        if((*it)->isAlive())
            ++it;
        else
        {
            delete *it;
            it = particles.erase(it);
        }
    }
}

void TextParticleEmitter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(TextParticle *p : particles)
        p->draw(target, states);
}

void TextParticleEmitter::emitDamage(std::string d, unsigned x, unsigned y)
{
    particles.push_back(new TextParticle(d, x, y, 0.5f));
}
