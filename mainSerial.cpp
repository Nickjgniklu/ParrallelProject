#include "Particle.hpp"
#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include "MJPEGWriter.h"

#define WORLDSIZE 1000
#define SCALE 2

float gravitationalConstant = 100000.0f;
// float accelerationCap = 1000.0f;
int numberOfParticles = 100;
float totalRunTime = 10.0f;

Mat GetWorldFrame(std::vector<Particle> particles)
{
    Mat world = Mat::zeros(WORLDSIZE, WORLDSIZE, CV_8U);
    for (auto currentParticle : particles)
    {
        if (currentParticle.posX >= 0 && currentParticle.posX < WORLDSIZE * SCALE && currentParticle.posY >= 0 && currentParticle.posY < WORLDSIZE * SCALE)
        {
            world.at<uchar>((int)currentParticle.posY / SCALE, (int)currentParticle.posX / SCALE) = 254;
            //cout<<"x:"<<(int)currentParticle.posX<<" y:"<<(int)currentParticle.posY<<endl;
        }
    }
    return world;
}

int main(int argc, char **argv)
{

    if (argc > 1)
    {
        numberOfParticles = std::atof(argv[1]);
    }
    if (argc > 2)
    {
        gravitationalConstant = std::atof(argv[2]);
    }
    if (argc > 3)
    {
        totalRunTime = std::atof(argv[3]);
    }

    //Create a random device that can generate a normal distribution around 500
    std::random_device rndm;
    std::default_random_engine engine(rndm());
    std::normal_distribution<float> distribution(WORLDSIZE * SCALE / 2, WORLDSIZE);

    //Create NumberOfParticle particles and give them random locations to start
    std::vector<Particle> particles(numberOfParticles);
    for (int i = 0; i < numberOfParticles; i++)
    {
        particles[i] = Particle(distribution(engine), distribution(engine), i);
    }
    //create server
    MJPEGWriter server(7778);
    Mat blank = Mat::zeros(WORLDSIZE, WORLDSIZE, CV_8U);
    server.write(blank);
    blank.release();
    server.start();
    std::cout << "Open localhost:7777 and then press enter in the terminal to start";
    std::cin.ignore();
    auto frameStart = std::chrono::high_resolution_clock::now();
    auto frameEnd = frameStart;
    std::chrono::duration<double> deltaTime = frameStart - frameEnd;
    std::chrono::duration<double> totalTime = frameStart - frameEnd;
    int frameNumber = 0;
    while (true)
    {

        for (auto &currentParticle : particles)
        {
            for (auto &otherParticle : particles)
            {
                if (otherParticle.id != currentParticle.id)
                {
                    currentParticle.calcGravityToOther(otherParticle);
                }
            }
        }
        frameEnd = std::chrono::high_resolution_clock::now();
        deltaTime = frameEnd - frameStart;
        totalTime += deltaTime;
        double count = deltaTime.count();
        // std::cout << std::setprecision(9) << "Frame number " << frameNumber << " had a frame time of : " << count << " seconds" << std::endl;
        frameStart = std::chrono::high_resolution_clock::now();
        // std::cout << "Tracking Pixel 1" << std::endl;
        // std::cout << "acceleration x: " << particles[1].accelerationX << " acceleration y: " << particles[1].accelerationY << std::endl;
        for (auto &currentParticle : particles)
        {
            currentParticle.update(count);
        }
        Mat worldFrame = GetWorldFrame(particles);
        server.write(worldFrame);
        worldFrame.release();

        // std::cout << "x:" << particles[1].posX << " y:" << particles[1].posY << std::endl;
        // std::cout << "velocity x: " << particles[1].velocityX << " velocity y: " << particles[1].velocityY << std::endl;

        // std::cout << "Frame #" << frameNumber++ << endl;
    }
    server.stop();

    return 0;
}