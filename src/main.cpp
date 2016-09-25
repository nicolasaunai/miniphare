#include <iostream>
#include <array>
#include <string>

#include "AMR/patch.h"
#include "AMR/patchdata.h"
#include "AMR/hierarchy.h"
#include "grid/gridlayout.h"
#include "AMR/mlmd.h"
#include "types.h"

#include "Initializer/initializerfactory.h"



int main(int argc, char *argv[])
{


    std::cout << "Welcome to MINIPHARE" << std::endl;

    std::unique_ptr<InitializerFactory> initFactory = fromCommandLine(argc, argv);

    MLMD mlmdManager{ std::move(initFactory) };

    mlmdManager.initializeRootLevel();

}






    // initialize patch from input params somehow

    // patch building
    // Ion, etc. attach to patch via patchdata
    //patch.set...(Ions)

    // patch() {Ions...}


    // main loop





    // patch.advance() --> data_.solver_.solvestep(EMFields, Ions)



    // diag()

    //







// pseudo code


// we assume here that all patches at t=0 have been initialized properly
// i.e. each patch has all its particles and fields everywhere needed.


// go through each level of the hierarchy
// for each level, go through each patch



// first thing : get data from parent patch:
// this is done *** AT THE FIRST SUBSTEP ONLY ***

//  - tell him my coordinates and ask for its particles in my PRA
//  - split particles
//  - delete bad particles (not in influence zone)
//  - deposit child particles in influence zone in PRA moment buffers
//  - add incoming child particles to current patch

//  - get field Boundary condition from parent patch
//  - calculate time interpolation of BC for all subsequent substeps and store that



// NO NEED TO EXCHANGE WITH PARENT PATCH ANYMORE
// START SUBSTEP SOLVE LOOP


// (Bn,En) ---> B_(n+1,P1)
// (Bn+1, Pe,n_n, un)-->E_(n+1,P1)


// we now have PRED1 fields


// En+1/2, Bn+1/2 == (E,B)_avg1


// get field BC from patch BC buffer.


// MOVE IONS

// (xn,vn) ---- dt_patch, (E,B)_avg1 ----> (xn+1, vn+1)


// deposit non-out-of-range ions
// deposit RPA moments



// we now have Moments_(n+1,P1)


// (Bn,EavgP1) ----- dtpatch ---->  B_(n+1,P2)
// (B_(n+1,P2), Moments_(n+1,P1) ) ---> E_(n+1,P2)


// we now have PRED2 fields

// (E,B)_avg2

// get field BC from patch BC buffer (same as before)


// MOVE IONS

// (xn,vn) ---- dt_patch, (E,B)_avg1 ----> (xn+1, vn+1)

// this time we can delete leaving particles


// deposit ions  + add PRA moment buffers


// (Bn,Eavg2) ----- dtpatch ---->  B_(n+1)
// (B_(n+1,P2), Moments_(n+1,P2) ) ---> E_(n+1)


// apply BC



// AFTER all substeps
// we need to traverse the tree from leaves to top to synchronize patches.



