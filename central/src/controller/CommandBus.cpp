#include "CommandBus.h"

class CommandBusImpl : public CommandBus
{
  private:
    std::list<Subscriber> subscribers;

  public:
    void subscribe(Subscriber subscriber) override
    {
        subscribers.push_back(subscriber);
    }

    void publish(Command command) override
    {
        for (auto subscriber : subscribers)
        {
            subscriber(command);
        }
    }
};

std::unique_ptr<CommandBus> CommandBus::instantiate() {
	return std::make_unique<CommandBusImpl>();
}