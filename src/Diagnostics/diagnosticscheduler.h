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

    DiagUnorderedMap<DiagType, std::vector<uint32> > computingIterations_;
    DiagUnorderedMap<DiagType, std::vector<uint32> > writingIterations_;
    DiagUnorderedMap<DiagType, uint32> nextComputingIterationIndex_;
    DiagUnorderedMap<DiagType, uint32> nextWritingIterationIndex_;

public:

    DiagnosticScheduler() = default;

    void registerDiagnostic(DiagType type,
                            std::vector<uint32> const& computingIterations,
                            std::vector<uint32> const& writingIterations);

    inline bool timeToWrite(Time const& timeManager, DiagType type)
    {
        uint32 it = timeManager.currentIteration();
        bool ret = false;
        uint32 index = nextWritingIterationIndex_[type];

        // if index == size it means we've done all the compute already
        if (index != writingIterations_[type].size()+1)
        {
            if (writingIterations_[type][index] == it)
            {
                ret = true;
                nextWritingIterationIndex_[type]++;
            }
        }
        return ret;
    }


    inline bool timeToCompute(Time const& timeManager, DiagType type)
    {
        uint32 it = timeManager.currentIteration();
        bool ret = false;
        uint32 index = nextComputingIterationIndex_[type];

        // if index is == to the size it means we've done
        // all the write already
        if (index != writingIterations_[type].size()+1)
        {
            if (computingIterations_[type][index] == it)
            {
                ret = true;
                nextComputingIterationIndex_[type]++;
            }
        }
        return ret;
    }

};




#endif // DIAGNOSTICSCHEDULER_H
