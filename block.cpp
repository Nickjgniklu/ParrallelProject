#include "InfInt.h"
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <math.h>
#include <iostream>
#include <string>
#include <mpi.h>
#define MCW MPI_COMM_WORLD



///<summary>
///calculates fibonacci number in blocks
///</summary>
///<param name="goal">The number of fibonacci numbers to calculate</param>
///<param name="blockSize">The size of blocks that processors will be given</param>
///<param name="numberOfProcess">The number of processors</param>
///<param name="rank">The rank of the current processor</param>
void FibInBlocks(int goal, int blockSize, int numberOfProcess, int rank)
{
    int start = blockSize * rank;
    int end = blockSize + start;
    //if the next block is in the range we want to calculate
    while (start < goal)
    {
        //do work for this block
        //this block may be incomplete if it is the last one
        for (int currentLocation = start; currentLocation < end && currentLocation < goal; currentLocation++)
        {
            //TODO Calculate fibonacci numbers here
            //If it is the first number or second number you must use the formula
            if (currentLocation == start || currentLocation == start + 1)
            {
                //use formula
            }
            else
            {
                //use formula or sum previous numbers
            }
            //Other wise you may add the 2 previous numbers together
            std::cout << "Process " + std::to_string(rank) + " will do fibonacci of " + std::to_string(currentLocation) + "\n";
        }
        //jump to this process's next chunk
        start += blockSize * numberOfProcess;
        end = blockSize + start;
    }
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int world_size;
    MPI_Comm_size(MCW, &world_size);
    int rank;
    MPI_Comm_rank(MCW, &rank);

    int Goal = 100;     //how many tasks to do
    int blockSize = 10; //the size of block to send to processes

    FibInBlocks(100, 10, world_size, rank);
    MPI_Finalize();
    return 0;
}
//to block each proces must decided which ranks it will be calculating
//ie if we need to know 100 fib # and have 4 process we must decided how to split work
//the largest block would be one chunk per process
//smallest block of work must containat least one task though

//using stride loop each process can spilt its work