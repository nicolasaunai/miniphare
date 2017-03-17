#ifndef HIERARCHY_H
#define HIERARCHY_H

#include "AMR/patch.h"
#include <vector>
#include <memory>





struct RefinementInfo
{
    std::shared_ptr<Patch> parentPatch;
    Box box;
    uint32 level;

    uint32 refinementRatio ;
    GridLayout const & baseLayout ;

    RefinementInfo( std::shared_ptr<Patch> parentInfo,
                    Box boxInfo, uint32 levelInfo,
                    uint32 refinementRatio,
                    GridLayout const & baseLayout ):
        parentPatch{parentInfo}, box{boxInfo},
        level{levelInfo}, refinementRatio{refinementRatio},
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

    std::vector< std::vector< std::shared_ptr<Patch> > > patchTable_ ;

public:

    using hierarchyType =
    std::vector< std::vector< std::shared_ptr<Patch> > > ;


    explicit Hierarchy(std::shared_ptr<Patch> root)
    {
        patchTable_.push_back(std::vector< std::shared_ptr<Patch> >{std::move(root)});
    }



    Patch& root() { return *patchTable_[0][0]; }

    hierarchyType & patchTable() { return patchTable_; }

    void evolveFieldsAndParticles() ;

    std::vector< std::vector<RefinementInfo> >
    evaluateAMRPatches( uint32 refineRatio, GridLayout const & baseLayout ) ;

    void updateHierarchy( std::vector< std::vector<RefinementInfo> > const & refinementTable ) ;

    void addNewPatch( RefinementInfo const & info ) ;

    GridLayout  buildLayout( RefinementInfo const & info ) ;

};

#endif // HIERARCHY_H
