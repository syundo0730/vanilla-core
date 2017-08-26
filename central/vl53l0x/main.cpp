#include "mraa.hpp"
#include "I2Cdev.h"
#include "RangeSensor.h"
#include "RangeSensorArray.h"
#include <cstdint>
#include <iostream>

int main() {
    mraa::I2c i2c(1, true);
    i2c.frequency(mraa::I2cMode::I2C_HIGH);
    I2Cdev i2cdev(i2c);
    auto sa = RangeSensorArray::instantiate(i2cdev);
    while (true) {
        auto ranges = sa->readRanges();
        for (auto &r : ranges) {
            std::cout << r << ", ";
        }
        std::cout << std::endl;
    }
}