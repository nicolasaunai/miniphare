
#include <string>
#include <array>
#include <iostream>
#include <fstream>
#include <cmath>

#include "test_faraday.h"

#include "Faraday/faraday.h"
#include "Faraday/faradayimpl1d.h"
#include "Faraday/faradayfactory.h"




::testing::AssertionResult AreVectorOfBfieldsEqual(
                                const std::vector<VecField> & expected_vector,
                                const std::vector<VecField> & actual_vector  ,
                                uint32 nStep, double precision  )
{
    ::testing::AssertionResult failure = ::testing::AssertionFailure();

    uint32 errorNbr = 0 ;

    if( expected_vector.size() != actual_vector.size() )
        return failure ;

    for( uint32 istep = 0 ; istep < nStep-1 ; ++istep )
    {
        for( uint32 iBfield = 0 ; iBfield < actual_vector.size() ; ++iBfield )
        {
            for( uint32 iComp = VecField::VecX ; iComp <= VecField::VecZ ; ++iComp )
            {
                Field actual_field   = actual_vector[iBfield].component(iComp) ;
                Field expected_field = expected_vector[iBfield].component(iComp) ;

                for( uint32 ix = 0 ; ix < actual_field.shape()[0] ; ++ix )
                {
                    if( fabs(expected_field(ix) - actual_field(ix)) > precision )
                    {
                        errorNbr++;
                    }
                }
            }
        }
    }

    if( errorNbr > 0 )
    {
        // Prepare error display
        failure << "Total number of differences = " << errorNbr ;


        return failure ;
    }

    return testing::AssertionSuccess() ;
}


class Faraday1DTest: public ::testing::TestWithParam<FaradayParams>
{
public:
    FaradayParams inputs;

    double precision ;

    std::vector<VecField> expected_Bfield ;
    std::vector<VecField> actual_Bfield ;


    ~Faraday1DTest() = default ;

