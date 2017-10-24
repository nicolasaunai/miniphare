
#include "fielddiagnosticcomputestrategy.h"



/**
 * @brief FieldDiagnosticComputeStrategy::fillDiagData1D_ takes data in a field
 * excluding ghost nodes and fill a FieldPack.data field with it
 * @param field is the Field from which data is taken
 * @param layout is the GridLayout on which the Field is defined
 * @param pack is the FieldPack to be filled.
 */
void FieldDiagnosticComputeStrategy::fillDiagData1D_(Field const& field, GridLayout const& layout,
                                                     FieldPack& pack)
{
    // this function will add an element to the container of DiagData
    // typically here this will be the data corresponding to a patch
    uint32 iStart = layout.physicalStartIndex(field, Direction::X);
    uint32 iEnd   = layout.physicalEndIndex(field, Direction::X);

    for (uint32 ixField = iStart, ixData = 0; ixField <= iEnd; ++ixField, ++ixData)
    {
        pack.data[field.name()][ixData] = static_cast<float>(field(ixField));
    }
}




/**
 * @brief FieldDiagnosticComputeStrategy::fillDiagData2D_ serves the same goal
 * as fillDiagData1D_ but in 2D. The data in the FieldPack is flattened
 */
void FieldDiagnosticComputeStrategy::fillDiagData2D_(Field const& field, GridLayout const& layout,
                                                     FieldPack& pack)
{
    (void)field;
    (void)layout;
    (void)pack;
    throw std::runtime_error("Not Implemented");
}




/**
 * @brief FieldDiagnosticComputeStrategy::fillDiagData3D_ serves the same goal
 * as fillDiagData1D_ but in 3D. The data in the FieldPack is flattened
 */
void FieldDiagnosticComputeStrategy::fillDiagData3D_(Field const& field, GridLayout const& layout,
                                                     FieldPack& pack)
{
    (void)field;
    (void)layout;
    (void)pack;
    throw std::runtime_error("Not Implemented");
}




/**
 * @brief fillPack_ knows how to extract information from a field and
 * a layout to fill a FieldPack correctly.
 */
void FieldDiagnosticComputeStrategy::fillPack_(FieldPack& pack, Field const& field,
                                               GridLayout const& layout)
{
    std::string key = field.name();

    pack.keys.push_back(key);

    HybridQuantity hybQty = field.hybridQty();
    pack.nbrNodes[key]    = layout.nbrPhysicalNodes(hybQty);


    uint64 totalSize = pack.nbrNodes[key][0] * pack.nbrNodes[key][1] * pack.nbrNodes[key][2];

    pack.centerings[key][0] = layout.fieldCentering(field, Direction::X);
    pack.centerings[key][1] = layout.fieldCentering(field, Direction::Y);
    pack.centerings[key][2] = layout.fieldCentering(field, Direction::Z);

    pack.nbrDimensions[key] = layout.nbDimensions();

    pack.data.insert({field.name(), std::vector<float>(totalSize)});

    pack.origin[key] = layout.origin();

    pack.gridSpacing[key][0] = static_cast<float>(layout.dx());
    pack.gridSpacing[key][1] = static_cast<float>(layout.dy());
    pack.gridSpacing[key][2] = static_cast<float>(layout.dz());

    switch (layout.nbDimensions())
    {
        case 1: fillDiagData1D_(field, layout, pack); break;
        case 2: fillDiagData2D_(field, layout, pack); break;
        case 3: fillDiagData3D_(field, layout, pack); break;
    }
}
