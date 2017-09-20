#include "StdIOCommandParser.h"
#include <stdexcept>
#include <string>
#include <regex>
#include <cstdint>

class StdIOCommandParserImpl : public StdIOCommandParser
{
  public:
    StdIOCommandParserImpl() {}

    Command parse(std::istream &stream) override
    {
        std::string command_str;
        stream >> command_str;
        Command command;
        if (std::regex_match(command_str, std::regex("stop_motion")))
        {
            command = createCommand(CommandType::MotionStop);
        }
        else if (parseMotionCommand(command_str, command))
        {
        }
        else if (std::regex_match(command_str, std::regex("show_joint_angles")))
        {
            command = createCommand(CommandType::ShowJointAngles);
        }
        else if (parseSetJointAngleCommand(command_str, command))
        {
        }
        else if (std::regex_match(command_str, std::regex("load_config")))
        {
            command = createCommand(CommandType::LoadConfig);
        }
        return command;
    }

  private:
    bool parseMotionCommand(std::string command_str, Command& command)
    {
        std::regex r("start_motion:(\\d+)");
        std::smatch m;
        bool matched = std::regex_search(command_str, m, r);
        if (!matched) {
            return false;
        }
        uint16_t id = std::stoi(m[1]);
        command = createCommand(CommandType::MotionStart, id);
        return true;
    }
    bool parseSetJointAngleCommand(std::string command_str, Command& command)
    {
        std::regex r("set_joint_angle:(\\d+),(-?\\d+)");
        std::smatch m;
        bool matched = std::regex_search(command_str, m, r);
        if (!matched) {
            return false;
        }
        uint16_t id = std::stoi(m[1]);
        uint16_t pos = std::stoi(m[2]);
        command = createCommand(CommandType::SetJointAngle, id, pos);
        return true;
    }
};

std::unique_ptr<StdIOCommandParser> StdIOCommandParser::instantiate()
{
    return std::make_unique<StdIOCommandParserImpl>();
}