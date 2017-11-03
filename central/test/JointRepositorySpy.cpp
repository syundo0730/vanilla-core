#include "JointRepositorySpy.h"
#include "JointAngle.h"
#include "Joint.h"

class JointRepositorySpyImpl : public JointRepositorySpy
{
  private:
	std::map<int, Joint> jointMap;

  public:
	std::map<int, int16_t> getJointAnglesAsDeciDegree() override
	{
		std::map<int, int16_t> jointAngles;
		for (auto kv : jointMap) {
			auto id = kv.first;
			auto joint = kv.second;
			jointAngles[id] = joint.getTargetAngle().getAsDeciDegree();
		}
		return jointAngles;
	}

	void setCurrentJointAngle(int id, JointAngle angle) override
	{
		jointMap[id].setCurrentAngle(angle);
	}
	void setTargetJointAngle(int id, JointAngle angle) override
	{
		jointMap[id].setTargetAngle(angle);
	}
	JointAngle getCurrentJointAngle(int id) override
	{
		return jointMap[id].getCurrentAngle();
	}
	JointAngle getTargetJointAngle(int id) override
	{
		return jointMap[id].getTargetAngle();
	}
	void applyTargetAngle(bool forceApply) override
	{
		for (const auto &kv : jointMap)
		{
			auto jointID = kv.first;
			auto joint = kv.second;
			auto target = joint.getTargetAngle().getAsDeciDegree();
			setCurrentJointAngle(jointID, target);
		}
	}
	void enable() override
	{
		applyTargetAngle(true);
	}
	void free() override
	{
	}
};

std::unique_ptr<JointRepositorySpy> JointRepositorySpy::instantiate()
{
	return std::make_unique<JointRepositorySpyImpl>();
}