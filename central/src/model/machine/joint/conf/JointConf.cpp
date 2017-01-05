#include "JointConf.h"

namespace jointconf
{

const std::map<int, Setting> setting_map = {
    {0, Setting{PRS, 0}},
    {0, Setting{PRS, 0}},
    {0, Setting{PRS, 0}},
    {0, Setting{PRS, 0}},
    {0, Setting{PRS, 0}},
    {0, Setting{PRS, 0}},
    {0, Setting{PRS, 0}},
    {0, Setting{PRS, 0}}
};

namespace prs {
uint16_t deciDegreeToDuty(int16_t ang) {
	return 0;
}
}

namespace enr {
uint16_t deciDegreeToDuty(int16_t ang) {
	return 0;
}
}

}