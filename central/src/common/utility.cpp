#include "utility.h"
#include <cmath>

namespace utility {
	int16_t radianToDeciDegree(double rad) {
		return 1800 * rad / M_PI;
	}
	int16_t joinTo16Bit(uint8_t l, uint8_t h) {
		return (static_cast<uint16_t>(h) << 8) | static_cast<uint16_t>(l);
	}
}