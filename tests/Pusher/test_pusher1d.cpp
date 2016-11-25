
#include <string>
#include <array>
#include <iostream>
#include <fstream>
#include <cmath>

#include "test_pusher.h"

#include "Plasmas/particles.h"

#include "pusher/pusher.h"
#include "pusher/pusher1d.h"
#include "pusher/pushertype.h"
#include "pusher/modifiedboris.h"
#include "pusher/pusherfactory.h"




uint32 PusherParams::testCaseNbr = 0 ;



::testing::AssertionResult AreVectorsEqual(
        const std::vector<double> & expected_vector,
        const std::vector<double> & actual_vector  ,
        double precision  )
{
    ::testing::AssertionResult failure = ::testing::AssertionFailure();

    uint32 errorNbr = 0 ;

    if( expected_vector.size() != actual_vector.size() )
    {
        ::testing::AssertionResult sizeFailure = ::testing::AssertionFailure();

        sizeFailure << "expected_vector.size() != actual_vector.size()" ;

        return sizeFailure ;
    }

    for( uint32 ik=0 ; ik<actual_vector.size() ; ++ik)
    {
        if( fabs(expected_vector[ik] - actual_vector[ik]) > precision )
        {
            failure << "\n" ;
            failure << "expected[" << ik << "] = " << expected_vector[ik] ;
            failure << "    " ;
            failure << "actual  [" << ik << "] = " << actual_vector[ik] ;
            failure << "\n" ;
            ++errorNbr ;
        }
    }


    if( errorNbr > 0 )
    {
        // Error Summary
        failure << "\nTotal number of differences = " << errorNbr << "\n" ;

        return failure ;
    }

    return testing::AssertionSuccess() ;
}


void print(PusherParams const& inputs);

void printTable( std::vector<double> const & table, const std::string & name );


void readFieldsAtParticle(PusherParams const & inputs,
                          std::vector<double> & Ex_p,
                          std::vector<double> & Ey_p,
                          std::vector<double> & Ez_p,
                          std::vector<double> & Bx_p,
                          std::vector<double> & By_p,
                          std::vector<double> & Bz_p ) ;


class PhaseSpaceTest: public ::testing::TestWithParam<PusherParams>
{
public:
    PusherParams inputs;

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


    ~PhaseSpaceTest() = default ;

    void SetUp()
    {
        inputs = GetParam();
        print(inputs) ;

        GridLayout layout{ inputs.dxdydz, inputs.nbrCells, inputs.nbDim, inputs.lattice, inputs.interpOrder  };


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

        readFieldsAtParticle( inputs,
                              Ex_p, Ey_p, Ez_p,
                              Bx_p, By_p, Bz_p ) ;


        std::unique_ptr<Pusher> pusher = PusherFactory::createPusher( layout, "modifiedBoris" ) ;

        // Initialize the Particle to be pushed
        double weight = 1. ;
        double mass = 1. ;

        uint32 icellx0 = static_cast<uint32>( std::floor(inputs.x0/layout.dx()) ) ;

        Particle testParticle( weight, inputs.q,
                 { {icellx0, 0, 0} },
                 { {static_cast<float>( inputs.x0 - icellx0*layout.dx() ), 0., 0.} },
                 { {inputs.vx0, inputs.vy0, inputs.vz0} }   );

        double posx = testParticle.icell[0]*layout.dx()
                + static_cast<double>(testParticle.delta[0]) ;

        actual_x_part.push_back( posx ) ;
        actual_vx_part.push_back( testParticle.v[0] ) ;
        actual_vy_part.push_back( testParticle.v[1] ) ;
        actual_vz_part.push_back( testParticle.v[2] ) ;

        // we compute the time step
        double dt = (inputs.tend - inputs.tbegin)/inputs.nstep ;

        precision_x = 6.*dt*dt ;
        precision_v = 2.45*dt ; // sqrt(6.) = 2.45

        for(uint32 ik=1 ; ik< inputs.nstep+1 ; ++ik)
        {
            Point E_part(Ex_p[ik], Ey_p[ik], Ez_p[ik]) ;
            Point B_part(Bx_p[ik], By_p[ik], Bz_p[ik]) ;

            pusher->move( testParticle,
                          dt, mass, inputs.q,
                          E_part, B_part) ;

            double posx = testParticle.icell[0]*layout.dx()
                 + static_cast<double>(testParticle.delta[0]) ;

            actual_x_part.push_back( posx ) ;
            actual_vx_part.push_back( testParticle.v[0] ) ;
            actual_vy_part.push_back( testParticle.v[1] ) ;
            actual_vz_part.push_back( testParticle.v[2] ) ;
        }


        std::cout << "dt = " << dt << std::endl ;
        std::cout << "precision_x = " << precision_x << std::endl ;
        std::cout << "precision_v = " << precision_v << std::endl ;

        printTable( actual_x_part, "actual_x_part" ) ;
        printTable( expected_x_part, "expected_x_part" ) ;
    }

};



void print(PusherParams const& inputs)
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


void printTable( std::vector<double> const & table, const std::string & name )
{
    std::cout << "- " << name << " -" << std::endl ;
    for( uint32 ik=0 ; ik<table.size() ; ++ik)
    {
        std::cout << " [" << ik << "] = " << table[ik] << "," ;
    }
    std::cout << std::endl ;
}


void readFieldsAtParticle(PusherParams const & inputs,
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


/***********************************************************/
/* */
/*                                                         */
/***********************************************************/
TEST_P(PhaseSpaceTest, xVxyzCompo)
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


INSTANTIATE_TEST_CASE_P(Pusher1DTest, PhaseSpaceTest,
                        testing::ValuesIn( getPusherParamsFromFile() ) );



