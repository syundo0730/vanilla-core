#ifndef UTILITY_H
#define UTILITY_H

#include <cstdint>

namespace utility {
	template <typename T, typename U>  T devideToByte(U src) {
		T buf;
		auto len = sizeof(src);
		for (int i = 0; i < len; ++i) {
			buf.push_back(static_cast<uint8_t>(src >> (8 * i)));
		}
		return buf;
	}

	template <typename T, typename U>  T devideToByte(U* src) {
		return devideToByte(*src);
	}
	
	int16_t joinTo16Bit(uint8_t l, uint8_t h);

	int16_t radianToDeciDegree(double rad);
}
 
#endif // UTILITY_H