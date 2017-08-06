#ifndef COMMAND_BUS_H_
#define COMMAND_BUS_H_

#include <memory>
#include <list>
#include <map>
#include <functional>
#include <cstdint>

enum class CommandType
{
	WalkStart,
	WalkStop,
	MotionStart,
	MotionStop,
};

struct Command
{
	CommandType commandType;
	std::map<int, uint16_t> payload;
};

class CommandBus
{
public:
	static std::unique_ptr<CommandBus> instantiate();
	using Subscriber = std::function<void(Command)>;
	virtual void subscribe(Subscriber subscriber) = 0;
	virtual void publish(Command command) = 0;
};

#endif
