
#include <string>
#include <array>
#include <iostream>
#include <fstream>
#include <cmath>

#include "test_commons.h"

#include "test_interpolate1d.h"

#include "Plasmas/particles.h"

#include "Interpolator/interpolator.h"

#include "pusher/pusher.h"
#include "pusher/pusher1d.h"
#include "pusher/pushertype.h"
#include "pusher/modifiedboris.h"
#include "pusher/pusherfactory.h"




uint32 InterpPushParams::testCaseNbr = 0 ;



void print(InterpPushParams const& inputs) ;

void allocEBVecFields( GridLayout const & layout, \
                      std::shared_ptr<VecField> & E_out, \
                      std::shared_ptr<VecField> & B_out );

void initEVecField( VecField & E_out,
                    double Ex, double Ey, double Ez ) ;

void initBVecField( VecField & B_out,
                    double Bx, double By, double Bz ) ;

void readFieldsAtParticle(InterpPushParams const & inputs,
                          std::vector<double> & Ex_p,
                          std::vector<double> & Ey_p,
                          std::vector<double> & Ez_p,
                          std::vector<double> & Bx_p,
                          std::vector<double> & By_p,
                          std::vector<double> & Bz_p ) ;


class PusherTest: public ::testing::TestWithParam<InterpPushParams>
{
public:
    InterpPushParams inputs;

    double precision_x ;
    double precision_v ;

    std::vector<double> expected_x_part ;
    std::vector<double> expected_vx_part ;
    std::vector<double> expected_vy_part ;
    std::vector<double> expected_vz_part ;

    std::vector<double> actual_x_part ;
    std::vector<double> actual_vx_part ;
    std::vector<double> actual_vy_part ;
    std::vector<double> actual_vz_part ;


    ~PusherTest() = default ;

    void SetUp()
    {
        inputs = GetParam();
        print(inputs) ;
        Point origin;

        GridLayout layout{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, inputs.lattice,
                    origin, inputs.interpOrder};


        std::string file_x{"../Pusher/odepush_x_testCase"
                    + std::to_string(inputs.testId) + ".txt"};

        std::string file_vx{"../Pusher/odepush_vx_testCase"
                    + std::to_string(inputs.testId) + ".txt"};
        std::string file_vy{"../Pusher/odepush_vy_testCase"
                    + std::to_string(inputs.testId) + ".txt"};
        std::string file_vz{"../Pusher/odepush_vz_testCase"
                    + std::to_string(inputs.testId) + ".txt"};

        std::cout << file_x << std::endl ;
        std::cout << file_vx << std::endl ;
        std::cout << file_vy << std::endl ;
        std::cout << file_vz << std::endl ;

        std::ifstream ifs_x{file_x};
        if (!ifs_x )
        {
            std::cout << "Could not open file : " << file_x << std::endl ;
            exit(-1);
        }
        std::ifstream ifs_vx{file_vx};
        if (!ifs_vx )
        {
            std::cout << "Could not open file : " << file_vx << std::endl ;
            exit(-1);
        }
        std::ifstream ifs_vy{file_vy};
        if (!ifs_vy )
        {
            std::cout << "Could not open file : " << file_vy << std::endl ;
            exit(-1);
        }
        std::ifstream ifs_vz{file_vz};
        if (!ifs_vz )
        {
            std::cout << "Could not open file : " << file_vz << std::endl ;
            exit(-1);
        }

        expected_x_part.assign( inputs.nstep+1, 0. ) ;
        expected_vx_part.assign( inputs.nstep+1, 0. ) ;
        expected_vy_part.assign( inputs.nstep+1, 0. ) ;
        expected_vz_part.assign( inputs.nstep+1, 0. ) ;
        for(uint32 ik=0 ; ik< inputs.nstep+1 ; ++ik)
        {
            ifs_x >> expected_x_part[ik] ;
            ifs_vx >> expected_vx_part[ik] ;
            ifs_vy >> expected_vy_part[ik] ;
            ifs_vz >> expected_vz_part[ik] ;
        }

        std::vector<double> Ex_p( inputs.nstep+1, 0.) ;
        std::vector<double> Ey_p( inputs.nstep+1, 0.) ;
        std::vector<double> Ez_p( inputs.nstep+1, 0.) ;
        std::vector<double> Bx_p( inputs.nstep+1, 0.) ;
        std::vector<double> By_p( inputs.nstep+1, 0.) ;
        std::vector<double> Bz_p( inputs.nstep+1, 0.) ;

        // HERE: We do not use fields read from input files
        readFieldsAtParticle( inputs,
                              Ex_p, Ey_p, Ez_p,
                              Bx_p, By_p, Bz_p ) ;


        std::unique_ptr<Pusher> pusher = PusherFactory::createPusher( layout, "modifiedBoris" ) ;

        // Initialize the Particle to be pushed
        double weight = 1. ;
        double mass = 1. ;

        uint32 icellx0 = static_cast<uint32>( std::floor(inputs.x0/layout.dx()) ) ;

        Particle partic( weight, inputs.q,
                 { {icellx0, 0, 0} },
                 { {static_cast<float>( inputs.x0 - icellx0*layout.dx() ), 0., 0.} },
                 { {inputs.vx0, inputs.vy0, inputs.vz0} }   );

        double posx = ( partic.icell[0] + \
                static_cast<double>(partic.delta[0]) )*layout.dx() ;

        actual_x_part.push_back( posx ) ;

        actual_vx_part.push_back( partic.v[0] ) ;
        actual_vy_part.push_back( partic.v[1] ) ;
        actual_vz_part.push_back( partic.v[2] ) ;

        // we compute the time step
        double dt = (inputs.tend - inputs.tbegin)/inputs.nstep ;

        // We need an interpolator
        std::unique_ptr<Interpolator> interpol{ new Interpolator{layout}} ;

        std::vector<Particle> particArray{partic} ;

        precision_x = 6.*dt*dt ;
        precision_v = 2.45*dt ; // sqrt(6.) = 2.45

        for(uint32 ik=1 ; ik< inputs.nstep+1 ; ++ik)
        {
            std::shared_ptr<VecField> E_ptr = nullptr ;
            std::shared_ptr<VecField> B_ptr = nullptr ;

            allocEBVecFields( layout, E_ptr, B_ptr ) ;
            initEVecField( *E_ptr, Ex_p[ik], Ey_p[ik], Ez_p[ik] ) ;
            initBVecField( *B_ptr, Bx_p[ik], By_p[ik], Bz_p[ik] ) ;

            pusher->move( particArray, particArray,
                          dt, mass, *E_ptr, *B_ptr) ;

            Particle const & iPart = particArray[0] ;

            double posx = ( iPart.icell[0] + \
                    static_cast<double>(iPart.delta[0]) )*layout.dx() ;

            actual_x_part.push_back( posx ) ;

            actual_vx_part.push_back( iPart.v[0] ) ;
            actual_vy_part.push_back( iPart.v[1] ) ;
            actual_vz_part.push_back( iPart.v[2] ) ;
        }


        std::cout << "dt = " << dt << std::endl ;
        std::cout << "precision_x = " << precision_x << std::endl ;
        std::cout << "precision_v = " << precision_v << std::endl ;

        printTable( actual_x_part, "actual_x_part" ) ;
        printTable( expected_x_part, "expected_x_part" ) ;
    }

};




