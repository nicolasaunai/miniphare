#ifndef INITIALIZERFACTORY_H
#define INITIALIZERFACTORY_H


#include <memory>
#include <string>

#include "ionsinitializer.h"
#include "electromaginitializer.h"
#include "ohminitializer.h"



/**
 * @brief An InitializerFactory is used as an interface to build and initialize
 *       objects in the simulation.
 *
 *
 * The InitializerFactory follows the Abstract Factory pattern so that
 * objects can be built without explicitly depending on the way the code
 * gets its input parameters. Concrete InitializerFactory object can obtain
 * their data from python scripts (PythonInitializerFactory), or simpler system
 * such as text files (SimpleInitializerFactory), etc.
 *
 *
 * It is important to understand that a concrete InitializerFactory is not
 * only useful to initialize objects from USER inputs. Concrete InitializerFactory
 * are to be created in runtime by the MLMD manager to build children Patch data from
 * parent Patch data.
 *
 */
class InitializerFactory
{

public:

    virtual std::unique_ptr<IonsInitializer> createIonsInitializer() const = 0;
    virtual std::unique_ptr<ElectromagInitializer> createElectromagInitializer() const = 0;
    virtual std::unique_ptr<OhmInitializer> createOhmInitializer() const = 0;

    virtual std::string pusherType() const = 0;
    virtual GridLayout const& gridLayout() const = 0;
    virtual double timeStep() const = 0;

    // virtual std::unique_ptr<SolverInitializer> createSolverInitializer() = 0;
    // virtual std::unique_ptr<DiagnosticInitializer> createDiagnosticInitializer() = 0;
    // virtual std::unique_ptr<GridLayoutInitializer> createGridLayoutInitializer() = 0;

    // TODO add HiearchyInitializer and MLMDInitializer ?


    virtual ~InitializerFactory() = 0;

};






std::unique_ptr<InitializerFactory> fromCommandLine(int argc, char** argv);





/* ----------------------------------------------------------------------------
 *
 *                                DOCUMENTATION
 *
   --------------------------------------------------------------------------- */


/**
  \fn virtual std::unique_ptr<IonsInitializer> InitializerFactory::createIonsInitializer() const = 0
  \brief creates an IonInitializer object from user inputs. The IonInitializer
  will be used by Ions to build its Species.


  \fn virtual std::unique_ptr<ElectromagInitializer> InitializerFactory::createElectromagInitializer() const = 0
  \brief creates an ElectromagInitializer object, used to initialize Electromag fields



  \fn virtual std::unique_ptr<OhmInitializer> InitializerFactory::createOhmInitializer() const = 0
  \brief creates an OhmInitializer, used to initialize the Ohm equation solver.



  \fn virtual GridLayout const& InitializerFactory::gridLayout() const = 0
  \brief returns a GridLayout object.



  \fn virtual double InitializerFactory::timeStep() const = 0
  \brief returns the timestep.

  */




#endif // INITIALIZERFACTORY_H
