#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H
#include "CommandBus.h"
#include <cstdint>
#include <memory>

class CommandParser {
  public:
    static std::unique_ptr<CommandParser> instantiate();
    virtual Command parse(const uint8_t *data, int length) = 0;
};

#endif