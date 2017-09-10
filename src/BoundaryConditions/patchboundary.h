#ifndef PATCHBOUNDARY_H
#define PATCHBOUNDARY_H

#include <iostream>


#include "Initializer/electromaginitializer.h"
#include "Initializer/ionsinitializer.h"

#include "Plasmas/ions.h"

#include "AMR/particleselector.h"
#include "BoundaryConditions/boundary.h"
#include "BoundaryConditions/boundary_conditions.h"
#include "Electromag/electromag.h"
#include "pusher/pusher.h"
#include "vecfield/vecfield.h"

#include "box.h"




class PatchBoundary : public Boundary
{
private:
    GridLayout layout_;
    GridLayout extendedLayout_;

    Ions ions_;

    Electromag EMfields_;

    Edge edge_;


    // corrected EM fields from the parent patch
    // at tn + dt(L0)
    Electromag correctedEMfields_;


    void removeOutgoingParticles_(std::vector<Particle>& particleArray,
                                  LeavingParticles const& leavingParticles) const;

    void addPRAmomentsToPatch1D_(GridLayout const& patchLayout, Field& rhoPatch,
                                 Field const& rhoPRA, Edge const& edge) const;
    void addPRAmomentsToPatch1D_(GridLayout const& patchLayout, VecField& bulkVelPatch,
                                 VecField const& bulkVelPRA, Edge const& edge) const;

    void addPRAmomentsToPatch2D_(GridLayout const& patchLayout, Field& rhoPatch,
                                 Field const& rhoPRA, Edge const& edge) const;
    void addPRAmomentsToPatch2D_(GridLayout const& patchLayout, VecField& bulkVelPatch,
                                 VecField const& bulkVelPRA, Edge const& edge) const;

    void addPRAmomentsToPatch3D_(GridLayout const& patchLayout, Field& rhoPatch,
                                 Field const& rhoPRA, Edge const& edge) const;
    void addPRAmomentsToPatch3D_(GridLayout const& patchLayout, VecField& bulkVelPatch,
                                 VecField const& bulkVelPRA, Edge const& edge) const;

    void getPRAandPatchStartIndexes_(GridLayout const& patchLayout, Field const& fieldPatch,
                                     Field const& fieldPRA, Edge const& edge,
                                     Direction const& direction, uint32& nbrNodes,
                                     uint32& iStartPatch, uint32& iStartPRA) const;

    Edge findBoundaryEdge_(Box const& praBox, Box const& patchBox, uint32 nbDims) const;
    Edge findBoundaryEdge1D_(Box const& praBox, Box const& patchBox) const;

public:
    PatchBoundary(GridLayout const& layout, GridLayout const& extendedLayout,
                  std::unique_ptr<IonsInitializer> ionsInit,
                  std::unique_ptr<ElectromagInitializer> electromagInit, Edge const& edge)
        : layout_{layout}
        , extendedLayout_{extendedLayout}
        , ions_{layout, std::move(ionsInit)}
        , EMfields_{std::move(electromagInit)}
        , edge_{edge}
    {
        correctedEMfields_ = EMfields_;

        std::cout << "creating patch boundary\n";
    }

    virtual ~PatchBoundary() = default;

    virtual void applyElectricBC(VecField& E, GridLayout const& layout) const override;
    virtual void applyMagneticBC(VecField& B, GridLayout const& layout) const override;
    virtual void applyCurrentBC(VecField& J, GridLayout const& layout) const override;
    virtual void applyDensityBC(Field& J, GridLayout const& layout) const override;
    virtual void applyBulkBC(VecField& Vi, GridLayout const& layout) const override;
    virtual void applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                         LeavingParticles const& leavingParticles) const override;

    virtual void applyIncomingParticleBC(BoundaryCondition& temporaryBC, Pusher& pusher,
                                         GridLayout const& patchLayout,
                                         std::vector<Particle>& particleArray,
                                         uint32 iesp) override;

    GridLayout const& layout() { return layout_; }
    GridLayout const& layout() const { return layout_; }

    GridLayout const& extendedLayout() { return extendedLayout_; }
    GridLayout const& extendedLayout() const { return extendedLayout_; }

    void initPRAParticles();


    void computePRAmoments(std::vector<uint32> const& orders);
};


#endif // PATCHBOUNDARY_H
