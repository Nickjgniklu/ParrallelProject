#include "Particle.hpp"
#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include "MJPEGWriter.h"
#include <mpi.h>

#define WORLDSIZE 1000
#define SCALE 2
#define MCW MPI_COMM_WORLD
float gravitationalConstant = 100000.0f;
// float accelerationCap = 1000.0f;
int numberOfParticles = 512;
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
    int frameLimit=500;
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MCW, &size);
    MPI_Comm_rank(MCW, &rank);
    MPI_Barrier(MCW);
    //create way to send particle struct
    const int nitems = 6;
    int timePerFrame=0;
    int blocklengths[6] = {1, 1, 1, 1, 1, 1};
    MPI_Datatype types[6] = {MPI_FLOAT, MPI_FLOAT, MPI_FLOAT, MPI_FLOAT, MPI_FLOAT, MPI_FLOAT};
    MPI_Datatype mpi_particle_type;
    MPI_Aint offsets[6];
    offsets[0] = offsetof(Particle, accelerationX);
    offsets[1] = offsetof(Particle, accelerationY);
    offsets[2] = offsetof(Particle, posX);
    offsets[3] = offsetof(Particle, posY);
    offsets[4] = offsetof(Particle, velocityX);
    offsets[5] = offsetof(Particle, velocityY);
    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_particle_type);
    MPI_Type_commit(&mpi_particle_type);

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
    std::vector<Particle> particles(numberOfParticles);
    std::vector<Particle> rparticles(numberOfParticles);

    int blocksize = numberOfParticles / size;
    if (rank == 0)
    {

        //Create a random device that can generate a normal distribution around 500
        std::random_device rndm;
        std::default_random_engine engine(rndm());
        std::normal_distribution<float> distribution(WORLDSIZE * SCALE / 2,WORLDSIZE/4);

        //Create NumberOfParticle particles and give them random locations to start
        for (int i = 0; i < numberOfParticles; i++)
        {
            particles[i] = Particle(distribution(engine), distribution(engine));
        }
    }
    //create server
    MJPEGWriter server;
    if (rank == 0)
    {
        server = MJPEGWriter(7777);
        Mat blank = Mat::zeros(WORLDSIZE, WORLDSIZE, CV_8U);
        server.write(blank);
        blank.release();
        server.start();
        std::cout << "Open localhost:7777 and then press enter in the terminal to start";
        std::cin.ignore();
    }
    double startTime=MPI_Wtime();
    auto frameStart = std::chrono::high_resolution_clock::now();
    auto frameEnd = frameStart;
    std::chrono::duration<double> deltaTime = frameStart - frameEnd;
    std::chrono::duration<double> totalTime = frameStart - frameEnd;
    int frameNumber = 0;
    while (true)
    {
        MPI_Bcast(&particles[0], particles.size(), mpi_particle_type, 0, MCW);
        MPI_Barrier(MCW);
        
        for (int i = blocksize * rank; i < (blocksize * rank) + blocksize; i++)
        {

            for (auto &otherParticle : particles)
            {
                particles[i].calcGravityToOther(otherParticle);
            }
        }

        frameEnd = std::chrono::high_resolution_clock::now();
        deltaTime = frameEnd - frameStart;
        totalTime += deltaTime;
        
        double count = deltaTime.count();
          for (int i = blocksize * rank; i < (blocksize * rank) + blocksize; i++)
         {
           
             particles[i].update(count);
         }
        MPI_Barrier(MCW);
       
        MPI_Gather(&particles[blocksize*rank],blocksize,mpi_particle_type,&rparticles[0],blocksize,mpi_particle_type,0,MCW);
        
        frameStart = std::chrono::high_resolution_clock::now();
       

       
        if (rank == 0)
        {
           
            Mat worldFrame = GetWorldFrame(rparticles);
            server.write(worldFrame);
            worldFrame.release();
            particles=rparticles;
        std::cout << "Frame #" << frameNumber++ << std::endl;



        }
                MPI_Barrier(MCW);
        if(frameNumber>=frameLimit){
            break;
        }


    }
    if (rank == 0)
    {
        std::cout<<MPI_Wtime()-startTime<<std::endl;

        server.stop();
    }
    MPI_Type_free(&mpi_particle_type);
    MPI_Finalize();

    return 0;
}