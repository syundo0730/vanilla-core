#ifndef JOINT_CONF_H
#define JOINT_CONF_H

#include <cstdint>
#include <map>

namespace jointconf
{

enum MotorType { RS300, PRS, ENR };

struct Setting {
    MotorType motor_type;
    int offset;
};

extern const std::map<int, Setting> setting_map;

namespace prs {
uint16_t deciDegreeToDuty(int16_t ang);
}

namespace enr {
uint16_t deciDegreeToDuty(int16_t ang);
}

}
#endif
