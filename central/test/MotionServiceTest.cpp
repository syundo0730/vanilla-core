#include "gtest/gtest.h"
#include "MotionService.h"
#include "conf.h"
#include "MockJointRepository.h"
#include "MockMotionLoader.h"

namespace
{

using ::testing::_;
using ::testing::Return;

class MotionServiceTest : public ::testing::Test
{
};

TEST_F(MotionServiceTest, update)
{
    std::vector<Motion> motions{
        Motion{
            ID: 0,
            Poses: {
                MotionPose{
                    Interval: 10,
                    JointAnglesDeciDegree: {100,200,300,400,500}
                },
                MotionPose{
                    Interval: 5,
                    JointAnglesDeciDegree: {0,0,0,0,0}
                },
            },
        },
        Motion{
            ID: 1,
            Poses: {
                MotionPose{
                    Interval: 100,
                    JointAnglesDeciDegree: {100,200,300,400,500}
                },
            },
        },
    };

    MockMotionLoader mockMotionLoader;
    EXPECT_CALL(mockMotionLoader, load(_)).WillOnce(Return(motions));

    MockJointRepository mockJointRepository;
    EXPECT_CALL(mockJointRepository, getCurrentJointAngle(_))
    .Times(12 * 3)
    .WillRepeatedly(Return(JointAngle(10)));
    EXPECT_CALL(mockJointRepository, setTargetJointAngle(_, _)).Times(5 * (10 + 5));

    auto conf = Conf::defaultConf();
    auto motionService = MotionService::instantiate(
        mockMotionLoader,
        mockJointRepository,
        conf);

    motionService->start(0);

    for (auto i = 0; i < 20; ++i) {
        motionService->update();
    }
}
}