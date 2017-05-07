
#include <cmath>

#include "Interpolator/interpolator.h"
#include "Interpolator/particlemesh.h"
#include "pusher/modifiedboris.h"



/**
 * @brief ModifiedBoris::move1D
 *
 * STEP 1: pre-push coordinates
 * STEP 2: compute fields at particles coordinates
 * STEP 3: push velocities
 * STEP 4: correction push for coordinates
 *
 * @param partIn positions and velocities at time tn
 * @param partPred positions and velocities at time tpred
 * @param dt
 * @param m
 * @param E is given at tn+1/2
 * @param B is given at tn+1/2
 */
void ModifiedBoris::move(std::vector<Particle> const& partIn, std::vector<Particle>& partOut,
                         double mass, VecField const& E, VecField const& B,
                         Interpolator& interpolator, BoundaryCondition const& boundaryCondition)
{
    // must clean the leaving particles buffer before the last step
    // since newly leaving particles will be added to it.
    leavingParticles_.cleanBuffers();

    // advance partOut from partIn(n) to n+1/2
    prePush_(partIn, partOut);
    boundaryCondition.applyParticleBC(partOut, leavingParticles_);

    // get fields(n+1/2) at position partOut now at n+11/2
    fieldsAtParticles(interpolator, E, B, layout_, partOut);

    // push velocity from partIn (n) to n+1 using fields at partOut(n+1/2)
    pushVelocity_(partOut, partOut, mass);

    // must clean the leaving particles buffer before the last step
    // since newly leaving particles will be added to it.
    leavingParticles_.cleanBuffers();

    postPush_(partOut, partOut);

    boundaryCondition.applyParticleBC(partOut, leavingParticles_);
}




void ModifiedBoris::prePush_(std::vector<Particle> const& particleIn,
                             std::vector<Particle>& particleOut)
{
    std::array<double, 3> dto2dl;
    // here dy and dz might be zero (1D, 2D) so dfo2dl[1,2] might be Inf
    // but that's ok because we loop on nbdims_
    dto2dl[0] = 0.5 * dt_ / layout_.dx();
    dto2dl[1] = 0.5 * dt_ / layout_.dy();
    dto2dl[2] = 0.5 * dt_ / layout_.dz();


    for (uint32 iPart = 0; iPart < particleIn.size(); ++iPart)
    {
        Particle const& partIn = particleIn[iPart];
        Particle& partOut      = particleOut[iPart];
        partOut.weight         = partIn.weight;
        partOut.charge         = partIn.charge;
        for (uint32 iv    = 0; iv < 3; ++iv)
            partOut.v[iv] = partIn.v[iv];

        for (uint32 dim = 0; dim < nbdims_; ++dim)
        {
            // time decentering of the delta position at tn+1/2
            float delta = partIn.delta[dim] + static_cast<float>(dto2dl[dim] * partIn.v[dim]);

            // check the validity of delta (0 <= delta <= 1)
            // and do auto-correction
            float iCell        = std::floor(delta);
            partOut.delta[dim] = delta - iCell;

            // update the logical node
            partOut.icell[dim] = static_cast<uint32>(iCell) + partIn.icell[dim];

            // check if the particle is now leaving the patch
            // and if it does, store it in the leavingParticles buffers
            leavingParticles_.storeIfLeaving(partOut.icell[dim], iPart, dim);
        }
    }
}




void ModifiedBoris::pushVelocity_(std::vector<Particle> const& particleIn,
                                  std::vector<Particle>& particleOut, double m)
{
    double dto2m = 0.5 * dt_ / m;

    for (uint32 iPart = 0; iPart < particleIn.size(); ++iPart)
    {
        Particle const& partIn = particleIn[iPart];
        Particle& partOut      = particleOut[iPart];

        double coef1 = partIn.charge * dto2m;

        // We now apply the 3 steps of the BORIS PUSHER

        // 1st half push of the electric field
        double velx1 = partIn.v[0] + coef1 * partIn.Ex;
        double vely1 = partIn.v[1] + coef1 * partIn.Ey;
        double velz1 = partIn.v[2] + coef1 * partIn.Ez;


        // preparing variables for magnetic rotation
        double const rx = coef1 * partIn.Bx;
        double const ry = coef1 * partIn.By;
        double const rz = coef1 * partIn.Bz;

        double const rx2  = rx * rx;
        double const ry2  = ry * ry;
        double const rz2  = rz * rz;
        double const rxry = rx * ry;
        double const rxrz = rx * rz;
        double const ryrz = ry * rz;

        double const invDet = 1. / (1. + rx2 + ry2 + rz2);

        // preparing rotation matrix due to the magnetic field
        // m = invDet*(I + r*r - r x I) - I where x denotes the cross product
        double const mxx = 1. + rx2 - ry2 - rz2;
        double const mxy = 2. * (rxry + rz);
        double const mxz = 2. * (rxrz - ry);

        double const myx = 2. * (rxry - rz);
        double const myy = 1. + ry2 - rx2 - rz2;
        double const myz = 2. * (ryrz + rx);

        double const mzx = 2. * (rxrz + ry);
        double const mzy = 2. * (ryrz - rx);
        double const mzz = 1. + rz2 - rx2 - ry2;

        // magnetic rotation
        double const velx2 = (mxx * velx1 + mxy * vely1 + mxz * velz1) * invDet;
        double const vely2 = (myx * velx1 + myy * vely1 + myz * velz1) * invDet;
        double const velz2 = (mzx * velx1 + mzy * vely1 + mzz * velz1) * invDet;


        // 2nd half push of the electric field
        velx1 = velx2 + coef1 * partIn.Ex;
        vely1 = vely2 + coef1 * partIn.Ey;
        velz1 = velz2 + coef1 * partIn.Ez;

        // Update particle velocity
        partOut.v[0] = velx1;
        partOut.v[1] = vely1;
        partOut.v[2] = velz1;
    }
}


void ModifiedBoris::postPush_(std::vector<Particle> const& particleIn,
                              std::vector<Particle>& particleOut)
{
    std::array<double, 3> dto2dl;
    dto2dl[0] = 0.5 * dt_ / layout_.dx();
    dto2dl[1] = 0.5 * dt_ / layout_.dy();
    dto2dl[2] = 0.5 * dt_ / layout_.dz();


    for (uint32 iPart = 0; iPart < particleIn.size(); ++iPart)
    {
        Particle const& partIn = particleIn[iPart];
        Particle& partOut      = particleOut[iPart];

        for (uint32 dim = 0; dim < nbdims_; ++dim)
        {
            // we update the delta position at tn+1
            float delta = partIn.delta[dim] + static_cast<float>(dto2dl[dim] * partIn.v[dim]);

            // check the validity of delta (0 <= delta <= 1)
            // and do auto-correction
            float iCell        = std::floor(delta);
            partOut.delta[dim] = delta - iCell;

            // update the logical node
            partOut.icell[dim] += iCell;

            // check if the particle is now leaving the patch
            // and if it does, store it in the leavingParticles buffers
            leavingParticles_.storeIfLeaving(partOut.icell[dim], iPart, dim);
        }
    }
}
