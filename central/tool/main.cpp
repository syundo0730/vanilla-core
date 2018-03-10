#include "RSApi.h"
#include "SerialApi.h"
#include "Timer.h"
#include <cstdint>
#include <iostream>

int main() {
    std::string portPath = "/dev/ttyAMA0";
    unsigned int baudRate = 115200;
    SerialApi serialApi(portPath, baudRate);
    auto rsApi = RSApi::instantiate(serialApi);

    std::cout << "this program will overwite all ids which is connectted to serial line" << std::endl;
    int id;
    std::cout << "input target id" << std::endl;
    std::cin >> id;

    for (int currentID = 1; currentID < 255; ++currentID)
    {
        std::cout << currentID << " will set id to " << id << std::endl;
        rsApi->setID(currentID, id);
        timer::waitMs(50);
    }
    std::cout << "writing to ROM" << std::endl;
    rsApi->writeToROM(id);
    timer::waitMs(20);
    std::cout << "will reboot" << std::endl;
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

// void onRead(const uint8_t* data, std::size_t size) {
//     std::cout << "data size: " << std::endl;
//     std::cout << data << std::endl;
// }

// int main() {
//     std::string portPath = "/dev/ttyAMA0";
//     unsigned int baudRate = 115200;
//     SerialApi bluetooth(portPath, baudRate);

//     bluetooth.setListener(onRead);
    
//     int willExit;
//     std::cout << "Exit ?" << std::endl;
//     std::cin >> willExit;
// }