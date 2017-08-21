#ifndef MOCK_RS_API_H
#define MOCK_RS_API_H

#include "gmock/gmock.h"
#include "RSApi.h"

class MockRSApi : public RSApi
{
  public:
    MOCK_METHOD2(sendMultiPosition, void(const std::vector<uint8_t> &, const std::vector<uint16_t> &));
    MOCK_METHOD2(sendPosition, void(uint8_t, uint16_t));
    MOCK_METHOD0(on, void(void));
    MOCK_METHOD0(off, void(void));
    MOCK_METHOD2(setID, void(uint8_t, uint8_t));
    MOCK_METHOD1(setAllID, void(uint8_t));
    MOCK_METHOD1(writeToROM, void(uint8_t));
    MOCK_METHOD1(reboot, void(uint8_t));
};
#endif