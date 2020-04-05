#include "Particle.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include "MJPEGWriter.h"

int WINDOWSIZE = 1000;
int SCALE = 5;
int WORLDCENTER = WINDOWSIZE * SCALE / 2;
float GRAVITATIONALCONSTANT = 100000.0f;
float FRICTIONCONSTANT = .9995f;
double MINFRAMETIME = .08333333f;
int NUMBEROFPARTICLES = 100;
int TOTALFRAMECOUNT = 1000;

Mat GetWorldFrame(std::vector<Particle> particles)
{
    Mat world = Mat::zeros(WINDOWSIZE, WINDOWSIZE, CV_8U);
    for (auto currentParticle : particles)
    {
        if (currentParticle.posX >= 0 && currentParticle.posX < WINDOWSIZE * SCALE && currentParticle.posY >= 0 && currentParticle.posY < WINDOWSIZE * SCALE)
        {
            world.at<uchar>((int)currentParticle.posY / SCALE, (int)currentParticle.posX / SCALE) = 254;
        }
    }
    return world;
}

int main(int argc, char **argv)
{

    if (argc > 1)
    {
        NUMBEROFPARTICLES = std::atof(argv[1]);
    }
    if (argc > 2)
    {
        GRAVITATIONALCONSTANT = std::atof(argv[2]);
    }
    if (argc > 3)
    {
        TOTALFRAMECOUNT = std::atof(argv[3]);
    }

    //Create a random device that can generate a normal distribution around 500
    std::random_device rndm;
    std::default_random_engine engine(rndm());
    std::normal_distribution<float> positionDistribution(WORLDCENTER, NUMBEROFPARTICLES / 2);
    std::normal_distribution<float> velocityDistribution(300, 50);

    //Create NumberOfParticle particles and give them random locations to start
    std::vector<Particle> particles(NUMBEROFPARTICLES);
    for (int i = 0; i < NUMBEROFPARTICLES; i++)
    {
        float xPos = positionDistribution(engine);
        float yPos = positionDistribution(engine);
        // do
        // {
        //     xPos = positionDistribution(engine);
        // } while (xPos > WINDOWSIZE * SCALE || xPos < 0);
        // do
        // {
        //     yPos = positionDistribution(engine);
        // } while (yPos > WINDOWSIZE * SCALE || yPos < 0);

        float deltaX = WORLDCENTER - xPos;
        float deltaY = WORLDCENTER - yPos;
        float angle = std::atan2(deltaY, deltaX);
        float distance = std::sqrt(std::pow((xPos - WORLDCENTER), 2) + std::pow((yPos - WORLDCENTER), 2));
        angle += M_PI / 2.3f;
        // float speed = 200 * std::pow(1 - .0001f, distance);
        // float speed = 200 + distance / 6;
        // float speed = NUMBEROFPARTICLES / 9;
        float speed = std::sqrt(GRAVITATIONALCONSTANT * (distance * distance / NUMBEROFPARTICLES) / distance);
        float velocityX = std::cos(angle) * speed;
        float velocityY = std::sin(angle) * speed;
        // float velocityX = 0;
        // float velocityY = 0;

        particles[i] = Particle(xPos, yPos, velocityX, velocityY);
    }
    //create server
    MJPEGWriter server(7777);
    Mat blank = Mat::zeros(WINDOWSIZE, WINDOWSIZE, CV_8U);
    server.write(blank);
    blank.release();
    server.start();
    std::cout << "Open localhost:7777 and then press enter in the terminal to start";
    std::cin.ignore();
    auto frameStart = std::chrono::high_resolution_clock::now();
    auto frameEnd = frameStart;
    std::chrono::duration<double> deltaTime = frameStart - frameEnd;
    std::chrono::duration<double> totalTime = frameStart - frameEnd;
    // for(int i = 0; i < totalFrameCount; i++){
    while (true)
    {

        for (auto &currentParticle : particles)
        {
            for (auto &otherParticle : particles)
            {
                currentParticle.calcGravityToOther(otherParticle);
            }
        }
        frameEnd = std::chrono::high_resolution_clock::now();
        deltaTime = frameEnd - frameStart;
        totalTime += deltaTime;
        double count = deltaTime.count();
        frameStart = std::chrono::high_resolution_clock::now();
        for (auto &currentParticle : particles)
        {
            currentParticle.update(std::min(MINFRAMETIME, count));
        }
        Mat worldFrame = GetWorldFrame(particles);
        server.write(worldFrame);
        worldFrame.release();
    }
    server.stop();

    return 0;
}