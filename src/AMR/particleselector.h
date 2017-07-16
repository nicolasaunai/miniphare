#ifndef PARTICLESELECTOR_H
#define PARTICLESELECTOR_H

#include "utilityphare.h"

#include "Plasmas/particles.h"
#include "grid/gridlayoutdefs.h"

/**
 * @brief The ParticleSelector class is an interface, it mainly
 * provides an operator on a Particle.
 * This operator tests whether the particle belongs to
 * an arbitrary geometrical domain.
 *
 * The most simple and evident domain being a
 * 1D, 2D or 3D cuboid.
 *
 * In the future, other concrete implementations might be
 * required.
 *
 */
class ParticleSelector
{
public:
    ParticleSelector() {}

    virtual bool pick(Particle const& particle) const = 0;

    virtual ~ParticleSelector() = default;
};




/**
 * @brief The isInBox class is a concrete implementation of
 * ParticleSelector.
 * The operator tests whether the particle belongs a 1D,
 * 2D or 3D cuboid.
 *
 */
class isInBox : public ParticleSelector
{
private:
    Box parentBox_;
    Box newBox_;

    double dx_;
    double dy_;
    double dz_;

    uint32 nbrGhosts_;

public:
    isInBox(Box const& parentBox, Box const& newBox, std::array<double, 3> dxdydz, uint32 nbrGhosts)
        : parentBox_{parentBox}
        , newBox_{newBox}
        , dx_{dxdydz[0]}
        , dy_{dxdydz[1]}
        , dz_{dxdydz[2]}
        , nbrGhosts_{nbrGhosts}
    {
    }


    inline bool pick(Particle const& particle) const override
    {
        double posx = (static_cast<int32>(particle.icell[0]) - static_cast<int32>(nbrGhosts_)
                       + particle.delta[0])
                          * dx_
                      + parentBox_.x0;
        double posy = (static_cast<int32>(particle.icell[1]) - static_cast<int32>(nbrGhosts_)
                       + particle.delta[1])
                          * dy_
                      + parentBox_.y0;
        double posz = (static_cast<int32>(particle.icell[2]) - static_cast<int32>(nbrGhosts_)
                       + particle.delta[2])
                          * dz_
                      + parentBox_.z0;

        double xlower = newBox_.x0;
        double xupper = newBox_.x1;
        double ylower = newBox_.y0;
        double yupper = newBox_.y1;
        double zlower = newBox_.z0;
        double zupper = newBox_.z1;

        // return true if the particle is in the box
        return posx >= xlower && posx <= xupper && posy >= ylower && posy <= yupper
               && posz >= zlower && posz <= zupper;
    }


    virtual ~isInBox() {}
};




class isInAndCloseToTheBox : public ParticleSelector
{
private:
    Box parentBox_;
    Box newBox_;

    double dx_;
    double dy_;
    double dz_;

    uint32 nbrGhosts_;

    uint32 interpOrder_;

    double halfSpreadx_;
    double halfSpready_;
    double halfSpreadz_;

    void computeNearPRARegion_()
    {
        // In 1D, if the mother particle position is farther than halfSpreadx_
        // from the physical boundary of the domain
        // then no child particle will enter the physical domain
        // should be 0.25 * (interpOrder_ + 1) * dx_
        halfSpreadx_ = 0.25 * (interpOrder_ + 1) * dx_;
        halfSpready_ = 0.25 * (interpOrder_ + 1) * dy_;
        halfSpreadz_ = 0.25 * (interpOrder_ + 1) * dz_;
    }

public:
    isInAndCloseToTheBox(Box const& parentBox, Box const& newBox, std::array<double, 3> dxdydz,
                         uint32 nbrGhosts, uint32 interpOrder)
        : parentBox_{parentBox}
        , newBox_{newBox}
        , dx_{dxdydz[0]}
        , dy_{dxdydz[1]}
        , dz_{dxdydz[2]}
        , nbrGhosts_{nbrGhosts}
        , interpOrder_{interpOrder}
        , halfSpreadx_{0.}
        , halfSpready_{0.}
        , halfSpreadz_{0.}
    {
        computeNearPRARegion_();
    }


