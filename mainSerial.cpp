#include "Particle.hpp"
#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include "MJPEGWriter.h"

#define WORLDSIZE 1000

float gravitationalConstant = 1.223f;
int numberOfParticles = 1000;
int iterations = 10;

Mat GetWorldFrame(std::vector<Particle> particles)
{
    Mat world = Mat::zeros(WORLDSIZE, WORLDSIZE, CV_8U);
    for (auto currentParticle : particles)
    {
            if(currentParticle.posX>=0 &&currentParticle.posX<WORLDSIZE&&currentParticle.posY>=0 &&currentParticle.posY<WORLDSIZE ){
            world.at<uchar>( (int)currentParticle.posY,(int)currentParticle.posX) = 254;
            //cout<<"x:"<<(int)currentParticle.posX<<" y:"<<(int)currentParticle.posY<<endl;
            }
    }
    return world;
}

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
    //create server
    MJPEGWriter server(7777);
    Mat blank =Mat::zeros(WORLDSIZE, WORLDSIZE, CV_8U);
    server.write(blank);
    blank.release();
    server.start();
    std::cout<<"Open localhost:7777 and then press enter in the terminal to start";
    std::cin.ignore();
    auto totalStart = std::chrono::high_resolution_clock::now();
    auto frameStart = std::chrono::high_resolution_clock::now();
    auto frameEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> deltaTime = frameStart - frameEnd;

    for (int i = 0; i < 1000; i++)
    {
        frameEnd = std::chrono::high_resolution_clock::now();

        for (auto currentParticle : particles)
        {
            for (auto otherParticle : particles)
            {
                currentParticle.calcGravityToOther(otherParticle);
            }
        }
        deltaTime = frameEnd - frameStart;
        double count = deltaTime.count();
        frameStart = std::chrono::high_resolution_clock::now();
        for (auto currentParticle : particles)
        {
            currentParticle.update(count);
        }
        Mat worldFrame=GetWorldFrame(particles);
        server.write(worldFrame);
        worldFrame.release();

        std::cout<<"Tracking Pixel 500"<<endl;
        cout<<"x:"<<particles[500].posX<<" y:"<<particles[500].posY<<endl;
        cout<<"acel x:"<<particles[500].accelerationX<<"acel y:"<<particles[500].accelerationY<<endl;


        std::cout<<"Frame #"<<i<<endl;

    }
    auto totalEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalDiff = totalEnd - totalStart;
    
    std::cout << "It took " << totalDiff.count() << " seconds to do " << iterations << " iterations of a universe with " << numberOfParticles << " particles in it." << std::endl;
        server.stop();

    return 0;
}