    void SetUp()
    {
        inputs = GetParam();
        print(inputs) ;

        GridLayout layout{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, "yee", inputs.interpOrder  };

        // this method has 2nd order precision
        precision = pow( layout.dx(), 2.) ;

        std::string testName = inputs.testName ;

        Faraday faraday = Faraday(inputs.dt, layout) ;

        auto allocBx = AllocSizeT(layout.allocSize(HybridQuantity::Bx).nx_, 1, 1) ;
        auto allocBy = AllocSizeT(layout.allocSize(HybridQuantity::By).nx_, 1, 1) ;
        auto allocBz = AllocSizeT(layout.allocSize(HybridQuantity::Bz).nx_, 1, 1) ;
        auto allocEx = AllocSizeT(layout.allocSize(HybridQuantity::Ex).nx_, 1, 1) ;
        auto allocEy = AllocSizeT(layout.allocSize(HybridQuantity::Ey).nx_, 1, 1) ;
        auto allocEz = AllocSizeT(layout.allocSize(HybridQuantity::Ez).nx_, 1, 1) ;

        // B at time n-1/2
        VecField Bold(allocBx, allocBy, allocBz,
           { {HybridQuantity::Bx, HybridQuantity::By, HybridQuantity::Bz} },
           "Bold" ) ;

        // B at time n+1/2
        VecField Bsol(allocBx, allocBy, allocBz,
           { {HybridQuantity::Bx, HybridQuantity::By, HybridQuantity::Bz} },
           "Bsol" ) ;

        // E at time n
        VecField Epred(allocEx, allocEy, allocEz,
           { {HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez} },
           "Epred" ) ;


        for( uint32 itime=0 ; itime<inputs.nbrTimeSteps ; ++itime )
        {

            for( uint32 ifield=0 ; ifield<inputs.nbrOfFields ; ++ifield )
            {
                std::string fieldName = inputs.fieldNames[ifield] ;

                HybridQuantity qty = GetHybridQtyFromString( fieldName ) ;
                uint32 iqty = static_cast<uint32>(qty);

                // faraday1D_test03_Bz_t10.txt
                std::string filename{"../Faraday/faraday1D_" + testName +
                            "_" + fieldName + "_t" + std::to_string(itime) + ".txt"};

                std::cout << filename << std::endl ;

                std::ifstream ifs2{filename};
                if (!ifs2 )
                {
                    std::cout << "Could not open file : " << filename << std::endl ;
                    exit(-1);
                }

                // we find out the field size
                auto allocSize = layout.allocSize(qty) ;

                Field EMfieldComponent{AllocSizeT(allocSize.nx_, 1, 1) , qty, "fieldName" };

                inputs.fieldInputs[iqty] = EMfieldComponent ;

                uint32 iStart = layout.physicalStartIndex( EMfieldComponent, Direction::X ) ;
                uint32 iEnd   = layout.physicalEndIndex  ( EMfieldComponent, Direction::X ) ;
                for(uint32 ik=iStart ; ik<=iEnd ; ++ik)
                {
                    double x ;
                    // the coordinate is not used
                    ifs2 >> x ;

                    ifs2 >> inputs.fieldInputs[iqty](ik) ;
                }
            }


            if( itime == 0)
            {
                // B at time n-1/2
                Bold.component(VecField::VecX) = inputs.fieldInputs[0] ;
                Bold.component(VecField::VecY) = inputs.fieldInputs[1] ;
                Bold.component(VecField::VecZ) = inputs.fieldInputs[2] ;
            }
            else
            {
                // B at time n+1/2
                Bsol.component(VecField::VecX) = inputs.fieldInputs[0] ;
                Bsol.component(VecField::VecY) = inputs.fieldInputs[1] ;
                Bsol.component(VecField::VecZ) = inputs.fieldInputs[2] ;

                expected_Bfield.push_back(Bsol) ;

                // B at time n-1/2
                Bold = Bsol ;
            }

            // E at time n
            Epred.component(VecField::VecX) = inputs.fieldInputs[3] ;
            Epred.component(VecField::VecY) = inputs.fieldInputs[4] ;
            Epred.component(VecField::VecZ) = inputs.fieldInputs[5] ;

            VecField Bnew(allocBx, allocBy, allocBz,
               { {HybridQuantity::Bx, HybridQuantity::By, HybridQuantity::Bz} },
               "Bnew" ) ;

            // Update B components with Maxwell Faraday's equation
            faraday(Epred, Bold, Bnew) ;

            actual_Bfield.push_back(Bnew) ;
        }

    }


    void print(FaradayParams const& inputs)
    {
        std::cout << "interpOrder : " << inputs.interpOrder
                  << " nbDims   : " << inputs.nbDim
                  << " nbr cell x = " << inputs.nbrCells[0] << ", nbr cell y = " \
                  << inputs.nbrCells[1] << ", nbr cell z = " << inputs.nbrCells[2]
                  << " dx = " << inputs.dxdydz[0] << ", dy = " \
                  << inputs.dxdydz[1] << ", dz = " << inputs.dxdydz[2]
                  << " dt = " << inputs.dt
                  << " tStart = " << inputs.tStart
                  << " tEnd = " << inputs.tEnd
                  << " testName = " << inputs.testName   ;

        std::cout << std::endl ;
    }

};


/***********************************************************/
/* Testing Bx is enough                                    */
/* Our only goal here is to test the derivative operator   */
/*                                                         */
/* The numerically computed derivative is accurate to the  */
/* 2nd order                                               */
/*                                                         */
/***********************************************************/
TEST_P(Faraday1DTest, BfieldSequence)
{

    EXPECT_TRUE( AreVectorOfBfieldsEqual(expected_Bfield, actual_Bfield, \
                                         inputs.nbrTimeSteps, precision) );

}



INSTANTIATE_TEST_CASE_P(FaradayTest, Faraday1DTest,
                        testing::ValuesIn( getFaraday1DInputsFromFile() ) );



