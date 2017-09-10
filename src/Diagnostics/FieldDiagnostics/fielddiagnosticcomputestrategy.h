#ifndef FIELDDIAGNOSTICCOMPUTESTRATEGY_H
#define FIELDDIAGNOSTICCOMPUTESTRATEGY_H

#include "AMR/patch.h"
#include "Field/field.h"
#include "fieldpack.h"
#include "grid/gridlayout.h"

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
    std::string stratName_;

    void fillDiagData1D_(Field const& field, GridLayout const& layout, FieldPack& pack);

    void fillDiagData2D_(Field const& field, GridLayout const& layout, FieldPack& pack);

    void fillDiagData3D_(Field const& field, GridLayout const& layout, FieldPack& pack);


    void fillPack_(FieldPack& pack, Field const& field, GridLayout const& layout);


public:
    FieldDiagnosticComputeStrategy(std::string name)
        : stratName_{name}
    {
    }

    /**
     * @brief compute is overriden by a concrete FieldDiagnosticCompute class
     * to extract from a Patch the relevant field and give it to fillPack_.
     * Fields can either be extracted from patch or built upon existing fields.
     * This routine is called by the FieldDiagnostic::compute() method in a loop
     * over the whole Hierarchy, and thus will be called once per patch.
     */
    FieldPack virtual compute(Patch const& patch) = 0;

    std::string const& name() const { return stratName_; }
};


#endif // FIELDDIAGNOSTICCOMPUTESTRATEGY_H
