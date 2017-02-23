#ifndef HIERARCHY_H
#define HIERARCHY_H

#include "AMR/patch.h"
#include <vector>
#include <memory>


/**
 * @brief The Hierarchy class describes the hierarchy of Patches.
 * It ignores what's in Patches, just knows how to go from one patch to its
 * children and the other way.
 */
class Hierarchy
{

private:

    Patch root_;

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

    void addNewPatch( std::shared_ptr<Patch> parent, Box & position,
                      uint32 refinement, uint32 level ) ;

    void evolveHierarchy() ;
    void evaluateHierarchy() ;
    void updateHierarchy( uint32 refinementRatio ) ;

};

#endif // HIERARCHY_H
