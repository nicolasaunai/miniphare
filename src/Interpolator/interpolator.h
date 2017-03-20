#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <memory>

#include "types.h"
#include "Field/field.h"
#include "Plasmas/species.h"
#include "grid/gridlayout.h"
#include "Plasmas/particles.h"
#include "grid/gridlayoutdefs.h"
#include "IndexesAndWeights/indexesandweights.h"



class Interpolator
{
private:

    //uint32 order_ ;
    std::unique_ptr<IndexesAndWeights> impl_ ;

public:

    Interpolator(uint32 order);


    /**
     * @brief operator () this overload is used to interpolate 'meshField' onto 'particle'
     * @return the meshField interpolated at the particle position
     */
    inline double operator()(Particle const& particle, Field const& meshField,
                             Direction direction,
                             QtyCentering centering) const
    {
        uint32 idir = static_cast<uint32>(direction) ;
        double reducedCoord = particle.icell[idir] + static_cast<double>(particle.delta[idir]) ;

        // we might interpolate a field from
        // a primal or a dual mesh
        if(centering == QtyCentering::dual) // TODO this if is probably slowing us down
        {
            reducedCoord -= 0.5 ;
        }

        std::vector<uint32> const& indexes = impl_->computeIndexes(reducedCoord);
        std::vector<double> const& weights = impl_->computeWeights(reducedCoord);

        double particleField = 0;

        for(uint32 ik=0 ; ik<indexes.size() ; ++ik)
        {
            particleField += meshField(indexes[ik]) * weights[ik] ;
        }
        return particleField;
    }


    /**
     * @brief operator () this overload projects 'particle' onto rho and fluxes
     */
    inline void operator()(Particle const& particle, double cellVolumeInverse,
                           Field& rho, Field& xFlux, Field& yFlux, Field& zFlux,
                           Direction direction) const
    {
        uint32 idir = static_cast<uint32>(direction) ;
        double reducedCoord = particle.icell[idir] + static_cast<double>(particle.delta[idir]) ;
        double weightOncellVolume = particle.weight*cellVolumeInverse;

        double partRho = weightOncellVolume * particle.charge;
        double partVx  = weightOncellVolume * particle.v[0] ;
        double partVy  = weightOncellVolume * particle.v[1] ;
        double partVz  = weightOncellVolume * particle.v[2] ;

        std::vector<uint32> const& indexes = impl_->computeIndexes(reducedCoord);
        std::vector<double> const& weights = impl_->computeWeights(reducedCoord);

        for(uint32 ik=0 ; ik<indexes.size() ; ++ik)
        {
            rho(   indexes[ik] ) += partRho * weights[ik] ;
            xFlux( indexes[ik] ) += partVx  * weights[ik] ;
            yFlux( indexes[ik] ) += partVy  * weights[ik] ;
            zFlux( indexes[ik] ) += partVz  * weights[ik] ;
        }
    }
};



/* ----------------------------------------------------------------------------

                      Field interpolations at particles

   ---------------------------------------------------------------------------- */

void fieldsAtParticles(Interpolator const& interp,
                       VecField const& E, VecField const& B,
                       GridLayout const& layout,
                       std::vector<Particle>& particles);



/* ----------------------------------------------------------------------------

                      Interpolations from particles to moments

   ---------------------------------------------------------------------------- */

void computeChargeDensityAndFlux(Interpolator& interpolator,
                                 Species& species,
                                 GridLayout const& layout,
                                 std::vector<Particle>& particles);





#endif // INTERPOLATOR_H
