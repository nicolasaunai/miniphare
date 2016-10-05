
#include <string>
#include <array>
#include <iostream>
#include <fstream>


#include "test_gridlayout.h"




class GridLayoutFieldCoordsTest: public ::testing::TestWithParam<GridLayoutParams>
{
public:
    GridLayoutParams inputs ;

//    std::vector<Point>  actual_fieldCoords ;

    std::vector<double>  actual_fieldCoordsx ;
    std::vector<double>  actual_fieldCoordsy ;
    std::vector<double>  actual_fieldCoordsz ;

    void SetUp()
    {
        inputs = GetParam() ;
        print(inputs) ;

        GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  } ;

        // Here the Field sizes for allocations are overestimated
        AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};

        // inputs.qty is the important parameter
        Field field{allocSize , inputs.qty, "testField" };

        uint32 iy = gl.indexAtMin( QtyCentering::primal, Direction::Y ) ;
        uint32 iz = gl.indexAtMin( QtyCentering::primal, Direction::Z ) ;

        // End of input reading in the adequate file
        uint32 order = inputs.interpOrder ;
        uint32 icase = inputs.icase ;
        uint32 dim = inputs.nbDim ;

        std::string qtyName = inputs.qtyName;

        std::string filename{"../GridLayout/fieldCoords_ord" + std::to_string(order) +
                    "_dim" + std::to_string(dim) + "_" + qtyName +
                    "_case" + std::to_string(icase) + ".txt"};

        std::cout << filename << std::endl ;

        std::ifstream ifs2{filename};
        if (!ifs2 )
        {
            std::cout << "Could not open file : " << filename << std::endl ;
            exit(-1);
        }

        uint32 iStart = inputs.field_iStart ;
        uint32 iEnd   = inputs.field_iEnd   ;
        for (uint32 ik=iStart ; ik<iEnd+1 ; ++ik)
        {
            ifs2 >> inputs.fieldXCoords[ik] ;
        }


        for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
        {
            Point fieldNode = gl.fieldNodeCoordinates(field, inputs.origin, ix, iy, iz) ;

            actual_fieldCoordsx.push_back( fieldNode.x_ ) ;
        }

    }


    void print(GridLayoutParams const& inputs)
    {
        std::cout << "interpOrder : " << inputs.interpOrder
                  << " nbDims   : " << inputs.nbDim
                  << " qtyName  : " << static_cast<int>(inputs.qty)
                  << " nbrCells : " << inputs.nbrCells[0] << ", " \
                  << inputs.nbrCells[1] << ", " << inputs.nbrCells[2]
                  << " dxdydz   : " << inputs.dxdydz[0] << ", " \
                  << inputs.dxdydz[1] << ", " << inputs.dxdydz[2]
                  << " " <<  inputs.iqty;
    }

};


/*-----------------------------------------------------
 *        X coordinates for fields:
 *        - Bx, By, Bz
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutFieldCoordsTest, XCoordsBx)
{
    // This test is only for Bx field
    if( inputs.qtyName == "Bx" )
    {
        // TODO :  try  ::testing::ContainerEq
        EXPECT_THAT( actual_fieldCoordsx,
                     ::testing::Pointwise(::testing::DoubleEq, inputs.fieldXCoords) ) ;
    }
}



TEST_P(GridLayoutFieldCoordsTest, XCoordsBy)
{
    // This test is only for By field
    if( inputs.qtyName == "By" )
    {
        EXPECT_THAT( actual_fieldCoordsx,
                     ::testing::Pointwise(::testing::DoubleEq, inputs.fieldXCoords) ) ;
    }
}



TEST_P(GridLayoutFieldCoordsTest, XCoordsBz)
{
    // This test is only for Bz field
    if( inputs.qtyName == "Bz" )
    {
        EXPECT_THAT( actual_fieldCoordsx,
                     ::testing::Pointwise(::testing::DoubleEq, inputs.fieldXCoords) ) ;
    }
}


/*-----------------------------------------------------
 *        X coordinates for fields:
 *        - Ex, Ey, Ez
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutFieldCoordsTest, XCoordsEx)
{
    // This test is only for Ex field
    if( inputs.qtyName == "Ex" )
    {
        EXPECT_THAT( actual_fieldCoordsx,
                     ::testing::Pointwise(::testing::DoubleEq, inputs.fieldXCoords) ) ;
    }
}


TEST_P(GridLayoutFieldCoordsTest, XCoordsEy)
{
    // This test is only for Ey field
    if( inputs.qtyName == "Ey" )
    {
        EXPECT_THAT( actual_fieldCoordsx,
                     ::testing::Pointwise(::testing::DoubleEq, inputs.fieldXCoords) ) ;
    }
}


TEST_P(GridLayoutFieldCoordsTest, XCoordsEz)
{
    // This test is only for Ez field
    if( inputs.qtyName == "Ez" )
    {
        EXPECT_THAT( actual_fieldCoordsx,
                     ::testing::Pointwise(::testing::DoubleEq, inputs.fieldXCoords) ) ;
    }

}


/*-----------------------------------------------------
 *        X coordinates for fields:
 *        - rho, V, P
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutFieldCoordsTest, XCoordsrho)
{
    // This test is only for rho field
    if( inputs.qtyName == "rho" )
    {
        EXPECT_THAT( actual_fieldCoordsx,
                     ::testing::Pointwise(::testing::DoubleEq, inputs.fieldXCoords) ) ;
    }

}


TEST_P(GridLayoutFieldCoordsTest, XCoordsV)
{
    // This test is only for V field
    if( inputs.qtyName == "V" )
    {
        EXPECT_THAT( actual_fieldCoordsx,
                     ::testing::Pointwise(::testing::DoubleEq, inputs.fieldXCoords) ) ;
    }

}


TEST_P(GridLayoutFieldCoordsTest, XCoordsP)
{
    // This test is only for P field
    if( inputs.qtyName == "P" )
    {
        EXPECT_THAT( actual_fieldCoordsx,
                     ::testing::Pointwise(::testing::DoubleEq, inputs.fieldXCoords) ) ;
    }

}


INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutFieldCoordsTest,
                        testing::ValuesIn( getFieldCoordsInputsFromFile() ) );



