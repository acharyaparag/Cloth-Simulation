#ifndef PARTICLE__H
#define PARTICLE__H

#include "Vector.h"

class Particle
{
private:

  Vector3d Velocity;
  Vector3d Position;
  Vector3d Force;
  Vector3d Accel;
  float Mass;
  float Lifespan;
  float Fade;
  bool Fixed;

public:

        Particle(Vector3d vel,Vector3d pos,Vector3d force,Vector3d accel, int mass,float life,float fade) : Velocity(vel), Position(pos),Force(force),Accel(accel),Mass(mass),Lifespan(life),Fade(fade),Fixed(false)              
        { }
	Particle(){ }

        Vector3d& getVelocity();
        Vector3d& getPosition();
        int& getAge();
        float& getMass();
        float& getLifespan();
        float& getFade();
        Vector3d& getForce();
        void setForce(Vector3d);
        Vector3d& getAccel();
        void setFixed(bool val);
        bool getFixed();

};

#endif
