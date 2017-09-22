#include "RangeDataProcessor.h"
#include "CommandBus.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include "InvalidRangeException.h"

class RangeDataProcessorImpl : public RangeDataProcessor
{
  private:
	using t_indexes = std::vector<int>;
	std::map<int, t_indexes> indexesMap;
	const double NEAR_THREASHOLD = 200;
	const double DETECTABLE_THREASHOLD = 400;
	const double ERROR_THREASHOLD = 8000;

  public:
	RangeDataProcessorImpl()
		: indexesMap({
			  { 0, {3, 4, 5, 6} },
			  { 1, {7, 8, 9, 10, 11} },
			  { 2, {12, 13, 14, 15} },
			  { 3, {16, 17, 18, 19, 20} },
			  { 4, {21, 22, 23, 24} },
			  { 5, {25, 26, 27, 28, 29} },
			  { 6, {30, 31, 32, 33} },
			  { 7, {34, 35, 0, 1, 2} },
		  })
	{
	}
	RangeState process(const t_ranges &ranges)
	{
		std::array<double, 8> averages;
		for (auto &kv : indexesMap)
		{
			auto i = kv.first;
			auto indexes = kv.second;
			auto ave = filter(ranges, indexes);
			if (ave < 0) {
				throw InvalidRangeException(i);
			}
			averages[i] = ave;
		}

		auto it = std::min_element(averages.begin(), averages.end());
		size_t index = std::distance(averages.begin(), it);
		auto distance = averages[index];
		Range range = Range::Far;
		if (isNear(distance)) {
			range = Range::Near;
		} else if (isDetectable(distance)) {
			range = Range::Detectable;
		}
		return RangeState{
			TargetIndex : index,
			Range : range,
		};
	}

  private:
	bool isNear(double distance)
	{
		return distance < NEAR_THREASHOLD;
	}
	bool isDetectable(double distance)
	{
		return distance < DETECTABLE_THREASHOLD;
	}
	double filter(const t_ranges &ranges, const t_indexes &indexes)
	{
		double sum = 0;
		int count = 0;
		for (auto &i : indexes)
		{
			auto r = ranges[i];
			if (r > ERROR_THREASHOLD) {
				continue;
			}
			sum += r;
			++count;
		}
		if (count == 0)
		{
			return -1;
            throw std::runtime_error("too short");
		}
		return sum / count;
	}
};

std::unique_ptr<RangeDataProcessor> RangeDataProcessor::instantiate()
{
	return std::make_unique<RangeDataProcessorImpl>();
}