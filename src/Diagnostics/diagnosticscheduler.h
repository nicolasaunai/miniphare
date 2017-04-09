#ifndef DIAGNOSTICSCHEDULER_H
#define DIAGNOSTICSCHEDULER_H


#include <unordered_map>

#include "diagtype.h"
#include "Time/time.h"



// trick seen here
// http://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
// to use an enum class as a key in unordered_map
struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};


template <typename Key>
using HashType = typename std::conditional<std::is_enum<Key>::value, EnumClassHash, std::hash<Key>>::type;

template <typename Key, typename T>
using DiagUnorderedMap = std::unordered_map<Key, T, HashType<Key>>;



// this object is in charge of saying TRUE or FALSE if a particular
// diagnostic is to be computed or written.
// each of the diagnostics in the DiagnosticManager will have to
// register its compute and dump iteration numbers to the scheduler
// these vectors will at some point come from the initialization factory
// this object is manipulated by a DiagnosticManager
class DiagnosticScheduler
{
private:

    DiagUnorderedMap<uint32, std::vector<uint32> > computingIterations_;
    DiagUnorderedMap<uint32, std::vector<uint32> > writingIterations_;
    DiagUnorderedMap<uint32, uint32> nextComputingIterationIndex_;
    DiagUnorderedMap<uint32, uint32> nextWritingIterationIndex_;

public:

    DiagnosticScheduler() = default;

    void registerDiagnostic(uint32 id,
                            std::vector<uint32> const& computingIterations,
                            std::vector<uint32> const& writingIterations);

    inline bool isTimeToWrite(Time const& timeManager, uint32 diagID)
    {
        uint32 it = timeManager.currentIteration();
        bool ret = false;
        uint32 index = nextWritingIterationIndex_[diagID];

        // if index == size it means we've done all the compute already
        if (index != writingIterations_[diagID].size()+1)
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
        uint32 it = timeManager.currentIteration();
        bool ret = false;
        uint32 index = nextComputingIterationIndex_[diagID];

        // if index is == to the size it means we've done
        // all the write already
        if (index != writingIterations_[diagID].size()+1)
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
