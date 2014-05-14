#ifndef STRUT__H
#define STRUT__H

#include "particle.h"

class Strut
{
private:
        float K;
        float D;
        float rest_length;
        Particle *P1;
        Particle *P2; 
        
public:
        
        Strut(Particle *p1,Particle *p2)
          {
           P1 = p1;
           P2 = p2;
           K = 4.44;
           D = 0.67;
           Vector3d vec = p1->getPosition() - p2->getPosition();
           rest_length = vec.norm();
          }

        float getrestLength();
        float getK();
        float getD();
        Particle* getParticle1();
        Particle* getParticle2();

};

#endif
