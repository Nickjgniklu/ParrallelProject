#include "Particle.hpp"
#include <iostream>

float gravitationalConstant = 100000.0f;

int main()
{
    auto one = Particle(550, 550);
    auto two = Particle(450, 450);

    for (int i = 0; i < 100; i++)
    {
        one.calcGravityToOther(two);
        two.calcGravityToOther(one);
        one.update(1.0f);
        two.update(1.0f);
        std::cout << "Particle one has posX: " << one.posX << " posY: " << one.posY << std::endl;
        std::cout << "Particle two has posX: " << two.posX << " posY: " << two.posY << std::endl;

        std::cout << "Particle one has accelerationX:" << one.accelerationX << " aaccelerationY:" << one.accelerationY << std::endl;
        std::cout << "Particle two has velocityX:" << two.velocityX << " velocityY:" << two.velocityY << std::endl;
    }
    return 0;
}