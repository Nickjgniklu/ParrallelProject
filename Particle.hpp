#include <cmath>

extern float gravitationalConstant;

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
        float deltaX = std::abs(posX - other.posX);
        float deltaY = std::abs(posY - other.posY);
        float distance = std::sqrt(std::pow((deltaX), 2) + std::pow((deltaY), 2));
        float gravity = gravitationalConstant / std::pow(distance, 2);
        float deltaTotal = deltaX + deltaY;
        float ratio = gravity / deltaTotal;
        accelerationX += ratio * deltaX;
        accelerationY += ratio * deltaY;
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
};
