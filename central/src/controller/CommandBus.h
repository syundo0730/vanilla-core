#ifndef COMMAND_BUS_H_
#define COMMAND_BUS_H_

#include <memory>
#include <list>
#include <map>
#include <functional>
#include <cstdint>
#include <vector>

enum class CommandType
{
	WalkStart,
	WalkStop,
	MotionStart,
	MotionStop,
	SetJointAngle,
	ShowJointAngles,
	LoadConfig,
	Enable,
	Free,
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

template <class... Payloads>
Command createCommand(CommandType type, Payloads... payloads) {
	std::vector<uint16_t> v = { payloads... };
	std::map<int, uint16_t> payload;
	int index = 0;
	for (const auto &p : v) {
		payload[index] = p;
		++index;
	}
	return Command{
		commandType: type,
		payload: payload,
	};
}

#endif
