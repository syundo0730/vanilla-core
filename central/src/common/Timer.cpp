#include "Timer.h"
#include <thread>
#include <chrono>

namespace timer
{
void waitMs(int ms)
{
    auto mils = std::chrono::milliseconds(ms);
    std::this_thread::sleep_for(mils);
}
unsigned long millis()
{
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    );
    return ms.count();
}
}