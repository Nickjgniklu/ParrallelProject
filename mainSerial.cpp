#include "Particle.hpp"
#include <chrono>
#include <iostream>
#include <vector>
#include <random>

float gravitationalConstant = 1.223f;
int numberOfParticles = 1000;
int iterations = 10;

int main()
{

    //Create a random device that can generate a normal distribution around 500
    std::random_device rndm;
    std::default_random_engine engine(rndm());
    std::normal_distribution<float> distribution(500, 250);

    //Create NumberOfParticle particles and give them random locations to start
    std::vector<Particle> particles(numberOfParticles);
    for (int i = 0; i < numberOfParticles; i++)
    {
        particles[i] = Particle(distribution(engine), distribution(engine));
    }

    auto totalStart = std::chrono::high_resolution_clock::now();
    auto frameStart = std::chrono::high_resolution_clock::now();
    auto frameEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> deltaTime = frameStart - frameEnd;

    for (int i = 0; i < 1000; i++)
    {
        for (auto currentParticle : particles)
        {
            for (auto otherParticle : particles)
            {
                currentParticle.calcGravityToOther(otherParticle);
            }
        }
        frameEnd = std::chrono::high_resolution_clock::now();
        deltaTime = frameEnd - frameStart;
        double count = deltaTime.count();
        frameStart = std::chrono::high_resolution_clock::now();
        for (auto currentParticle : particles)
        {
            currentParticle.update(count);
        }
    }
    auto totalEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalDiff = totalEnd - totalStart;
    std::cout << "It took " << totalDiff.count() << " seconds to do " << iterations << " iterations of a universe with " << numberOfParticles << " particles in it." << std::endl;
    return 0;
}