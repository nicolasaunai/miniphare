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

    RefinementInfo( std::shared_ptr<Patch> parentInfo, Box boxInfo,
                    uint32 levelInfo):
        parentPatch{parentInfo}, box{boxInfo},
        level{levelInfo} {}

};


GridLayout  buildNewLayout( RefinementInfo const & info, GridLayout const & layoutL0,
                            uint32 refinement ) ;


/**
 * @brief The Hierarchy class describes the hierarchy of Patches.
 * It ignores what's in Patches, just knows how to go from one patch to its
 * children and the other way.
 */
class Hierarchy
{

private:
    Patch root_;

    std::vector< RefinementInfo > patchToBeRefined_ ;

    std::vector< std::vector< std::shared_ptr<Patch> > > patchTable_ ;

public:

    using hierarchyType =
    std::vector< std::vector< std::shared_ptr<Patch> > > ;

    explicit Hierarchy(Patch&& root):root_{std::move(root)}
    {
        std::shared_ptr<Patch> sharedRoot(&root_) ;
        std::vector< std::shared_ptr<Patch> > rootVector{ sharedRoot } ;

        patchTable_ =  hierarchyType
        {1, std::vector< std::shared_ptr<Patch> > (rootVector) };
    }


    Patch& root() { return root_; }

    hierarchyType & patchTable() { return patchTable_; }

    void addNewPatch( RefinementInfo const & info, GridLayout const & layoutL0,
                      uint32 const & refinement ) ;

    void evolveHierarchy() ;
    void evaluateHierarchy() ;
    void updateHierarchy( GridLayout const & layoutL0,
                          uint32 const & refinementRatio ) ;

};

#endif // HIERARCHY_H
