#include "StdIOCommandParser.h"
#include <stdexcept>
#include <string>

class StdIOCommandParserImpl : public StdIOCommandParser
{
  public:
    StdIOCommandParserImpl() {}

    Command parse(std::istream &stream) override
    {
        std::string command_str;
        stream >> command_str;
        Command command;
        if (command_str == "a")
        {
            return createCommand(CommandType::MotionStart, 0);
        }
        else if (command_str == "b")
        {
            return createCommand(CommandType::MotionStart, 1);
        }
        else if (command_str == "c")
        {
            return createCommand(CommandType::MotionStop);
        }
        return Command();
    }
};

std::unique_ptr<StdIOCommandParser> StdIOCommandParser::instantiate()
{
    return std::make_unique<StdIOCommandParserImpl>();
}