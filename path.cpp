#include "path.h"
#include "world.h"
#include <map>
#include <set>
#include <math.h>

Path::Path() : found(false) {}

Path::~Path() {}

bool Path::Node::operator<(const Path::Node& n) const
{
    return (pos.y < n.pos.y) ? true : (pos.y == n.pos.y ? pos.x < n.pos.x : false);
}

bool Path::Node::getNeighbour(Path::Node& res, unsigned direction) const
{
    switch(direction)
    {
    case 0:
        if(pos.y == 0)
            return false;
        res.pos.y = pos.y - 1;
        res.pos.x = pos.x;
        return true;
    case 1:
        if(pos.x == 0)
            return false;
        res.pos.x = pos.x - 1;
        res.pos.y = pos.y;
        return true;
    case 2:
        res.pos.x = pos.x + 1;
        if(res.pos.x >= World::WIDTH)
            return false;
        res.pos.y = pos.y;
        return true;
    case 3:
        res.pos.y = pos.y + 1;
        if(res.pos.y >= World::HEIGHT)
            return false;
        res.pos.x = pos.x;
        return true;
    }
    return false;
}

void Path::reset()
{
    found = false;
}

bool Path::recalculate(bool targetingEntity)
{
    if(!targetingEntity && !World::getInstance().isWalkable(end.x, end.y))
    {
        found = false;
        return false;
    }
    std::set<Node> open, closed;
    std::map<Node, Node> parent;
    float newScore;
    open.insert(Node{start, 0.0f, hCost(start, end)});
    Node current, neighbour;
    std::set<Node>::iterator temp;
    while(!open.empty())
    {
        temp = open.begin();
        for(std::set<Node>::iterator it = open.begin(); it != open.end(); ++it)
            if(it->fScore < temp->fScore)
                temp = it;
        current = *temp;
        open.erase(temp);
        closed.insert(current);
        for(unsigned dir = 0; dir < 4; ++dir)
        {
            if(!current.getNeighbour(neighbour, dir) ||
               closed.find(neighbour) != closed.end() ||
               (!World::getInstance().isWalkable(neighbour.pos.x, neighbour.pos.y) &&
                !(targetingEntity &&
                  neighbour.pos.x == end.x && neighbour.pos.y == end.y)))
                continue;
            if(neighbour.pos == end)
            {
                parent[neighbour] = current;
                nodes = std::deque<sf::Vector2u>();
                while(true)
                {
                    if(neighbour.pos == start)
                        break;
                    nodes.push_front(neighbour.pos);
                    neighbour = parent[neighbour];
                }
                found = true;
                return true;
            }
            newScore = 1.0f + current.gScore;
            temp = open.find(neighbour);
            if(temp != open.end() && newScore >= temp->gScore)
                continue;
            parent[neighbour] = current;
            neighbour.gScore = newScore;
            neighbour.fScore = newScore + hCost(neighbour.pos, end);
            open.insert(neighbour);
        }
    }
    found = false;
    return false;
}

float Path::hCost(sf::Vector2u from, sf::Vector2u to) const
{
    return std::fabs(((int)to.x) - (int)from.x) +
           std::fabs(((int)to.y) - (int)from.y);
}

bool Path::wasFound() const
{
    return found;
}

void Path::setStart(unsigned sX, unsigned sY)
{
    start.x = sX;
    start.y = sY;
}

void Path::setEnd(unsigned eX, unsigned eY)
{
    end.x = eX;
    end.y = eY;
}

sf::Vector2u Path::getEnd() const
{
    return end;
}

sf::Vector2u Path::next()
{
    sf::Vector2u v = nodes.front();
    nodes.pop_front();
    return v;
}

bool Path::isEmpty() const
{
    return found && nodes.empty();
}
