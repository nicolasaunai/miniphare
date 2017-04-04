#ifndef TIME_H
#define TIME_H

#include <vector>
#include <numeric>

#include "types.h"

class Time
{
private:

    double dt_;
    double startTime_;
    double endTime_;
    double duration_;
    double currentTime_;
    uint32 iteration_;
    uint32 nbrIterations_;
    std::vector<double> times_;


public:
    Time(double timeStep, double startTime, double duration)
        : dt_{timeStep},
          startTime_{startTime},
          endTime_{startTime + duration},
          duration_{duration},
          currentTime_{startTime},
          nbrIterations_{}
    {
        nbrIterations_ = static_cast<uint32>(static_cast<std::size_t>(duration/dt_) + 1);
    }

    Time(Time&& source) = default;
    Time(Time& source) = default;


    uint32 time2iter(double time) const {return static_cast<uint32>((time-startTime_)/dt_); }
    uint32 currentIteration() const {return iteration_;}
    uint32 nbrIter() const {return nbrIterations_;}
    double timeStep() const {return dt_;}
    double currentTime() const {return currentTime_;}
    void advance() {currentTime_ += dt_; iteration_++;}


};

#endif // TIME_H
