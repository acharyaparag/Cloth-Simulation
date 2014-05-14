#include "Vector.h"
#include "particle.h"

Vector3d& Particle::getVelocity()
{ 
return Velocity;
}

Vector3d& Particle::getPosition()
{ 
return Position;
}

float& Particle::getMass()
{ 
return Mass;
}

float& Particle::getLifespan()
{ 
return Lifespan;
}

float& Particle::getFade()
{ 
return Fade;
}

Vector3d& Particle::getForce()
{ 
return Force;
}

void Particle::setForce(Vector3d f)
{  
  Force = f;
}

Vector3d& Particle::getAccel()
{
 return Accel;
}

void Particle::setFixed(bool val)
{
  Fixed = val;
}

bool Particle::getFixed()
{
  return Fixed;
}

