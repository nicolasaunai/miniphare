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

    uint32 order_ ;
    double dualOffset_ ;

    std::unique_ptr<IndexesAndWeights> impl_ ;


    /**
        * @brief interpolateFieldOntoPoint is used to interpolate
        * 'meshField' onto a point denoted by reducedCoord
        *
        *
        * @param reducedCoord
        * @param meshField
        * @param centering
        * @return
        */
       inline double interpolateFieldOnto1DPoint_(double reducedCoord, Field const& meshField,
                                                  QtyCentering const & centering) const
       {
           // we might interpolate a field from
           // a primal or a dual mesh
           if(centering == QtyCentering::dual) // TODO this if is probably slowing us down
           {
               reducedCoord += dualOffset_;
           }

           std::vector<uint32> const& indexes = impl_->computeIndexes(reducedCoord);
           std::vector<double> const& weights = impl_->computeWeights(reducedCoord);

           double particleField = 0;

           for(uint32 ik=0 ; ik<indexes.size() ; ++ik)
           {
               particleField += meshField(indexes[ik]) * weights[ik];
           }
           return particleField;
       }





       inline double interpolateFieldOnto2DPoint_(double Xreduced, double Yreduced,
                                                  Field const& meshField,
                                                  QtyCentering const& Xcentering,
                                                  QtyCentering const& Ycentering) const
       {
           // we might interpolate a field from a primal or a dual mesh
           if(Xcentering == QtyCentering::dual) Xreduced += dualOffset_;
           if(Ycentering == QtyCentering::dual) Yreduced += dualOffset_;

           std::vector<uint32> const& Xindexes = impl_->computeIndexes(Xreduced);
           std::vector<double> const& Xweights = impl_->computeWeights(Xreduced);
           std::vector<uint32> const& Yindexes = impl_->computeIndexes(Yreduced);
           std::vector<double> const& Yweights = impl_->computeWeights(Yreduced);

           double fieldAtParticle = 0. ;
           for(uint32 iy=0 ; iy < Yindexes.size() ; ++iy)
           {
               double Xinterp = 0.;
               for(uint32 ix=0 ; ix < Xindexes.size() ; ++ix)
               {
                   Xinterp += meshField(Xindexes[ix], Yindexes[iy]) * Xweights[ix];
               }
               fieldAtParticle += Xinterp * Yweights[iy] ;
           }
           return fieldAtParticle ;
       }






       inline double interpolateFieldOnto3DPoint_(double Xreduced,
                                                  double Yreduced,
                                                  double Zreduced,
                                                  Field const& meshField,
                                                  QtyCentering const & Xcentering,
                                                  QtyCentering const & Ycentering,
                                                  QtyCentering const & Zcentering ) const
       {
           // we might interpolate a field from
           // a primal or a dual mesh
           if(Xcentering == QtyCentering::dual) Xreduced += dualOffset_;
           if(Ycentering == QtyCentering::dual) Yreduced += dualOffset_;
           if(Zcentering == QtyCentering::dual) Zreduced += dualOffset_;

           std::vector<uint32> const& Xindexes = impl_->computeIndexes(Xreduced);
           std::vector<uint32> const& Yindexes = impl_->computeIndexes(Yreduced);
           std::vector<uint32> const& Zindexes = impl_->computeIndexes(Zreduced);
           std::vector<double> const& Xweights = impl_->computeWeights(Xreduced);
           std::vector<double> const& Yweights = impl_->computeWeights(Yreduced);
           std::vector<double> const& Zweights = impl_->computeWeights(Zreduced);

           double fieldAtParticle = 0. ;
           for(uint32 iz=0 ; iz<Zindexes.size() ; ++iz)
           {
               double Yinterp = 0. ;
               for(uint32 iy=0 ; iy<Yindexes.size() ; ++iy)
               {
                   double Xinterp = 0. ;
                   for(uint32 ix=0 ; ix<Xindexes.size() ; ++ix)
                   {
                       Xinterp += meshField(Xindexes[ix], Yindexes[iy], Zindexes[iz]) * Xweights[ix];
                   }
                   Yinterp += Xinterp*Yweights[iy];
               }
               fieldAtParticle += Yinterp * Zweights[iz];
           }
           return fieldAtParticle;
       }




public:

    Interpolator(uint32 order);

    uint32 order(){return order_;}



    /**
     * @brief operator () this overload is used to interpolate
     * the fields from a coarse patch into a refined patch
     *
     * @return the meshField interpolated at the particle position
     */
    inline double operator()(double reducedCoord, Field const& meshField,
                             QtyCentering centering) const
    {
        return interpolateFieldOnto1DPoint_(reducedCoord, meshField, centering) ;
    }




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

        return interpolateFieldOnto1DPoint_(reducedCoord, meshField, centering) ;
    }




    /**
     * @brief operator () this 2D overload is used to interpolate
     * 'meshField' onto 'particle'
     *
     * @param centering 1st direction centering
     * @param centering 2nd direction centering
     *
     * @return the meshField interpolated at the particle position
     */
    inline double operator()(Particle const& particle, Field const& meshField,
                             QtyCentering Xcentering,
                             QtyCentering Ycentering) const
    {
        uint32 dirX = static_cast<uint32>(Direction::X) ;
        uint32 dirY = static_cast<uint32>(Direction::Y) ;

        double Xreduced = particle.icell[dirX] + static_cast<double>(particle.delta[dirX]) ;
        double Yreduced = particle.icell[dirY] + static_cast<double>(particle.delta[dirY]) ;

        return interpolateFieldOnto2DPoint_(Xreduced, Yreduced, meshField, Xcentering, Ycentering);
    }



    /**
     * @brief operator () this 3D overload is used to interpolate
     * 'meshField' onto 'particle'
     *
     * @param particle
     * @param meshField
     * @param centering 1st direction centering
     * @param centering 2nd direction centering
     * @param centering 3rd direction centering
     *
     * @return the meshField interpolated at the particle position
     */
    inline double operator()(Particle const& particle, Field const& meshField,
                             QtyCentering Xcentering,
                             QtyCentering Ycentering,
                             QtyCentering Zcentering) const
    {
        uint32 dirX = static_cast<uint32>(Direction::X) ;
        uint32 dirY = static_cast<uint32>(Direction::Y) ;
        uint32 dirZ = static_cast<uint32>(Direction::Z) ;

        double Xreduced = particle.icell[dirX] + static_cast<double>(particle.delta[dirX]);
        double Yreduced = particle.icell[dirY] + static_cast<double>(particle.delta[dirY]);
        double Zreduced = particle.icell[dirZ] + static_cast<double>(particle.delta[dirZ]);

        return interpolateFieldOnto3DPoint_(Xreduced, Yreduced, Zreduced, meshField,
                                            Xcentering, Ycentering, Zcentering);
    }


    /**
     * @brief operator () this overload projects 'particle' onto rho and fluxes
     */
    inline void operator()(Particle const& particle, double cellVolumeInverse,
                           Field& rho, Field& xFlux, Field& yFlux, Field& zFlux,
                           Direction direction) const
    {
        uint32 idir = static_cast<uint32>(direction) ;
        double reducedCoord = particle.icell[idir] + static_cast<double>(particle.delta[idir]);
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




#endif // INTERPOLATOR_H
