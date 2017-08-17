#ifndef STDIO_ROUTER_H_
#define STDIO_ROUTER_H_

#include <memory>
#include <iostream>
#include "CommandBus.h"

class CommandBus;
class StdIOCommandParser;

class StdIORouter
{
public:
	static std::unique_ptr<StdIORouter> instantiate(CommandBus &, StdIOCommandParser &);
	virtual void route() = 0;
};

#endif
