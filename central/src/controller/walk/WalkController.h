#ifndef WALK_CONTROLLER_H_
#define WALK_CONTROLLER_H_

#include <memory>

class CommandBus;
class WalkGenerator;
class LegControlService;

class WalkController {
  public:
	static std::unique_ptr<WalkController> instantiate(
			CommandBus &,
			WalkGenerator &,
			LegControlService &);
	virtual void update() = 0;
};

#endif
