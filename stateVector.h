#ifndef STATEVECTOR__H
#define STATEVECTOR__H

#include "Vector.h"

#define no_particles_width   40
#define no_particles_height  40

class StateVector
{
private:

  static const int SIZE = no_particles_width*no_particles_height*2;
  Vector3d SV[SIZE];
  Vector3d Normal[SIZE/2];

public:

        StateVector()             
        {
           for (int i = 0;i<SIZE;i++)
                {
                 SV[i].set(0,0,0);
                }
        }
       
        StateVector(Vector3d *vec)
        {
          for (int i = 0;i<SIZE;i++)
             {
                 SV[i] = vec[i];
             }
        }
      
        Vector3d& getData(int);
        Vector3d& getNormal(int);
        StateVector operator*(double);
        StateVector operator+(StateVector);

};

#endif