void allocEBVecFields( GridLayout const & layout, \
                      std::shared_ptr<VecField> & E_out, \
                      std::shared_ptr<VecField> & B_out )
{
    auto allocEx = layout.allocSize(HybridQuantity::Ex) ;
    auto allocEy = layout.allocSize(HybridQuantity::Ey) ;
    auto allocEz = layout.allocSize(HybridQuantity::Ez) ;
    auto allocBx = layout.allocSize(HybridQuantity::Bx) ;
    auto allocBy = layout.allocSize(HybridQuantity::By) ;
    auto allocBz = layout.allocSize(HybridQuantity::Bz) ;

    E_out = std::make_shared<VecField>( VecField(allocEx, allocEy, allocEz,\
            { {HybridQuantity::Ex, HybridQuantity::Ey, HybridQuantity::Ez} }, "E") ) ;

    B_out = std::make_shared<VecField>( VecField(allocBx, allocBy, allocBz,\
            { {HybridQuantity::Bx, HybridQuantity::By, HybridQuantity::Bz} }, "B") ) ;

}


/**
 * @brief initEVecField
 * @param E_out is set using Ex, Ey, Ez
 * @param Ex uniform value for Field E_out.x_
 * @param Ey uniform value for Field E_out.y_
 * @param Ez uniform value for Field E_out.z_
 */
void initEVecField( VecField & E_out,
                    double uniformEx, double uniformEy, double uniformEz )
{
    Field & Ex = E_out.component(VecField::VecX) ;
    Field & Ey = E_out.component(VecField::VecY) ;
    Field & Ez = E_out.component(VecField::VecZ) ;

    for( auto it=Ex.begin() ; it<Ex.end() ; ++it ) *it = uniformEx ;

    for( auto it=Ey.begin() ; it<Ey.end() ; ++it ) *it = uniformEy ;

    for( auto it=Ez.begin() ; it<Ez.end() ; ++it ) *it = uniformEz ;
}


