#ifndef PUSHER_H
#define PUSHER_H


#include <Plasmas//particles.h>

class Vector;
#if 0
class Pusher
{
public:
    Pusher();
    ~Pusher();

   virtual void accel(Particles &particles,
              double dt, double m, double q,
              const Vector &E,
              const Vector &B)=0;

   virtual void move(Particles &particles)=0;
};
#endif

#endif // PUSHER_H
