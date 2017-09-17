#ifndef DEPENDENCY_H
#define DEPENDENCY_H

#include <memory>
#include "MainController.h"
#include "StdIORouter.h"
#include "AutoController.h"

class Dependency
{
  public:
	static std::unique_ptr<Dependency> instantiate();
	virtual MainController *getMainController() = 0;
	virtual StdIORouter *getStdIORouter() = 0;
	virtual AutoController *getAutoController() = 0;
};

#endif // DEPENDENCY_H