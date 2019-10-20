#include "slider.h"

Slider::Slider(unsigned _x, unsigned _y, unsigned _width, unsigned _min,
               unsigned _max, unsigned _step) : lineWidth(_width), step(_step)
{
    lineX = _x;
    lineY = _y;
    width = 6;
    height = 20;
    y = lineY - 10;
    action = nullptr;
    line.setFillColor(sf::Color(180, 130, 60));
    line.setSize(sf::Vector2f(lineWidth, 6.0f));
    line.setOrigin(0.0f, 3.0f);
    line.setPosition(lineX, lineY);
    indicator.setSize(sf::Vector2f(6.0f, 20.0f));
    setMinMax(_min, _max);
}

Slider::~Slider() {}

void Slider::activate(GUI* gui) const {}

void Slider::setAction(void(GUI::* a)(void)) {}

void Slider::setString(std::string s) {}

std::string Slider::getString() const
{
    return "";
}

void Slider::setMinMax(unsigned _min, unsigned _max)
{
    min = _min;
    max = _max;
    if(min >= max)
    {
        enabled = false;
        min = max;
        value = min;
        x = lineX - 3;
        indicator.setPosition(x, y);
        indicator.setFillColor(sf::Color(197, 148, 78));
    }
    else
    {
        enabled = true;
        setValue(min);
        indicator.setFillColor(sf::Color(214, 167, 96));
    }
}

void Slider::setX(unsigned _x)
{
    lineX = _x;
    x = lineX + (value - min) / (float) (max - min) * lineWidth - 3;
    line.setPosition(lineX, lineY);
    indicator.setPosition(x, y);
}

void Slider::setY(unsigned _y)
{
    lineY = _y;
    y = lineY - 10;
    line.setPosition(lineX, lineY);
    indicator.setPosition(x, y);
}

void Slider::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(line, states);
    target.draw(indicator, states);
}

void Slider::setValue(unsigned v)
{
    value = v;
    x = lineX + (value - min) / (float) (max - min) * lineWidth - 3;
    indicator.setPosition(x, y);
}

unsigned Slider::getValue() const
{
    return value;
}

void Slider::reset()
{
    setValue(min);
}

void Slider::increase(bool faster)
{
    if(value == (int) max)
        return;
    int newValue = value + step;
    if(faster)
        newValue += step * 9;
    if(newValue >= (int) max)
        setValue(max);
    else
        setValue(newValue);
}

void Slider::decrease(bool faster)
{
    if(value == (int) min)
        return;
    int newValue;
    if(value % step)
        newValue = value - value % step;
    else
        newValue = value - step;
    if(faster)
        newValue -= step * 9;
    if(newValue <= (int) min)
        setValue(min);
    else
        setValue(newValue);
}
