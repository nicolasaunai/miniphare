#ifndef TEST_GRIDLAYOUT_H
#define TEST_GRIDLAYOUT_H


#include "gtest/gtest.h"

#include "types.h"
#include "utility.h"

#include "grid/gridlayout.h"
#include "grid/gridlayoutimplfactory.h"
#include "grid/gridlayoutimplyee.h"

#include "Field/field.h"


struct GridLayoutParams;

HybridQuantity GetHybridQty(uint iqty) ;

std::string GetHybridQtyName(uint iqty) ;

std::vector<GridLayoutParams> getInputsFromFile() ;

std::vector<GridLayoutParams> getAllocInputsFromFile() ;

std::vector<GridLayoutParams> getFieldCoordsInputsFromFile() ;

std::vector<GridLayoutParams> getCenteredCoordsInputsFromFile() ;

std::vector<GridLayoutParams> getDerivInputsFromFile() ;


#define  MAX_SIZE 1000

/* ----------------------------------------------------------------------------
 *
 *                            GridLayout TEST
 * ---------------------------------------------------------------------------- */

struct GridLayoutParams
{
    uint32 interpOrder;
    uint32 nbDim;

    HybridQuantity qty;
    std::string qtyName;
    uint32 iqty ;        // integer equals to static_cast<uint32> (qty)

    std::string functionName ;

    uint32 icase ;

    std::string layoutName ;

    std::array<uint32, 3> nbrCells;
    std::array<double, 3> dxdydz;

    std::array<uint32,3> allocSizes;
    std::array<uint32,3> allocSizeDerived;

    std::array<uint32,3> PSI;
    std::array<uint32,3> PEI;
    std::array<uint32,3> GSI;
    std::array<uint32,3> GEI;

    uint32  field_iStart ;
    uint32  field_iEnd   ;
    Point origin{0., 0., 0.} ;

    std::vector<double>  fieldXCoords ;
    std::vector<double>  fieldXValues ;

    std::vector<double>  derivedFieldXCoords ;
    std::vector<double>  derivedFieldXValues ;

    std::vector<double>  cellCenteredXCoords ;

    std::string testComment;

    GridLayoutParams() = default ;

    GridLayoutParams(double dx, double dy, double dz,
                     uint32 nbrCellx, uint32 nbrCelly, uint32 nbrCellz,
                     std::string const& name,
                     uint32 dim, std::string comment):
        nbDim{dim}, layoutName{name},
        nbrCells{ {nbrCellx,nbrCelly,nbrCellz} }, dxdydz{ {dx,dy,dz} },
        testComment{comment}
    {

    }


    friend std::ostream& operator<<(std::ostream& os, GridLayoutParams const& inputs)
    {
        os << "interpOrder : " << inputs.interpOrder
           << " nbDims   : " << inputs.nbDim
           << " qtyName  : " << static_cast<int>(inputs.qty)
           << " nbrCells : " << inputs.nbrCells[0] << ", " << inputs.nbrCells[1] << ", " << inputs.nbrCells[2]
           << " dxdydz   : " << inputs.dxdydz[0] << ", " << inputs.dxdydz[1] << ", " << inputs.dxdydz[2]
           << " AllocSize x : " << inputs.allocSizes[0]
           << " AllocSize y : " << inputs.allocSizes[1]
           << " AllocSize z : " << inputs.allocSizes[2]
           << " Derived Size x : " << inputs.allocSizeDerived[0]
           << " Derived Size y : " << inputs.allocSizeDerived[1]
           << " Derived Size z : " << inputs.allocSizeDerived[2]
           << " " <<  inputs.iqty;

        os << " " << inputs.testComment;

        return os;
    }


//    friend std::ostream& operator<<(std::ostream& os, GridLayoutParams const& inputs)
//    {
//        os << "interpOrder : " << inputs.interpOrder
//           << " nbDims   : " << inputs.nbDim
//           << " qtyName  : " << static_cast<int>(inputs.qty)
//           << " nbrCells : " << inputs.nbrCells[0] << ", " << inputs.nbrCells[1] << ", " << inputs.nbrCells[2]
//           << " dxdydz   : " << inputs.dxdydz[0] << ", " << inputs.dxdydz[1] << ", " << inputs.dxdydz[2]
//           << " PSI x : " << inputs.PSI[0] << " PEI x : " << inputs.PEI[0]
//           << " GSI x : " << inputs.GSI[0] << " GEI x : " << inputs.GEI[0]
//           << " PSI y : " << inputs.PSI[1] << " PEI y : " << inputs.PEI[1]
//           << " GSI y : " << inputs.GSI[1] << " GEI y : " << inputs.GEI[1]
//           << " PSI z : " << inputs.PSI[2] << " PEI z : " << inputs.PEI[2]
//           << " GSI z : " << inputs.GSI[2] << " GEI z : " << inputs.GEI[2]
//           << " " <<  inputs.iqty;

//        os << " " << inputs.testComment;

//        return os;
//    }

};



/* ----------------------------------------------------------------------------
 *
 *                            GridLayoutFactory TEST
 *
 * SPECIFICATION:
 * -------------
 *
 *  - Implementation Factory throws an exception if unknown layout and
 *          nbDims < 1 and > 3
 *
 * ---------------------------------------------------------------------------- */


struct GridLayoutImplFactoryParams
{
    uint32 nbDims_ ;
    uint32 interpOrder_ ;
    std::string layoutName_ ;
    std::array<uint32,3> nbrCellsXYZ_ ;
    std::array<double,3> dxdydz_ ;


    GridLayoutImplFactoryParams(uint32 nbDims, uint32 interpOrder ,
                                std::string const& layoutName     ,
                                std::array<uint32,3> nbrCellsXYZ  ,
                                std::array<double,3> dxdydz       )
        : nbDims_{nbDims}, interpOrder_{interpOrder}, layoutName_{layoutName},
          nbrCellsXYZ_{nbrCellsXYZ}, dxdydz_{dxdydz}  {}


    friend std::ostream& operator<<(std::ostream& os, GridLayoutImplFactoryParams const& params)
    {
        os << "nbDims = " << params.nbDims_ << " ; implTypeName = " << params.layoutName_;
        return os;
    }
};






class GridLayoutFixture : public testing::Test
{
public:
    GridLayout* gl;
    GridLayoutFixture():gl(nullptr){}

     virtual void SetUp() override /*std::array<double,3> dxdydz, std::array<uint32,3> nbrCells,
               uint32 nbDims, std::string layoutName)*/
    {
        // dxdydz, nbrCells, nbDims, layoutName, interpOrder)
        gl = new GridLayout( {{0.1, 0, 0}}, {{15,0,0}}, 1, "yee", 1 );
    }

    virtual void TearDown() override
    {
        delete gl;
    }
};


class GridLayoutImplFactoryTest : public ::testing::TestWithParam<GridLayoutImplFactoryParams>
{

};


class GridLayoutConstructorTest: public ::testing::TestWithParam<GridLayoutParams>
{

};


class GridLayoutIndexingTest: public ::testing::TestWithParam<GridLayoutParams>
{

};


class GridLayoutAllocTest: public ::testing::TestWithParam<GridLayoutParams>
{

};


class GridLayoutFieldCoordsTest: public ::testing::TestWithParam<GridLayoutParams>
{

};


class GridLayoutCenteredCoordsTest: public ::testing::TestWithParam<GridLayoutParams>
{

};


class GridLayoutDeriv1D: public ::testing::TestWithParam<GridLayoutParams>
{

};


#endif // TEST_GRIDLAYOUT_H
