#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <vector>
#include <memory>

#include "AMR/patch.h"
#include "AMR/patchinfo.h"





struct RefinementInfo
{
    std::shared_ptr<Patch> parentPatch;
    Box refinedDomain;
    uint32 level;

    uint32 refinementRatio ;
    GridLayout const & baseLayout ;

    RefinementInfo( std::shared_ptr<Patch> parentPatch,
                    Box area, uint32 level,
                    uint32 refinementRatio,
                    GridLayout const & baseLayout ):
        parentPatch{parentPatch}, refinedDomain{area},
        level{level}, refinementRatio{refinementRatio},
        baseLayout{baseLayout} {}

};



/**
 * @brief The Hierarchy class describes the hierarchy of Patches.
 * It ignores what's in Patches, just knows how to go from one patch to its
 * children and the other way.
 */
class Hierarchy
{
private:

    std::vector< std::vector< std::shared_ptr<Patch> > > patchTable_;
    GridLayout  buildLayout_(RefinementInfo const & info);

public:

    using hierarchyType = std::vector< std::vector< std::shared_ptr<Patch> > > ;
    using RefinementInfoTable = std::vector< std::vector<RefinementInfo> >;


    explicit Hierarchy(std::shared_ptr<Patch> root)
    {
        patchTable_.push_back(std::vector< std::shared_ptr<Patch> >{std::move(root)});
    }

    Patch& root() { return *patchTable_[0][0]; }
    hierarchyType & patchTable() { return patchTable_; }


    void evolvePlasma() ;
    std::shared_ptr<Patch> addNewPatch(RefinementInfo const& refineInfo,
                                       PatchInfo const& patchInfo) ;
    void refine(std::vector< std::vector<RefinementInfo> > const& refinementTable, PatchInfo const& patchInfo);
    RefinementInfoTable evaluateRefinementNeed(uint32 refineRatio, GridLayout const& baseLayout) ;

    std::shared_ptr<Patch>
    addNewPatch( RefinementInfo const & info,
                 uint32 const & refineFactor,
                 std::vector<uint32> const & orders,
                 std::string const & pusher,
                 std::vector<std::string> const & splitMethod ) ;


};

#endif // HIERARCHY_H
