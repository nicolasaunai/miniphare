#include "fielddiagnostic.h"



void FieldDiagnostic::fillDiagData1D_(Field const& field, GridLayout const& layout, DiagPack& pack)
{
    // this function will add an element to the container of DiagData
    // typically here this will be the data corresponding to a patch
    uint32 iStart   = layout.physicalStartIndex(field, Direction::X);
    uint32 iEnd     = layout.physicalEndIndex(field, Direction::X);
    Point origin    = layout.origin();

    for (uint32 ix=iStart; ix <= iEnd;  ++ix )
    {
        Point pos = layout.fieldNodeCoordinates(field, origin, ix, 0, 0);
        pack.data[ field.name() ][ix] = static_cast<float>( field(ix) );
        pack.depends["x_" + field.name()][ix] = static_cast<float>(pos.x_);
    }
}





void FieldDiagnostic::fillDiagData2D_(Field const& field, GridLayout const& layout, DiagPack& pack)
{
    (void)field;
    throw std::runtime_error("Not Implemented");
}


void FieldDiagnostic::fillDiagData3D_(Field const& field, GridLayout const& layout, DiagPack& pack)
{
    (void)field;
    throw std::runtime_error("Not Implemented");
}




void FieldDiagnostic::addVecField_(VecField const& vecField,
                                   GridLayout const& layout
{
    std::array<uint32, 3> nbrNodesFx = layout.nbrPhysicalNodes(qty[0]);
    std::array<uint32, 3> nbrNodesFy = layout.nbrPhysicalNodes(qty[1]);
    std::array<uint32, 3> nbrNodesFz = layout.nbrPhysicalNodes(qty[2]);

    Field const& fx = vecField.component(VecField::VecX);
    Field const& fy = vecField.component(VecField::VecY);
    Field const& fz = vecField.component(VecField::VecZ);

    std::array<HybridQuantity,3> hybQties;

    hybQties[0]= fx.hybridQty();
    hybQties[1]= fy.hybridQty();
    hybQties[2]= fz.hybridQty();

    DiagPack pack;
    pack.depends.insert( {"x_" + fx.name(), std::vector<float>(nbrNodesFx[0])} );
    pack.depends.insert( {"x_" + fy.name(), std::vector<float>(nbrNodesFy[0])} );
    pack.depends.insert( {"x_" + fz.name(), std::vector<float>(nbrNodesFz[0])} );

    uint64 totalSizeEx = nbrNodesFx[0]*nbrNodesFx[1]*nbrNodesFx[2];
    uint64 totalSizeEy = nbrNodesFy[0]*nbrNodesFy[1]*nbrNodesFy[2];
    uint64 totalSizeEz = nbrNodesFz[0]*nbrNodesFz[1]*nbrNodesFz[2];

    pack.data.insert( {fx.name(), std::vector<float>(totalSizeEx)} );
    pack.data.insert( {fy.name(), std::vector<float>(totalSizeEy)} );
    pack.data.insert( {fz.name(), std::vector<float>(totalSizeEz)} );

    pack.nbrNodes.insert( {"n_" + fx.name(), std::array<uint32,3>{ nbrNodesFx } } );
    pack.nbrNodes.insert( {"n_" + fy.name(), std::array<uint32,3>{ nbrNodesFy } } );
    pack.nbrNodes.insert( {"n_" + fz.name(), std::array<uint32,3>{ nbrNodesFz } } );

    if (layout.nbDimensions() >= 2)
    {
        pack.depends.insert( {"y_" + fx.name(), std::vector<float>(nbrNodesFx[1])} );
        pack.depends.insert( {"y_" + fy.name(), std::vector<float>(nbrNodesFy[1])} );
        pack.depends.insert( {"y_" + fz.name(), std::vector<float>(nbrNodesFz[1])} );
    }

    if (layout.nbDimensions() == 3)
    {
        pack.depends.insert( {"z_" + fx.name(), std::vector<float>(nbrNodesFx[2])} );
        pack.depends.insert( {"z_" + fy.name(), std::vector<float>(nbrNodesFy[2])} );
        pack.depends.insert( {"z_" + fz.name(), std::vector<float>(nbrNodesFz[2])} );
    }


    switch (layout.nbDimensions() )
    {
        case 1:
        fillDiagData1D_(vecField.component(0), layout, pack);
        fillDiagData1D_(vecField.component(1), layout, pack);
        fillDiagData1D_(vecField.component(2), layout, pack);
        break;

        case 2:
        fillDiagData2D_(vecField.component(0), layout, pack);
        fillDiagData2D_(vecField.component(1), layout, pack);
        fillDiagData2D_(vecField.component(2), layout, pack);
        break;

        case 3:
        fillDiagData3D_(vecField.component(0), layout, pack);
        fillDiagData3D_(vecField.component(1), layout, pack);
        fillDiagData3D_(vecField.component(2), layout, pack);
        break;
    }


    diagPack_.push_back( std::move(pack) );

}
