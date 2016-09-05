#include "ions.h"



 Ions::Ions(GridLayout const& layout):
     layout_{layout},
     rho_    {layout.nx(), layout.ny(), layout.nz(), "_rhoTot"},
     bulkVel_{layout.nx(), layout.ny(), layout.nz(), "_BulkVelTot"}
 {
 }



 Ions::Ions(GridLayout&& layout)
     : layout_{std::move(layout)}, // layout is DEAD, use layout_ now
       rho_    {layout_.nx(), layout_.ny(), layout_.nz(), "_rhoTot"},
       bulkVel_{layout_.nx(), layout_.ny(), layout_.nz(), "_BulkVelTot"}
{
}




 Species const& Ions::species(uint32 index) const
 {
    return speciesArray_[index];
 }


 Species& Ions::species(uint32 index)
 {
    return const_cast<Species&>(const_cast<const Ions*>(this)->species(index)) ;
 }
