#include "RSApi.h"
#include "utility.h"
#include "SerialApi.h"

class RSApiImpl : public RSApi
{
  private:
    SerialApi &serial_api;

  public:
    RSApiImpl(SerialApi &serial_api)
        : serial_api(serial_api) {}
    void sendMultiPosition(const std::vector<uint8_t> &ids, const std::vector<uint16_t> &pos) override
    {
        std::vector<uint8_t> buf;

        auto idsit = ids.begin();
        auto posit = pos.begin();
        for (; posit != pos.end(); ++idsit, ++posit)
        {
            //ID
            buf.push_back(*idsit);
            //POS
            auto posbyte = utility::devideToByte<
                std::vector<uint8_t>,
                uint16_t>(*posit);
            buf.insert(buf.end(), posbyte.begin(), posbyte.end());
        }
        serial_api.write(makePacket(0, 0, 0x1E, pos.size(), buf));
    }
    void sendPosition(uint8_t id, uint16_t pos) override
    {
        auto posbyte = utility::devideToByte<
            std::vector<uint8_t>,
            uint16_t>(pos);
        serial_api.write(makePacket(id, 0, 0x1E, 1, posbyte));
    }
    void on() override
    {
        serial_api.write(makePacket(0xFF, 0, 0x24, 0x01));
    }
    void off() override
    {
        serial_api.write(makePacket(0xFF, 0, 0x24, 0x00));
    }
    void setID(uint8_t currentID, uint8_t targetID) override
    {
    }
};

std::unique_ptr<RSApi> RSApi::instantiate(SerialApi& serialApi) {
    return std::make_unique<RSApiImpl>(serialApi);
}

std::vector<uint8_t> makePacket(uint8_t id, uint8_t flag, uint8_t adr, uint8_t cnt, std::vector<uint8_t> &data)
{
    std::vector<uint8_t> buf;
    buf.push_back(0xFA);              //Header :H
    buf.push_back(0xAF);              //Header :L
    buf.push_back(id);                //id
    buf.push_back(flag);              //flag
    buf.push_back(adr);               //adress
    buf.push_back(data.size() / cnt); //data size

    buf.push_back(cnt);                              //amount of servo
    buf.insert(buf.end(), data.begin(), data.end()); //data

    uint8_t sum = buf[2]; //check sum from id to data
    for (int i = 3, size = buf.size(); i < size; ++i)
    {
        sum = sum ^ buf[i];
    }
    buf.push_back(sum);

    return buf;
}

std::vector<uint8_t> makePacket(uint8_t id, uint8_t flag, uint8_t adr, uint8_t data)
{
    std::vector<uint8_t> buf;
    buf.push_back(0xFA); //Header :H
    buf.push_back(0xAF); //Header :L
    buf.push_back(id);   //id
    buf.push_back(flag); //flag
    buf.push_back(adr);  //adress
    buf.push_back(1);    //data size
    buf.push_back(1);    //amount of servo
    buf.push_back(data); //data

    uint8_t sum = buf[2]; //check sum from id to data
    for (int i = 3, size = buf.size(); i < size; ++i)
    {
        sum = sum ^ buf[i];
    }
    buf.push_back(sum);

    return buf;
}