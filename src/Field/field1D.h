#ifndef FIELD1D_H
#define FIELD1D_H


#include "field.h"


//! class Field1D used to defined a 1d vector
class Field1D : public Field
{
private:
    Array<double,1> data ;

public:
    //! Constructor for Field1D: with no input argument
    Field1D();

    //! Constructor for Field1D: with
    Field1D( int mx, int px );

    //! Constructor for Field1D: with
    Field1D( string name, int mx, int px );

    //! Destructor for Field1D: with
    ~Field1D();

    //! friend overloaded << operator for class Field1D
    friend ostream & operator << (ostream &, Field1D) ;

};


#endif
