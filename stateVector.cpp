#include "stateVector.h"

Vector3d& StateVector::getData(int index)
{
 return SV[index];
}

Vector3d& StateVector::getNormal(int index)
{
 return Normal[index];
}


StateVector StateVector::operator*(double b)
{
  StateVector a;
  for(int i = 0; i<SIZE; i++)
      a.SV[i] =  SV[i]*b;
  return a;
}

StateVector StateVector::operator+(StateVector b)
{
 StateVector a;
 for (int i = 0;i<SIZE;i++)
      a.SV[i] = SV[i] + b.SV[i];
 return a;
}



