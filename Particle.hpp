#include <algorithm>
#include <cmath>
#include <iostream>

extern float gravitationalConstant;
extern float accelerationCap;

struct Particle
{
    float accelerationX = 0;
    float accelerationY = 0;
    float posX = 0;
    float posY = 0;
    float velocityX = 0;
    float velocityY = 0;
    int id;

    Particle(float xPos, float yPos, float xVel, float yVel, int id) : posY(yPos), posX(xPos), velocityY(yVel), velocityX(xVel), id(id) {}
    Particle(float xPos, float yPos, int id) : posY(yPos), posX(xPos), id(id) {}
    Particle() = default;

    void calcGravityToOther(const Particle &other)
    {
        float distance = calcDistanceToOther(other);
        if (distance > 5)
        {
            float deltaX = other.posX - posX;
            float deltaY = other.posY - posY;
            float gravity = gravitationalConstant / std::pow(distance, 2);
            float angle = std::atan2(deltaY, deltaX);
            accelerationX += std::cos(angle) * gravity;
            accelerationY += std::sin(angle) * gravity;
        }
        // float deltaTotal = std::abs(deltaX) + std::abs(deltaY);
        // if (deltaTotal != 0.0)
        // {
        // float ratio = gravity / deltaTotal;
        // accelerationX = cap(accelerationX + ratio * deltaX);
        // accelerationY = cap(accelerationY + ratio * deltaY);
        // }
    }

    float calcDistanceToOther(const Particle &other)
    {
        return std::sqrt(std::pow((posX - other.posX), 2) + std::pow((posY - other.posY), 2));
    }

    void update(float deltaTime)
    {
        posX += velocityX * deltaTime + accelerationX * .5f * std::pow(deltaTime, 2);
        posY += velocityY * deltaTime + accelerationY * .5f * std::pow(deltaTime, 2);
        velocityX += accelerationX * deltaTime;
        velocityY += accelerationY * deltaTime;
        accelerationY = 0.0f;
        accelerationX = 0.0f;
    }

    float cap(float value)
    {
        if (value < -accelerationCap)
        {
            return -accelerationCap;
        }
        else if (value > accelerationCap)
        {
            return accelerationCap;
        }
    }
};
