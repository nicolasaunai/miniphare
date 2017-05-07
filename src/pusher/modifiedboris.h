#ifndef MODIFIEDBORIS_H
#define MODIFIEDBORIS_H

#include "Interpolator/interpolator.h"
#include "pusher.h"

/**
 * @brief Boris pusher used in Kunz et al. 2014 http://dx.doi.org/10.1016/j.jcp.2013.11.035
 */
class ModifiedBoris : public Pusher
{
private:
    void prePush_(std::vector<Particle> const& particleIn, std::vector<Particle>& particleOut);

    void pushVelocity_(std::vector<Particle> const& particleIn, std::vector<Particle>& particleOut,
                       double m);

    void postPush_(std::vector<Particle> const& particleIn, std::vector<Particle>& particleOut);

public:
    ModifiedBoris(GridLayout layout, std::string pusherType, double dt)
        : Pusher(std::move(layout), pusherType, dt)
    {
    }

    virtual ~ModifiedBoris() {}

    virtual void move(std::vector<Particle> const& partIn, std::vector<Particle>& partOut, double m,
                      VecField const& E, VecField const& B, Interpolator& interpolator,
                      BoundaryCondition const& boundaryCondition) override;
};



#endif // MODIFIEDBORIS_H
