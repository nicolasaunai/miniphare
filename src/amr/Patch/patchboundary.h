#ifndef PATCHBOUNDARY_H
#define PATCHBOUNDARY_H

#include <iostream>


#include "data/Electromag/electromag.h"
#include "data/Electromag/electromaginitializer.h"
#include "data/Plasmas/ions.h"
#include "data/Plasmas/ionsinitializer.h"
#include "data/vecfield/vecfield.h"

#include "core/Ampere/ampere.h"
#include "core/BoundaryConditions/boundary.h"
#include "core/BoundaryConditions/boundary_conditions.h"
#include "core/pusher/pusher.h"

#include "utilities/box.h"
#include "utilities/particleselector.h"




class PatchBoundary : public Boundary
{
private:
    GridLayout layout_;
    GridLayout extendedLayout_;

    Ions ions_;

    Electromag EMfields_;

    // Jtot is computed when necessary
    // at any time substep of a refined patch
    Ampere ampere_;
    mutable VecField Jtot_;

    Edge edge_;

    // corrected EM fields from the parent patch
    // at tn + dt(L0)
    Electromag correctedEMfields_;

    double freeEvolutionTime_;
    double dtParent_;


    void addPRAChargeDensityToPatch1D_(GridLayout const& patchLayout, Field& rhoPatch,
                                       Field const& rhoPRA, Edge const& edge) const;
    void addPRAChargeDensityToPatch2D_(GridLayout const& patchLayout, Field& rhoPatch,
                                       Field const& rhoPRA, Edge const& edge) const;
    void addPRAChargeDensityToPatch3D_(GridLayout const& patchLayout, Field& rhoPatch,
                                       Field const& rhoPRA, Edge const& edge) const;

    void addPRAFluxesToPatch1D_(GridLayout const& patchLayout, Ions& ionsPatch, Ions const& ionsPRA,
                                Edge const& edge) const;
    void addPRAFluxesToPatch2D_(GridLayout const& patchLayout, Ions& ionsPatch, Ions const& ionsPRA,
                                Edge const& edge) const;
    void addPRAFluxesToPatch3D_(GridLayout const& patchLayout, Ions& ionsPatch, Ions const& ionsPRA,
                                Edge const& edge) const;

    void getPRAandPatchStartIndexes_(GridLayout const& patchLayout, Field const& fieldPatch,
                                     Field const& fieldPRA, Edge const& edge,
                                     Direction const& direction, uint32& nbrNodes,
                                     uint32& iStartPatch, uint32& iStartPRA) const;

    Edge findBoundaryEdge_(Box const& praBox, Box const& patchBox, uint32 nbDims) const;
    Edge findBoundaryEdge1D_(Box const& praBox, Box const& patchBox) const;

    void applyPRAfieldsToPatch1D_(GridLayout const& patchLayout, VecField& EMfieldPatch,
                                  VecField const& EMfieldPRA, Edge const& edge) const;
    void applyPRAfieldsToPatch2D_(GridLayout const& patchLayout, VecField& EMfieldPatch,
                                  VecField const& EMfieldPRA, Edge const& edge) const;
    void applyPRAfieldsToPatch3D_(GridLayout const& patchLayout, VecField& EMfieldPatch,
                                  VecField const& EMfieldPRA, Edge const& edge) const;

    void getPRAIndexesOverlappingPatchGhostNodes(GridLayout const& patchLayout,
                                                 Field const& fieldPatch, Field const& fieldPRA,
                                                 Edge const& edge, Direction const& direction,
                                                 uint32& nbrNodes, uint32& iStartPatch,
                                                 uint32& iStartPRA) const;

    void interpolateElectricFieldInTime_(VecField& E_interp) const;
    void interpolateMagneticFieldInTime_(VecField& B_interp) const;

public:
    PatchBoundary(GridLayout const& layout, GridLayout const& extendedLayout,
                  std::unique_ptr<IonsInitializer> ionsInit,
                  std::unique_ptr<ElectromagInitializer> electromagInit, Edge const& edge,
                  double dtParent)
        : layout_{layout}
        , extendedLayout_{extendedLayout}
        , ions_{layout, std::move(ionsInit)}
        , EMfields_{std::move(electromagInit)}
        , ampere_{layout}
        , Jtot_{layout.allocSize(HybridQuantity::Ex),
                layout.allocSize(HybridQuantity::Ey),
                layout.allocSize(HybridQuantity::Ez),
                {{HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez}},
                "Jtot"}
        , edge_{edge}
        , freeEvolutionTime_{0.}
        , dtParent_{dtParent}
    {
        correctedEMfields_ = EMfields_;
        ampere_(EMfields_.getB(), Jtot_);

        std::cout << "creating patch boundary\n";
    }

    virtual ~PatchBoundary() = default;

    virtual void applyElectricBC(VecField& E, GridLayout const& layout) const override;
    virtual void applyMagneticBC(VecField& B, GridLayout const& layout) const override;
    virtual void applyCurrentBC(VecField& J, GridLayout const& layout) const override;
    virtual void applyDensityBC(Field& J, GridLayout const& layout) const override;
    virtual void applyFluxBC(Ions& ions, GridLayout const& layout) const override;

    virtual void applyOutgoingParticleBC(std::vector<Particle>& particleArray,
                                         LeavingParticles const& leavingParticles) const override;

    virtual void applyIncomingParticleBC(BoundaryCondition& temporaryBC, Pusher& pusher,
                                         GridLayout const& patchLayout,
                                         std::vector<Particle>& particleArray,
                                         std::string const& species) override;

    GridLayout const& layout() { return layout_; }
    GridLayout const& layout() const { return layout_; }

    GridLayout const& extendedLayout() { return extendedLayout_; }
    GridLayout const& extendedLayout() const { return extendedLayout_; }

    void initPRAParticles();

    void computePRADensityAndFlux(uint32 orders);
    void computePRAChargeDensity();

    void updateCorrectedEMfields(GridLayout const& parentLayout,
                                 Electromag const& parentElectromag);
    void updateEMfields();

    void resetFreeEvolutionTime();
    void updateFreeEvolutionTime(double dt);
};


#endif // PATCHBOUNDARY_H
