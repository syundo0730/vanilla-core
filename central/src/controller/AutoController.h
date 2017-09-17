#ifndef AUTO_CONTROLLER_H_
#define AUTO_CONTROLLER_H_

#include <memory>

class CommandBus;
class ActiveRangeSensorArray;

class AutoController
{
  public:
    static std::unique_ptr<AutoController> instantiate(
        ActiveRangeSensorArray &,
        CommandBus &);
    virtual void update() = 0;
};

#endif
