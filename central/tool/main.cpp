#include "RSApi.h"
#include "SerialApi.h"
#include "Timer.h"
#include <cstdint>
#include <iostream>

int main() {
    std::string portPath = "/dev/ttyS0";
    unsigned int baudRate = 115200;
    SerialApi serialApi(portPath, baudRate);
    auto rsApi = RSApi::instantiate(serialApi);

    std::cout << "this program will overwite all ids which is connectted to serial line" << std::endl;
    uint8_t id;
    std::cout << "input target id" << std::endl;
    std::cin >> id;

    std::cout << "will set id to " << id << std::endl;
    rsApi->setAllID(id);
    timer::waitMs(20);
    rsApi->writeToROM(id);
    timer::waitMs(20);
    rsApi->reboot(id);
    timer::waitMs(1000);

    std::cout << "move to zero test ..." << id << std::endl;
    rsApi->on();
    timer::waitMs(1000);
    rsApi->sendPosition(id, 0);
    timer::waitMs(1000);
    rsApi->off();
    std::cout << "end" << std::endl;
}