/**
 * @brief initBVecField
 * @param B_out is set using Bx, By, Bz
 * @param Bx uniform value for Field B_out.x_
 * @param By uniform value for Field B_out.y_
 * @param Bz uniform value for Field B_out.z_
 */
void initBVecField( VecField & B_out,
                    double uniformBx, double uniformBy, double uniformBz )
{
    Field & Bx = B_out.component(VecField::VecX) ;
    Field & By = B_out.component(VecField::VecY) ;
    Field & Bz = B_out.component(VecField::VecZ) ;

    for( auto it=Bx.begin() ; it<Bx.end() ; ++it ) *it = uniformBx ;

    for( auto it=By.begin() ; it<By.end() ; ++it ) *it = uniformBy ;

    for( auto it=Bz.begin() ; it<Bz.end() ; ++it ) *it = uniformBz ;
}



void readFieldsAtParticle(InterpPushParams const & inputs,
                          std::vector<double> & Ex_p,
                          std::vector<double> & Ey_p,
                          std::vector<double> & Ez_p,
                          std::vector<double> & Bx_p,
                          std::vector<double> & By_p,
                          std::vector<double> & Bz_p )
{

    std::string fileFields{"../Pusher/odepush_fields_testCase"
                + std::to_string(inputs.testId) + ".txt"};

    std::cout << fileFields << std::endl ;

    std::ifstream ifsFields{fileFields};
    if (!ifsFields )
    {
        std::cout << "Could not open file : " << fileFields << std::endl ;
        exit(-1);
    }

    // Reading Exyz(t), Bxyz(t) at the particle position
    for(uint32 ik=0 ; ik< inputs.nstep+1 ; ++ik) ifsFields >> Ex_p[ik] ;
    for(uint32 ik=0 ; ik< inputs.nstep+1 ; ++ik) ifsFields >> Ey_p[ik] ;
    for(uint32 ik=0 ; ik< inputs.nstep+1 ; ++ik) ifsFields >> Ez_p[ik] ;
    for(uint32 ik=0 ; ik< inputs.nstep+1 ; ++ik) ifsFields >> Bx_p[ik] ;
    for(uint32 ik=0 ; ik< inputs.nstep+1 ; ++ik) ifsFields >> By_p[ik] ;
    for(uint32 ik=0 ; ik< inputs.nstep+1 ; ++ik) ifsFields >> Bz_p[ik] ;

}





void print(InterpPushParams const& inputs)
{
    std::cout << "tbegin = " << inputs.tbegin
              << " tend  = " << inputs.tend
              << " nstep = " << inputs.nstep << "\n"
              << " q = " << inputs.q << "\t"
              << " m = " << inputs.m << "\n"
              << " x0 = " << inputs.x0 << "\n"
              << " y0 = " << inputs.y0 << "\n"
              << " z0 = " << inputs.z0 << "\n"
              << " vx0 = " << inputs.vx0 << "\n"
              << " vy0 = " << inputs.vy0 << "\n"
              << " vz0 = " << inputs.vz0 << "\n"
              << " dxdydz[0] = " << inputs.dxdydz[0]
              << " dxdydz[1] = " << inputs.dxdydz[1]
              << " dxdydz[2] = " << inputs.dxdydz[2] << "\n"
              << " nbrCells[0] = " << inputs.nbrCells[0]
              << " nbrCells[1] = " << inputs.nbrCells[1]
              << " nbrCells[2] = " << inputs.nbrCells[2] << "\n"
              << " nbDim = " << inputs.nbDim
              << " lattice = " << inputs.lattice
              << " interpOrder : " << inputs.interpOrder
              << std::endl ;
}





/***********************************************************/
/* */
/*                                                         */
/***********************************************************/
TEST_P(PusherTest, xVxyzCompo)
{
    // it might look dirty to have 4 EXPECT_TRUE
    // but DO NOT change this !!
    // or you will break the tests

    // uint32 testCaseNbr is static
    // splitting in 4 TEST_P results in testCaseNbr taking
    // values greater than the actual number of test cases available

    EXPECT_TRUE( AreVectorsEqual(expected_x_part, actual_x_part, precision_x ) );
    EXPECT_TRUE( AreVectorsEqual(expected_vx_part, actual_vx_part, precision_v ) );
    EXPECT_TRUE( AreVectorsEqual(expected_vy_part, actual_vy_part, precision_v ) );
    EXPECT_TRUE( AreVectorsEqual(expected_vz_part, actual_vz_part, precision_v ) );
}


INSTANTIATE_TEST_CASE_P(Interpolate1DTest, PusherTest,
                        testing::ValuesIn( getInterpPushParamsFromFile() ) );



