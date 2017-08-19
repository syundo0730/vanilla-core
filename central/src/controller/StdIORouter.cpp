#include "StdIORouter.h"
#include "StdIOCommandParser.h"
#include <iostream>

class StdIORouterImpl : public StdIORouter
{
  private:
    CommandBus &commandBus;
    StdIOCommandParser &commandParser;

  public:
    StdIORouterImpl(CommandBus &commandBus, StdIOCommandParser &commandParser)
        : commandBus(commandBus),
          commandParser(commandParser) {}

    void route() override
    {
        auto command = commandParser.parse(std::cin);
        std::cout << "parsed" << std::endl;
        commandBus.publish(command);
    }
};

std::unique_ptr<StdIORouter> StdIORouter::instantiate(
    CommandBus &commandBus,
    StdIOCommandParser &commandParser)
{
    return std::make_unique<StdIORouterImpl>(
        commandBus,
        commandParser);
}