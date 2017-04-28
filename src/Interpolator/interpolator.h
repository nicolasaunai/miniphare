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
    std::vector<uint32> xIndexes_;
    std::vector<uint32> yIndexes_;
    std::vector<uint32> zIndexes_;

    std::vector<double> xWeights_;
    std::vector<double> yWeights_;
    std::vector<double> zWeights_;

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
                                               QtyCentering const & centering)
    {
        // we might interpolate a field from
        // a primal or a dual mesh
        if(centering == QtyCentering::dual) // TODO this if is probably slowing us down
        {
            reducedCoord += dualOffset_;
        }

        impl_->computeIndexes(reducedCoord, xIndexes_);
        impl_->computeWeights(reducedCoord, xIndexes_, xWeights_);

        double particleField = 0;

        for(uint32 ik=0 ; ik<xIndexes_.size() ; ++ik)
        {
            particleField += meshField(xIndexes_[ik]) * xWeights_[ik];
        }
        return particleField;
    }





    inline double interpolateFieldOnto2DPoint_(double Xreduced, double Yreduced,
                                               Field const& meshField,
                                               QtyCentering const& Xcentering,
                                               QtyCentering const& Ycentering)
    {
        // we might interpolate a field from a primal or a dual mesh
        if(Xcentering == QtyCentering::dual) Xreduced += dualOffset_;
        if(Ycentering == QtyCentering::dual) Yreduced += dualOffset_;

        impl_->computeIndexes(Xreduced,xIndexes_);
        impl_->computeIndexes(Yreduced,yIndexes_);
        impl_->computeWeights(Xreduced,xIndexes_, xWeights_);
        impl_->computeWeights(Yreduced,yIndexes_, yWeights_);

        double fieldAtParticle = 0. ;
        for(uint32 ix=0 ; ix < xIndexes_.size() ; ++ix)
        {
            double Yinterp = 0. ;
            for(uint32 iy=0 ; iy < yIndexes_.size() ; ++iy)
            {
                Yinterp += meshField(xIndexes_[ix], yIndexes_[iy]) * yWeights_[iy];
            }
            fieldAtParticle += Yinterp * xWeights_[ix] ;
        }

        return fieldAtParticle ;
    }






    inline double interpolateFieldOnto3DPoint_(double Xreduced,
                                               double Yreduced,
                                               double Zreduced,
                                               Field const& meshField,
                                               QtyCentering const & Xcentering,
                                               QtyCentering const & Ycentering,
                                               QtyCentering const & Zcentering )
    {
        // we might interpolate a field from
        // a primal or a dual mesh
        if(Xcentering == QtyCentering::dual) Xreduced += dualOffset_;
        if(Ycentering == QtyCentering::dual) Yreduced += dualOffset_;
        if(Zcentering == QtyCentering::dual) Zreduced += dualOffset_;

        impl_->computeIndexes(Xreduced,xIndexes_);
        impl_->computeIndexes(Yreduced,yIndexes_);
        impl_->computeIndexes(Zreduced,zIndexes_);
        impl_->computeWeights(Xreduced,xIndexes_, xWeights_);
        impl_->computeWeights(Yreduced,yIndexes_, yWeights_);
        impl_->computeWeights(Zreduced,zIndexes_, zWeights_);

        double fieldAtParticle = 0. ;
        for(uint32 ix=0 ; ix<xIndexes_.size() ; ++ix)
        {
            double Yinterp = 0. ;
            for(uint32 iy=0 ; iy<yIndexes_.size() ; ++iy)
            {
                double Zinterp = 0. ;
                for(uint32 iz=0 ; iz<zIndexes_.size() ; ++iz)
                {
                    Zinterp += meshField(xIndexes_[ix], yIndexes_[iy], zIndexes_[iz]) * zWeights_[iz];
                }
                Yinterp += Zinterp*yWeights_[iy];
            }
            fieldAtParticle += Yinterp * xWeights_[ix];
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
                             QtyCentering centering)
    {
        return interpolateFieldOnto1DPoint_(reducedCoord, meshField, centering) ;
    }



    /**
     * @brief operator () this 1D overload is used to interpolate
     * 'meshField' onto 'particle'
     *
     * @param centering 1st direction centering
     *
     * @return
     */
    inline double operator()(Particle const& particle, Field const& meshField,
                             QtyCentering Xcentering)
    {
        uint32 dirX = static_cast<uint32>(Direction::X) ;

        double Xreduced = particle.icell[dirX] + static_cast<double>(particle.delta[dirX]) ;

        return interpolateFieldOnto1DPoint_(Xreduced, meshField, Xcentering) ;
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
                             QtyCentering Ycentering)
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
                             QtyCentering Zcentering)
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
                           Direction direction)
    {
        uint32 idir = static_cast<uint32>(direction) ;
        double reducedCoord = particle.icell[idir] + static_cast<double>(particle.delta[idir]);
        double weightOncellVolume = particle.weight*cellVolumeInverse;

        double partRho = weightOncellVolume * particle.charge;
        double partVx  = weightOncellVolume * particle.v[0] ;
        double partVy  = weightOncellVolume * particle.v[1] ;
        double partVz  = weightOncellVolume * particle.v[2] ;

        impl_->computeIndexes(reducedCoord, xIndexes_);
        impl_->computeWeights(reducedCoord, xIndexes_, xWeights_);

        for(uint32 ik=0 ; ik<xIndexes_.size() ; ++ik)
        {
            rho(   xIndexes_[ik] ) += partRho * xWeights_[ik] ;
            xFlux( xIndexes_[ik] ) += partVx  * xWeights_[ik] ;
            yFlux( xIndexes_[ik] ) += partVy  * xWeights_[ik] ;
            zFlux( xIndexes_[ik] ) += partVz  * xWeights_[ik] ;
        }
    }

};




#endif // INTERPOLATOR_H
