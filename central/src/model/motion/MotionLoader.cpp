#include "MotionLoader.h"

class MotionLoaderImpl : public MotionLoader
{
  public:
    std::vector<Motion> load(std::string path) override
    {
        return YAML::LoadFile(path)["motions"].as<std::vector<Motion>>();
    }
};

std::unique_ptr<MotionLoader> MotionLoader::instantiate()
{
    return std::make_unique<MotionLoaderImpl>();
}