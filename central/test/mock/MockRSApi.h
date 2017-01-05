#ifndef MOCK_RS_API_H
#define MOCK_RS_API_H

#include "gmock/gmock.h"
#include "RSApi.h"

class MockRSApi : public RSApi
{
  public:
    MOCK_METHOD2(sendMultiPosition, void(const std::vector<uint8_t> &, const std::vector<uint16_t> &));
    MOCK_METHOD2(sendPosition, void(uint8_t id, uint16_t pos));
    MOCK_METHOD0(on, void(void));
    MOCK_METHOD0(off, void(void));
};
#endif