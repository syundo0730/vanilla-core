#ifndef STDIO_COMMAND_PARSER_H_
#define STDIO_COMMAND_PARSER_H_

#include <memory>
#include <iostream>
#include "CommandBus.h"

class StdIOCommandParser
{
  public:
	static std::unique_ptr<StdIOCommandParser> instantiate();
	virtual Command parse(std::istream &stream) = 0;
};

#endif
