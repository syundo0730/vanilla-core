#ifndef RS_API_H
#define RS_API_H

#include <memory>
#include <vector>

class SerialApi;

class RSApi
{
  public:
	static std::unique_ptr<RSApi> instantiate(SerialApi &serialApi);
	virtual void sendMultiPosition(const std::vector<uint8_t> &ids, const std::vector<uint16_t> &pos) = 0;
	virtual void sendPosition(uint8_t id, uint16_t pos) = 0;
	virtual void on() = 0;
	virtual void off() = 0;
    virtual void setID(uint8_t currentID, uint8_t targetID) = 0;
    virtual void setAllID(uint8_t targetID) = 0;
    virtual void writeToROM(uint8_t id) = 0;
    virtual void reboot(uint8_t id) = 0;
};

std::vector<uint8_t> makeFlagPacket(uint8_t id, uint8_t flag, uint8_t len);
std::vector<uint8_t> makePacket(uint8_t id, uint8_t flag, uint8_t adr, uint8_t cnt, std::vector<uint8_t> &data);
std::vector<uint8_t> makePacket(uint8_t id, uint8_t flag, uint8_t adr, uint8_t data);

#endif // RS_API_H