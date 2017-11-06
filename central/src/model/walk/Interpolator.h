#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <memory>
#include "dimensional_types.h"

class Interpolator
{
  public:
	static std::unique_ptr<Interpolator> instantiate();
    virtual void initialize(Vector3 x0, Vector3 dx0, Vector3 ddx0, double Tdbl) = 0;
    virtual Vector3 getPosition(double t) = 0;
};

#endif // INTERPOLATOR_H