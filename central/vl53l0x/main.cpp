#include "mraa.hpp"
#include "I2Cdev.h"
#include "VL53L0X.h"
#include "Timer.h"
#include <cstdint>
#include <iostream>

int main() {
    mraa::I2c i2c(1, true);
    I2Cdev i2cdev(i2c);
    VL53L0X vl53l0x(i2cdev);
    vl53l0x.init();
    vl53l0x.setTimeout(500);

    while (true) {
        std::cout << vl53l0x.readRangeSingleMillimeters() << std::endl;
        if (vl53l0x.timeoutOccurred()) {
            std::cout << "timed out" << std::endl;
        }
    }
}