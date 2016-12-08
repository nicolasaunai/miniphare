#ifndef PARTICLEINITIALIZER_H
#define PARTICLEINITIALIZER_H


#include <array>

#include "Plasmas/particles.h"
//#include "particleinitializerimpl.h"
#include "grid/gridlayout.h"




enum class ParticleInitializerType {Fluid, Kinetic, count};




/**
 * @brief ParticleInitializer is an interface class used by Species to
 *        load their particles into their internal particle array without
 *        knowing how it is done. Loading particles is typically done at
 *        initialization of Species, either at the start of a simulation
 *        or when a new Patch is created by the MLMD system.
 */
class ParticleInitializer
{

private:
    //std::unique_ptr< ParticleInitializerImpl > implPtr_;

    //virtual ParticleInitializer* cloneImpl() const = 0;

public:



    // TODO deal with copy/move Ctors
    //ParticleInitializer() = default;

    //ParticleInitializer(GridLayout const& layout/*, ParticleInitializerType type*/);
    //ParticleInitializer(ParticleInitializer const& source);

    //std::unique_ptr<ParticleInitializer> clone() const ;

    virtual void loadParticles(std::vector<Particle>& particles) const = 0;

    virtual ~ParticleInitializer() = 0;


};



/* ----------------------------------------------------------------------------
 *
 *                                DOCUMENTATION
 *
   --------------------------------------------------------------------------- */


/**
  \fn virtual void ParticleInitializer::loadParticles(std::vector<Particle>& particles) const = 0
  \brief load particles into a Particle array following a method implemented in concrete classes
  \param particles is the vector to be filled. Assumed of size 0
  */



/**
  \fn virtual std::unique_ptr<ParticleInitializer> ParticleInitializer::clone() const = 0
  \brief clone a ParticleInitializer object. Needs to be defined by concrete classes

  What is a clone method?
  http://www.cplusplus.com/forum/articles/18757/
  */



#endif // PARTICLEINITIALIZER_H






