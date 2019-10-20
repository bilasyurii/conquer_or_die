#ifndef PATH_H
#define PATH_H

#include <deque>
#include <SFML/Graphics.hpp>

class World;

class Path
{
    public:
        Path();
        virtual ~Path();

        bool wasFound() const;
        sf::Vector2u getEnd() const;
        void setStart(unsigned sX, unsigned sY);
        void setEnd(unsigned eX, unsigned eY);
        bool recalculate(bool targetingEntity = false);
        void reset();
        sf::Vector2u next();
        bool isEmpty() const;
    private:
        bool found;
        sf::Vector2u start, end;
        std::deque<sf::Vector2u> nodes;
        struct Node
        {
            sf::Vector2u pos;
            mutable float gScore, fScore;
            bool operator<(const Node &n) const;
            bool getNeighbour(Node &res, unsigned direction) const;
        };
        float hCost(sf::Vector2u from, sf::Vector2u to) const;
        friend class World;
        friend class GUI;
};

#endif // PATH_H
