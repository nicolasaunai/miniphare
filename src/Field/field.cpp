
#include "field.h"

using namespace std;
using namespace blitz;


// --------------------------------------------
// Cronstructors for Field1D
// --------------------------------------------

// with no input argument
Field::Field()
{

}

// with the name of the field
Field::Field( const string name )
{

    (*this).name = name ;

    cout << "++constructeur de Field( const string name )  " << endl ;

}


// --------------------------------------------
// Destructor for Field1D
// --------------------------------------------
Field::~Field()
{


}


// ------------------------------------------
// Other methods
// ------------------------------------------
string Field::get_name() const
{

    return( name ) ;
}

// ----------------------------------------------
// friend Overloaded operator << for Field
// ----------------------------------------------
ostream & operator << (ostream & output, Field f1)
{

    output << "Field name : " << f1.get_name() << endl ;

    return output ;
}



