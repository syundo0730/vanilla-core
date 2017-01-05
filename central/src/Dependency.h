#ifndef DEPENDENCY_H
#define DEPENDENCY_H

#include <memory>
#include "MainController.h"

class Dependency
{
public:
	static std::unique_ptr<Dependency> instantiate();
	virtual MainController *getMainController() = 0;
};

#endif // DEPENDENCY_H