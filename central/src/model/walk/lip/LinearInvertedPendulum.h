#ifndef LINEAR_INVERTED_PENDULUM_H
#define LINEAR_INVERTED_PENDULUM_H

#include <memory>
#include <map>
#include <vector>
#include "dimensional_types.h"
#include "physical_const.h"
#include "LegSide.h"
#include "LIPState.h"
class Conf;

class LinearInvertedPendulum
{
  public:
    static std::unique_ptr<LinearInvertedPendulum> instantiate(Conf &conf);
    virtual void setInitialParam(double _Tsup, double _z_c) = 0;
    virtual void setupFirstStep(Vector2IntMap wp, LegSide _supporting_leg_side) = 0;
    virtual void switchToNextStep(Vector2IntMap wp) = 0;
    virtual void optimize(double a, double b) = 0;
    virtual LIPState getState(double t, int n) = 0;
};
 
#endif // LINEAR_INVERTED_PENDULUM_H