#include "MotionService.h"
#include "MotionLoader.h"
#include "JointRepository.h"
#include "JointAngle.h"
#include "Motion.h"
#include "conf.h"
#include <vector>
#include <iostream>

class MotionServiceImpl : public MotionService
{
  private:
    MotionLoader &motionLoader;
    Conf &conf;
    JointRepository &jointRepository;

    std::vector<Motion> motions;
    bool isPlaying;
    int playingMotionID, playingPoseIndex, playingStep;
    std::vector<int16_t> initialPose;

  public:
    MotionServiceImpl(
        MotionLoader &motionLoader,
        JointRepository& jointRepository,
        Conf &conf)
        : motionLoader(motionLoader),
          conf(conf),
          jointRepository(jointRepository),
          motions(motionLoader.load("motion.yaml"))
    {
        _reset();
    }
    void update()
    {
        if (!isPlaying) {
            return;
        }
        _update();
    }
    void start(int motionID) {
        playingMotionID = motionID;
        isPlaying = true;
    }
    void stop() {
        _reset();
    }
    void pause() {
        isPlaying = false;
    }

  private:
    void _setInitialPose() {
        auto settingMap = conf.Joint.SettingMap;
        initialPose.resize(settingMap.size());
        std::cout << "setInitialPose: ";
        for (const auto &kv : settingMap) {
            auto jointID = kv.first;
            auto current = jointRepository.getCurrentJointAngle(jointID).getAsDeciDegree();
            initialPose[jointID] = current;
            std::cout << current << ", ";
        }
        std::cout << std::endl;
    }
    void _reset() {
        isPlaying = false;
        playingMotionID = 0;
        playingPoseIndex = 0;
        playingStep = 0;
        _setInitialPose();
    }
    void _update() {
        auto m = motions.at(playingMotionID);
        auto poses = m.Poses;
        auto poseSize = poses.size();
        if (playingPoseIndex >= poseSize) {
            stop();
            return;
        }
        auto start = playingPoseIndex == 0 ? initialPose : poses.at(playingPoseIndex - 1).JointAnglesDeciDegree;
        auto end = poses.at(playingPoseIndex).JointAnglesDeciDegree;
        auto interval = poses.at(playingPoseIndex).Interval;
        setTargetWithInterpolation(start, end, interval);

        if (playingStep == interval - 1) {
            playingStep = 0;
            ++playingPoseIndex;
        } else {
            ++playingStep;
        }
    }
    void setTargetWithInterpolation(
        const std::vector<int16_t> &start,
        const std::vector<int16_t> &end,
        int interval) {
        auto angleLength = end.size();
        auto rate = (double)(playingStep + 1) / (double)interval;
        for (auto i = 0; i < angleLength; ++i) {
            auto s = start[i];
            auto e = end[i];
            int16_t target = s + (e - s) * rate;
            jointRepository.setTargetJointAngle(i, JointAngle(target));
            // std::cout << "id: " << i << " * "
            // << "s: " << s << " * "
            // << "e: " << e << " * "
            // << "t: " << target << " * "
            // << "rate: " << rate << " | ";
        }
        std::cout << std::endl;
    }
};

std::unique_ptr<MotionService> MotionService::instantiate(
    MotionLoader &motionLoader,
    JointRepository& jointRepository,
    Conf &conf)
{
    return std::make_unique<MotionServiceImpl>(
        motionLoader,
        jointRepository,
        conf);
}