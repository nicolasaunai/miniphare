#ifndef DIAGNOSTICSCHEDULER_H
#define DIAGNOSTICSCHEDULER_H


#include "Time/time.h"
#include "diagtype.h"
#include "types.h"

#include <unordered_map>
#include <vector>




// this object is in charge of saying TRUE or FALSE if a particular
// diagnostic is to be computed or written.
// each of the diagnostics in the DiagnosticManager will have to
// register its compute and dump iteration numbers to the scheduler
// these vectors will at some point come from the initialization factory
// this object is manipulated by a DiagnosticManager
class DiagnosticScheduler
{
private:
    std::unordered_map<uint32, std::vector<uint32>> computingIterations_;
    std::unordered_map<uint32, std::vector<uint32>> writingIterations_;
    std::unordered_map<uint32, uint32> nextComputingIterationIndex_;
    std::unordered_map<uint32, uint32> nextWritingIterationIndex_;

public:
    DiagnosticScheduler() = default;

    void registerDiagnostic(uint32 id, std::vector<uint32> const& computingIterations,
                            std::vector<uint32> const& writingIterations);

    inline bool isTimeToWrite(Time const& timeManager, uint32 diagID)
    {
        uint32 it    = timeManager.currentIteration();
        bool ret     = false;
        uint32 index = nextWritingIterationIndex_[diagID];

        // if index == size it means we've done all the compute already
        if (index != writingIterations_[diagID].size() + 1)
        {
            if (writingIterations_[diagID][index] == it)
            {
                ret = true;
                nextWritingIterationIndex_[diagID]++;
            }
        }
        return ret;
    }


    inline bool isTimeToCompute(Time const& timeManager, uint32 diagID)
    {
        uint32 it    = timeManager.currentIteration();
        bool ret     = false;
        uint32 index = nextComputingIterationIndex_[diagID];

        // if index is == to the size it means we've done
        // all the write already
        if (index != writingIterations_[diagID].size() + 1)
        {
            if (computingIterations_[diagID][index] == it)
            {
                ret = true;
                nextComputingIterationIndex_[diagID]++;
            }
        }
        return ret;
    }
};




#endif // DIAGNOSTICSCHEDULER_H
