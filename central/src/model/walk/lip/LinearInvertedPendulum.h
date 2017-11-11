#ifndef LINEAR_INVERTED_PENDULUM_H
#define LINEAR_INVERTED_PENDULUM_H

#include <memory>
#include <map>
#include <vector>
#include "dimensional_types.h"
#include "physical_const.h"
#include "LegSide.h"
#include "COGState.h"
class Conf;
class Gait;

class LinearInvertedPendulum
{
  public:
    static std::unique_ptr<LinearInvertedPendulum> instantiate(Conf &conf);
    virtual void setParams(double _Tsup, double _Zc) = 0;
    virtual void optimizeStep(bool isFirstStep, LegSide supportingLegSide, const Gait &gait) = 0;
    virtual COGState getCOGState(double t, int n) = 0;
    virtual Vector3 getTargetLegPos(int n) = 0;
};
 
#endif // LINEAR_INVERTED_PENDULUM_H