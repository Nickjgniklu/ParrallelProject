#include <vector>

float gravitationalConstant = 1.223f;

struct Particle
{

    int accelerationX = 0;
    int accelerationY = 0;
    int posX = 0;
    int posY = 0;
    int velecityX = 0;
    int velecityY = 0;
};

int main()
{

    std::vector<Particle> particles;

    for (auto currentParticle : particles)
    { //<--
        for (auto otherParticle : particles)
        {
            //Calculate the pull of gravity between current, and other. Add to acceleration.
            //currentParticle.acceleration += (gravitationalConstant / (currentParticle.pos distanceTo otherParticle))
        }
    }

    for (auto currentParticle : particles)
    { //<-- split
        //Adjust particle's position based on acceleration and velocity.
        //part.pos += part.vel * time + part.accell * .5 * time^2
        //Adjust particle's velocity
        //part.vel += part.accell * time
        //reset acceleration for next frame
        // part.accel = 0
    }

    return 0;
}