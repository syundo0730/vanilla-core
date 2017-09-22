#include "gtest/gtest.h"
#include "ActiveRangeSensorArray.h"
#include "MockJointRepository.h"
#include "MockRangeSensorArray.h"
#include <cstdint>

namespace
{
using ::testing::_;
using ::testing::Return;

class ActiveRangeSensorArrayTest : public ::testing::Test
{
};

TEST_F(ActiveRangeSensorArrayTest, update)
{
    MockJointRepository mockJointRepository;
    EXPECT_CALL(mockJointRepository, setTargetJointAngle(_, _)).Times(101);

    MockRangeSensorArray mockRangeSensorArray;
    std::vector<uint16_t> ret = {1, 2, 3, 4};
    EXPECT_CALL(mockRangeSensorArray, readRanges())
    .Times(100)
    .WillRepeatedly(Return(ret));

    auto asa = ActiveRangeSensorArray::instantiate(
        mockRangeSensorArray,
        mockJointRepository,
        0);

    using t_ranges = std::vector<uint16_t>;
    std::function<void(const t_ranges&)> onRead = [&](
        const t_ranges& data
    ){
        std::cout << "onRead" << std::endl;
        for (auto &d : data) {
            std::cout << d << ", ";
        }
        std::cout << std::endl;
    };
    asa->setListener(onRead);
    asa->moveToInitialPosition();

    for (int i = 0; i < 100; ++i) {
        asa->update();
    }
}
}