    inline bool pick(Particle const& particle) const override
    {
        double posx = (static_cast<int32>(particle.icell[0]) - static_cast<int32>(nbrGhosts_)
                       + particle.delta[0])
                          * dx_
                      + parentBox_.x0;
        double posy = (static_cast<int32>(particle.icell[1]) - static_cast<int32>(nbrGhosts_)
                       + particle.delta[1])
                          * dy_
                      + parentBox_.y0;
        double posz = (static_cast<int32>(particle.icell[2]) - static_cast<int32>(nbrGhosts_)
                       + particle.delta[2])
                          * dz_
                      + parentBox_.z0;

        double xlower = newBox_.x0 - halfSpreadx_;
        double xupper = newBox_.x1 + halfSpreadx_;
        double ylower = newBox_.y0 - halfSpready_;
        double yupper = newBox_.y1 + halfSpready_;
        double zlower = newBox_.z0 - halfSpreadz_;
        double zupper = newBox_.z1 + halfSpreadz_;

        // return true if the particle is in the box
        return posx >= xlower && posx <= xupper && posy >= ylower && posy <= yupper
               && posz >= zlower && posz <= zupper;
    }


    virtual ~isInAndCloseToTheBox() {}
};




class isNearBorder : public ParticleSelector
{
private:
    Box parentBox_;
    Box newBox_;

    double dx_;
    double dy_;
    double dz_;

    uint32 nbrGhosts_;

    uint32 interpOrder_;

    Direction direction_;

    double halfSpreadx_;
    double halfSpready_;
    double halfSpreadz_;

    // AVOID INITIALIZE split particles in ghost cells
    void computeNearPRARegion_()
    {
        // In 1D, if the mother particle position is farther than halfSpreadx_
        // from the physical boundary of the domain
        // then no child particle will enter the physical domain
        // should be 0.25 * (interpOrder_ + 1) * dx_
        halfSpreadx_ = 0.25 * (interpOrder_ + 1) * dx_;
        halfSpready_ = 0.25 * (interpOrder_ + 1) * dy_;
        halfSpreadz_ = 0.25 * (interpOrder_ + 1) * dz_;
    }

public:
    isNearBorder(Box const& parentBox, Box const& newBox, std::array<double, 3> dxdydz,
                 uint32 nbrGhosts, uint32 interpOrder, Direction direction)
        : parentBox_{parentBox}
        , newBox_{newBox}
        , dx_{dxdydz[0]}
        , dy_{dxdydz[1]}
        , dz_{dxdydz[2]}
        , nbrGhosts_{nbrGhosts}
        , interpOrder_{interpOrder}
        , direction_{direction}
        , halfSpreadx_{0.}
        , halfSpready_{0.}
        , halfSpreadz_{0.}
    {
        computeNearPRARegion_();
    }


    inline bool pick(Particle const& particle) const override
    {
        bool inBorder{true};

        double lpos = 0.;

        double lmin_out = 0.;
        double lmin_pra = 0.;
        double lmax_pra = 0.;
        double lmax_out = 0.;

        switch (direction_)
        {
            case Direction::X:
                lpos = (static_cast<int32>(particle.icell[0]) - static_cast<int32>(nbrGhosts_)
                        + particle.delta[0])
                           * dx_
                       + parentBox_.x0;

                lmin_out = newBox_.x0 - halfSpreadx_;
                lmin_pra = newBox_.x0;
                lmax_pra = newBox_.x1;
                lmax_out = newBox_.x1 + halfSpreadx_;
                break;

            case Direction::Y:
                lpos = (static_cast<int32>(particle.icell[1]) - static_cast<int32>(nbrGhosts_)
                        + particle.delta[1])
                           * dy_
                       + parentBox_.y0;

                lmin_out = newBox_.y0 - halfSpready_;
                lmin_pra = newBox_.y0;
                lmax_pra = newBox_.y1;
                lmax_out = newBox_.y1 + halfSpready_;
                break;

            case Direction::Z:
                lpos = (static_cast<int32>(particle.icell[2]) - static_cast<int32>(nbrGhosts_)
                        + particle.delta[2])
                           * dz_
                       + parentBox_.z0;

                lmin_out = newBox_.z0 - halfSpreadz_;
                lmin_pra = newBox_.z0;
                lmax_pra = newBox_.z1;
                lmax_out = newBox_.z1 + halfSpreadz_;
                break;
        }

        inBorder = (lpos >= lmin_out && lpos < lmin_pra) || (lpos > lmax_pra && lpos <= lmax_out);

        // return true if the particle belongs to the border
        return inBorder;
    }


    virtual ~isNearBorder() {}
};


#endif // PARTICLESELECTOR_H
