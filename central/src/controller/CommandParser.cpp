#include "CommandParser.h"
#include "utility.h"
#include <stdexcept>

// command
// | 0xAF | 0xFA | 0x02 |   0x02  | 0x00 | 0x01 | 0x11 |
// | head | head | comm | bodylen | valh | vall | sum  |
class CommandParserImpl : public CommandParser
{
  public:
    CommandParserImpl() {}
    Command parse(const uint8_t *data, std::size_t length) override
    {
        if (length < 3)
        {
            throw std::runtime_error("too short");
        }
        if (!validateHeader(data[0], data[1]))
        {
            throw std::runtime_error("invalid header");
        }
        if (!validateCheckSum(data, length))
        {
            throw std::runtime_error("invalid checksum");
        }
        CommandType commandType;
        auto cmdData = data[2];
        switch (cmdData)
        {
        case 0x01:
            commandType = CommandType::WalkStop;
            break;
        case 0x02:
            commandType = CommandType::WalkStart;
            break;
        case 0x05:
            commandType = CommandType::SetJointAngle;
            break;
        }
        int bodyLen = data[3];
        std::map<int, uint16_t> payload;
        for (int i = 0, j = 4; i < bodyLen; ++i, j+=2)
        {
            payload[i] = utility::joinTo16Bit(
                data[j],
                data[j + 1]);
        }
        return Command{
            commandType: commandType,
            payload: payload,
        };
    }

  private:
    bool validateHeader(uint8_t h, uint8_t l)
    {
        return h == 0xFA && l == 0xAF;
    }
    bool validateCheckSum(const uint8_t *data, std::size_t length)
    {
        uint8_t sum = data[2];
        int last = length - 1;
        for (int i = 3; i < last; ++i)
        {
            sum = sum ^ data[i];
        }
        return sum == data[last];
    }
};

std::unique_ptr<CommandParser> CommandParser::instantiate() {
    return std::make_unique<CommandParserImpl>();
}