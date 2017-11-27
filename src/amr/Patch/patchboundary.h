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


    void addGCAChargeDensityToPatch1D_(GridLayout const& patchLayout, Field& rhoPatch,
                                       Field const& rhoGCA, Edge const& edge) const;
    void addGCAChargeDensityToPatch2D_(GridLayout const& patchLayout, Field& rhoPatch,
                                       Field const& rhoGCA, Edge const& edge) const;
    void addGCAChargeDensityToPatch3D_(GridLayout const& patchLayout, Field& rhoPatch,
                                       Field const& rhoGCA, Edge const& edge) const;

    void addGCAFluxesToPatch1D_(GridLayout const& patchLayout, Ions& ionsPatch, Ions const& ionsGCA,
                                Edge const& edge) const;
    void addGCAFluxesToPatch2D_(GridLayout const& patchLayout, Ions& ionsPatch, Ions const& ionsGCA,
                                Edge const& edge) const;
    void addGCAFluxesToPatch3D_(GridLayout const& patchLayout, Ions& ionsPatch, Ions const& ionsGCA,
                                Edge const& edge) const;

    void getGCAandPatchStartIndexes_(GridLayout const& patchLayout, Field const& fieldPatch,
                                     Field const& fieldGCA, Edge const& edge,
                                     Direction const& direction, uint32& nbrNodes,
                                     uint32& iStartPatch, uint32& iStartGCA) const;

    Edge findBoundaryEdge_(Box const& gcaBox, Box const& patchBox, uint32 nbDims) const;
    Edge findBoundaryEdge1D_(Box const& gcaBox, Box const& patchBox) const;

    void applyGCAfieldsToPatch1D_(GridLayout const& patchLayout, VecField& EMfieldPatch,
                                  VecField const& EMfieldGCA, Edge const& edge) const;
    void applyGCAfieldsToPatch2D_(GridLayout const& patchLayout, VecField& EMfieldPatch,
                                  VecField const& EMfieldGCA, Edge const& edge) const;
    void applyGCAfieldsToPatch3D_(GridLayout const& patchLayout, VecField& EMfieldPatch,
                                  VecField const& EMfieldGCA, Edge const& edge) const;

    void getGCAIndexesOverlappingPatchGhostNodes(GridLayout const& patchLayout,
                                                 Field const& fieldPatch, Field const& fieldGCA,
                                                 Edge const& edge, Direction const& direction,
                                                 uint32& nbrNodes, uint32& iStartPatch,
                                                 uint32& iStartGCA) const;

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

    void initGCAParticles();

    void computeGCADensityAndFlux(uint32 orders);
    void computeGCAChargeDensity();

    void updateCorrectedEMfields(GridLayout const& parentLayout,
                                 Electromag const& parentElectromag);
    void updateEMfields();

    void resetFreeEvolutionTime();
    void updateFreeEvolutionTime(double dt);
};


#endif // PATCHBOUNDARY_H
