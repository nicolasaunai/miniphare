#ifndef FIELD_H
#define FIELD_H

#include <cmath>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

#include <blitz/array.h>

using namespace std ;
using namespace blitz ;



//! Class Field: generic class allowing to define vectors
class Field
{

private:

    string name ;


public:
    //! Constructor for Field
    Field() ;

    //! Constructor for Field
    Field( const string name ) ;

    //! Destructor for Field
    virtual ~Field() ;

    //!
    string get_name() const ;

    //! friend overloaded << operator for class Field
    friend ostream & operator << (ostream &, Field) ;
};

#endif
