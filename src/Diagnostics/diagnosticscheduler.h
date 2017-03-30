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

    //std::unordered_map< uint32, std::vector<uint32> > computingIterations_;
    //std::unordered_map< uint32, std::vector<uint32> > writingIterations_;

    DiagUnorderedMap<DiagType, std::vector<uint32> > computingIterations_;
    DiagUnorderedMap<DiagType, std::vector<uint32> > writingIterations_;

public:

    DiagnosticScheduler() = default;

    void registerDiagnostic(DiagType type,
                            std::vector<uint32> const& computingIterations,
                            std::vector<uint32> const& writingIterations);

    inline bool timeToWrite(Time const& timeManager, DiagType type)
    {
        return writingIterations_[type][timeManager.currentIteration()];
    }


    inline bool timeToCompute(Time const& timeManager, DiagType type)
    {
        return computingIterations_[type][timeManager.currentIteration()];
    }

};




#endif // DIAGNOSTICSCHEDULER_H
