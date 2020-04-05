#include <algorithm>
#include <cmath>
#include <iostream>

extern float GRAVITATIONALCONSTANT;
extern float FRICTIONCONSTANT;

struct Particle
{
    float accelerationX = 0;
    float accelerationY = 0;
    float posX = 0;
    float posY = 0;
    float velocityX = 0;
    float velocityY = 0;

    Particle(float xPos, float yPos, float xVel, float yVel) : posY(yPos), posX(xPos), velocityY(yVel), velocityX(xVel) {}
    Particle(float xPos, float yPos) : posY(yPos), posX(xPos) {}
    Particle() = default;

    void calcGravityToOther(const Particle &other)
    {
        float distance = calcDistanceToOther(other);
        if (distance > 7)
        {
            float deltaX = other.posX - posX;
            float deltaY = other.posY - posY;
            float gravity = GRAVITATIONALCONSTANT / std::pow(distance, 2);
            float angle = std::atan2(deltaY, deltaX);
            accelerationX += std::cos(angle) * gravity;
            accelerationY += std::sin(angle) * gravity;
        }
    }

    float calcDistanceToOther(const Particle &other)
    {
        return std::sqrt(std::pow((posX - other.posX), 2) + std::pow((posY - other.posY), 2));
    }

    void update(float deltaTime)
    {
        posX += velocityX * deltaTime + accelerationX * .5f * std::pow(deltaTime, 2);
        posY += velocityY * deltaTime + accelerationY * .5f * std::pow(deltaTime, 2);
        velocityX = (velocityX + accelerationX * deltaTime) * FRICTIONCONSTANT;
        velocityY = (velocityY + accelerationY * deltaTime) * FRICTIONCONSTANT;
        accelerationY = 0.0f;
        accelerationX = 0.0f;
    }
};
