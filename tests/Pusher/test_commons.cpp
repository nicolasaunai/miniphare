
#include "hybridenums.h"

#include "test_commons.h"





void printTable( std::vector<double> const & table, const std::string & name )
{
    std::cout << "- " << name << " -" << std::endl ;
    for( uint32 ik=0 ; ik<table.size() ; ++ik)
    {
        std::cout << " [" << ik << "] = " << table[ik] << "," ;
    }
    std::cout << std::endl ;
}




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




std::string GetHybridQtyName(uint32 iqty)
{
    std::string qtyName{"None"} ;

    std::array<std::string, static_cast<uint32>(HybridQuantity::count) >  hybridQtyNames ;

    hybridQtyNames[0] = "Bx"  ;
    hybridQtyNames[1] = "By"  ;
    hybridQtyNames[2] = "Bz"  ;
    hybridQtyNames[3] = "Ex"  ;
    hybridQtyNames[4] = "Ey"  ;
    hybridQtyNames[5] = "Ez"  ;
    hybridQtyNames[6] = "rho" ;
    hybridQtyNames[7] = "V"   ;
    hybridQtyNames[8] = "P"   ;

    if( iqty < static_cast<uint32>(HybridQuantity::count) )
    {
        qtyName = hybridQtyNames[iqty] ;
    }

    return qtyName ;
}


