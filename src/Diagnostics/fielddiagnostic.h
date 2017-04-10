#ifndef FIELDDIAGNOSTIC_H
#define FIELDDIAGNOSTIC_H

#include <vector>
#include <array>

#include "Field/field.h"
#include "grid/gridlayout.h"
#include "vecfield/vecfield.h"
#include "Field/field.h"
#include "utilityphare.h"
#include "Diagnostics/diagnostics.h"


/**
 * @brief The FieldPack struct constains data and metadata for a field to be saved
 */
struct FieldPack
{
    std::vector<float> data;
    std::array<uint32, 3> nbrNodes;
    std::array<QtyCentering,3> centerings;
    uint32 nbrDimensions;
    Point origin;
};





/**
 * @brief The FieldDiagnosticComputeStrategy class is a base class used
 * by FieldDiagnostic::compute() to fill a single FieldPack with field data.
 * Given a field and a layout in read access, it knows how to fill the FieldPack.
 * This base class has a pure virtual method compute() that has to be overriden
 * to chose the right field in the Patch to give to fillPack_().
 */
class FieldDiagnosticComputeStrategy
{

protected:

    void fillDiagData1D_(Field const& field,
                         GridLayout const& layout,
                         FieldPack& pack);

    void fillDiagData2D_(Field const& field,
                         GridLayout const& layout,
                         FieldPack& pack);

    void fillDiagData3D_(Field const& field,
                         GridLayout const& layout,
                         FieldPack& pack);


    void fillPack_(FieldPack& pack, Field const& field, GridLayout const& layout);


public:

    /**
     * @brief compute is overriden by a concrete FieldDiagnosticCompute class
     * to extract from a Patch the relevant field and give it to fillPack_.
     * Fields can either be extracted from patch or built upon existing fields.
     * This routine is called by the FieldDiagnostic::compute() method in a loop
     * over the whole Hierarchy, and thus will be called once per patch.
     */
    FieldPack virtual compute(Patch const& patch) = 0;
};







/**
 * @brief The FieldDiagnostic class is the interface used by the ExportStrategy
 * to get data that is going to be written on disk. It encapsulates a container
 * of FieldPack and an abstract strategy that will compute (or just get) a field
 * and put it in a FieldPack. The FieldDiagnostic::compute() method is in charge
 * of looping over the patch Hierarchy and give each of the Patches to a concrete
 * compute strategy.
 */
class FieldDiagnostic : public Diagnostic
{

protected:

    std::vector<FieldPack> packs_;  // one pack per patch
    std::unique_ptr<FieldDiagnosticComputeStrategy> strat_;


    FieldDiagnostic(uint32 id, std::string diagName, std::unique_ptr<FieldDiagnosticComputeStrategy> strat)
        : Diagnostic{id, diagName},
          strat_{std::move(strat)}
    {}


 public:

    // routines used to access the diagnostic data per patch by export strat.
    LogicalBox getLogicalBox(FieldPack const & pack) const;
    Box getPhysicalBox(FieldPack const &pack) const;
    std::array<uint32,3> nbrNodes(FieldPack const& pack) const;
    std::vector<float> data(FieldPack const& data) const;
    uint32 nbrDimensions(FieldPack const& pack) const;


    virtual void compute(Hierarchy const& hierarchy) final;


};



#endif // FIELDDIAGNOSTIC_H
