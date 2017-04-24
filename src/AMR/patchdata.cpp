
#include "patchdata.h"
#include "hybridenums.h"



PatchData::PatchData(const InitializerFactory &initFactory)

    : EMfields_ {initFactory.createElectromagInitializer()},
      solver_{ initFactory.gridLayout(), initFactory.timeStep(),
               initFactory.createSolverInitializer() },
      ions_{ initFactory.gridLayout(), initFactory.createIonsInitializer() },
      electrons_{initFactory.gridLayout(), 0.2},
      boundaryCondition_{initFactory.createBoundaryCondition()}
      //EMInitializer_{initFactory->createElectromagInitializer()}
{

}


void PatchData::initPatchPhysicalDomain()
{
    std::cout << "Initializing PatchData..." << std::endl;
    ions_.loadParticles();
    solver_.init(ions_, *boundaryCondition_);
    //EMfields_.init(EMInitializer_.get());
    std::cout << "PatchData initialized!" << std::endl;
}


/**
 * @brief PatchData::initPRA will trigger the initialization of
 * the Particle Repopulation Area, contained in
 * boundaryCondition_ attribute
 *
 */
void PatchData::initPatchGhostDomain()
{
    // we have to trigger loadParticles() method of Ions,
    // the latter will call loadParticles() of Species
    boundaryCondition_->initializeGhostArea() ;

}


void PatchData::solveStep()
{
    solver_.solveStep(EMfields_, ions_, electrons_, *boundaryCondition_);
}


