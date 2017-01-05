#ifndef CP_STABILIZER_H
#define CP_STABILIZER_H

#include <memory>
#include "dimensional_types.h"

class Conf;

class CPStabilizer
{
public:
	static std::unique_ptr<CPStabilizer> instantiate(Conf &conf);
	virtual double stabilize(const Vector2 &state_d, const Vector2 &state) = 0;
};

#endif // CP_STABILIZER_H
