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
        for (const auto &kv : settingMap) {
            auto jointID = kv.first;
            auto current = jointRepository.getCurrentJointAngle(jointID).getAsDeciDegree();
            initialPose[jointID] = current;
        }
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
        auto current = playingPoseIndex == 0 ? initialPose : poses.at(playingPoseIndex - 1).JointAnglesDeciDegree;
        auto target = poses.at(playingPoseIndex).JointAnglesDeciDegree;
        auto interval = poses.at(playingPoseIndex).Interval;
        setTargetWithInterpolation(current, target, interval);

        if (playingStep == interval - 1) {
            playingStep = 0;
            ++playingPoseIndex;
        } else {
            ++playingStep;
        }
    }
    void setTargetWithInterpolation(
        const std::vector<int16_t> &current,
        const std::vector<int16_t> &target,
        int interval) {
        auto angleLength = target.size();
        auto rate = (double)(playingStep + 1) / (double)interval;
        std::cout << "next: ";
        for (auto i = 0; i < angleLength; ++i) {
            auto c = current[i];
            auto t = target[i];
            int16_t next = c + (t - c) * rate;
            jointRepository.setTargetJointAngle(i, JointAngle(next));
            std::cout << next << " : " << c << " : " << t << " : " << rate << ", ";
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