# Gravity Simulator 
Uses openMPI to simulate gravity. a mjpeg server shows frames as they are rendered.
## Dependencies
Requires OpenCV and openMPI
# How To Install OpenCV
	sudo apt-get update
	sudo apt-get upgrade
	sudo apt-get install libopencv-dev

## Compile	
	g++ MJPEGWriter.cpp MJPEGTest.cpp -o MJPEG `pkg-config --cflags --libs opencv` -lpthread -lopencv_highgui -lopencv_core -std=c++11
## run test
 ./MJPG
 A diagonal line should appear if you open localhost:7777

# Using the serial version

## compile serial version
g++ MJPEGWriter.cpp mainSerial.cpp -o serial `pkg-config --cflags --libs opencv` -lpthread -lopencv_highgui -lopencv_core -std=c++11 -O2

## run it
./serial
open localhost 7777
then press enter in the terminal to start

# Using the parallel version

## compile mainP.cpp
mpic++ MJPEGWriter.cpp mainP.cpp -o parrel `pkg-config --cflags --libs opencv` -lpthread -lopencv_highgui -lopencv_core -std=c++11 -O2

## running
NOTE 
The number of particles must be divisible by the number of processors

mpirun -np 4 ./parrel 1024

