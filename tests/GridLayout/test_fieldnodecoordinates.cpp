
#include <string>
#include <array>
#include <iostream>
#include <fstream>
#include <limits>

#include "test_gridlayout.h"


MATCHER_P(DoubleNear, epsilon, "Precision out of range")
{
    // we get the actual value
    double actual = std::get<0>(arg) ;

    // we get the expected value
    double expected = std::get<1>(arg) ;

    return actual > expected-epsilon && actual < expected+epsilon ;
}


class GridLayoutFieldCoordsTest: public ::testing::TestWithParam<GridLayoutParams>
{
public:
    GridLayoutParams inputs ;

    // 2.e-7 is greater than
    // std::numeric_limits<float>::epsilon()
    const double dbl_epsilon = std::numeric_limits<float>::epsilon() ;

    std::vector<double>  actual_fieldCoordsx ;
    std::vector<double>  expected_fieldCoordsx ;


    void mySetUp( const std::string & testField )
    {
        inputs = GetParam() ;
        print(inputs) ;

        GridLayout gl{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  } ;

        // Here the Field sizes for allocations are overestimated
        AllocSizeT allocSize{2*inputs.nbrCells[0],2*inputs.nbrCells[1],2*inputs.nbrCells[2]};

        // inputs.qty is the important parameter
        Field field{allocSize , inputs.qty, "testField" };

        uint32 iy = gl.physicalStartIndex( QtyCentering::primal, Direction::Y ) ;
        uint32 iz = gl.physicalEndIndex( QtyCentering::primal, Direction::Z ) ;

        // End of input reading in the adequate file
        uint32 order = inputs.interpOrder ;
        uint32 icase = inputs.icase ;
        uint32 dim = inputs.nbDim ;

        std::string qtyName = inputs.qtyName;

        if( qtyName == testField )
        {
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
            for (uint32 ik=iStart ; ik<=iEnd ; ++ik)
            {
                ifs2 >> inputs.fieldXCoords[ik] ;

                expected_fieldCoordsx.push_back( inputs.fieldXCoords[ik] ) ;
            }


            for( uint32 ix= iStart ; ix<= iEnd ; ix++ )
            {
                Point fieldNode = gl.fieldNodeCoordinates(field, inputs.origin, ix, iy, iz) ;

                actual_fieldCoordsx.push_back( fieldNode.x_ ) ;
            }
        }
    }


    void print(GridLayoutParams const& inputs)
    {
        std::cout << " interpOrder : " << inputs.interpOrder
                  << " nbDims   : " << inputs.nbDim
                  << " (uint32) Hybridqty  : " << inputs.iqty << "\n"
                  << " dbl_epsilon : " << dbl_epsilon << "\n"
                  << " nbrCells : " << inputs.nbrCells[0] << ", " \
                  << inputs.nbrCells[1] << ", " << inputs.nbrCells[2] << "\n"
                  << " dxdydz   : " << inputs.dxdydz[0] << ", " \
                  << inputs.dxdydz[1] << ", " << inputs.dxdydz[2] << "\n"
                  << " field iStart : " << inputs.field_iStart << "\t"
                  << " field iEnd : " << inputs.field_iEnd << "\n"
                  << " Origin, x=" << inputs.origin.x_ << ", y=" << inputs.origin.y_
                  << ", z=" << inputs.origin.z_
                  << std::endl  ;
    }

};


/*-----------------------------------------------------
 *        X coordinates for fields:
 *        - Bx, By, Bz
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutFieldCoordsTest, XCoordsBx)
{
    this->mySetUp("Bx") ;

    // This test is only for Bx field
    if( inputs.qtyName == "Bx" )
    {
        EXPECT_THAT( actual_fieldCoordsx, \
                     ::testing::Pointwise(DoubleNear(dbl_epsilon), expected_fieldCoordsx) ) ;
    }
}



TEST_P(GridLayoutFieldCoordsTest, XCoordsBy)
{
    this->mySetUp("By") ;

    // This test is only for By field
    if( inputs.qtyName == "By" )
    {
        EXPECT_THAT( actual_fieldCoordsx, \
                     ::testing::Pointwise(DoubleNear(dbl_epsilon), expected_fieldCoordsx) ) ;
    }
}



TEST_P(GridLayoutFieldCoordsTest, XCoordsBz)
{
    this->mySetUp("Bz") ;

    // This test is only for Bz field
    if( inputs.qtyName == "Bz" )
    {
        EXPECT_THAT( actual_fieldCoordsx, \
                     ::testing::Pointwise(DoubleNear(dbl_epsilon), expected_fieldCoordsx) ) ;
    }
}


/*-----------------------------------------------------
 *        X coordinates for fields:
 *        - Ex, Ey, Ez
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutFieldCoordsTest, XCoordsEx)
{
    this->mySetUp("Ex") ;

    // This test is only for Ex field
    if( inputs.qtyName == "Ex" )
    {
        EXPECT_THAT( actual_fieldCoordsx, \
                     ::testing::Pointwise(DoubleNear(dbl_epsilon), expected_fieldCoordsx) ) ;
    }
}


TEST_P(GridLayoutFieldCoordsTest, XCoordsEy)
{
    this->mySetUp("Ey") ;

    // This test is only for Ey field
    if( inputs.qtyName == "Ey" )
    {
        EXPECT_THAT( actual_fieldCoordsx, \
                     ::testing::Pointwise(DoubleNear(dbl_epsilon), expected_fieldCoordsx) ) ;
    }
}


TEST_P(GridLayoutFieldCoordsTest, XCoordsEz)
{
    this->mySetUp("Ez") ;

    // This test is only for Ez field
    if( inputs.qtyName == "Ez" )
    {
        EXPECT_THAT( actual_fieldCoordsx, \
                     ::testing::Pointwise(DoubleNear(dbl_epsilon), expected_fieldCoordsx) ) ;
    }

}


/*-----------------------------------------------------
 *        X coordinates for fields:
 *        - rho, V, P
 *
 *-----------------------------------------------------*/

TEST_P(GridLayoutFieldCoordsTest, XCoordsrho)
{
    this->mySetUp("rho") ;

    // This test is only for rho field
    if( inputs.qtyName == "rho" )
    {
        EXPECT_THAT( actual_fieldCoordsx, \
                     ::testing::Pointwise(DoubleNear(dbl_epsilon), expected_fieldCoordsx) ) ;
    }

}


TEST_P(GridLayoutFieldCoordsTest, XCoordsV)
{
    this->mySetUp("V") ;

    // This test is only for V field
    if( inputs.qtyName == "V" )
    {
        EXPECT_THAT( actual_fieldCoordsx, \
                     ::testing::Pointwise(DoubleNear(dbl_epsilon), expected_fieldCoordsx) ) ;
    }

}


TEST_P(GridLayoutFieldCoordsTest, XCoordsP)
{
    this->mySetUp("P") ;

    // This test is only for P field
    if( inputs.qtyName == "P" )
    {
        EXPECT_THAT( actual_fieldCoordsx, \
                     ::testing::Pointwise(DoubleNear(dbl_epsilon), expected_fieldCoordsx) ) ;
    }

}


INSTANTIATE_TEST_CASE_P(GridLayoutTest, GridLayoutFieldCoordsTest,
                        testing::ValuesIn( getFieldCoordsInputsFromFile() ) );



