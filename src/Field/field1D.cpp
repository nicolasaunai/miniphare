

#include "field1D.h"

using namespace std;
using namespace blitz;


// ---------------------------------------------------------------------------------------------------------------------
// Creators for Field1D
// ---------------------------------------------------------------------------------------------------------------------

// with no input argument
Field1D::Field1D() : Field()
{

}

// with the bounds index as input arguments
Field1D::Field1D( int mx, int px ):data(Range(mx,px))
{
    cout << "++constructeur de Field1D( int mx, int px )  " << endl ;

    data(mx) = mx ;
    data(px) = px ;
    cout << (*this) << endl ;
}

// with name, and the bounds index as input arguments
Field1D::Field1D( string name, int mx, int px ):data(Range(mx,px)), Field(name)
{
    cout << "++constructeur de Field1D(  string name, int mx, int px )  " << endl ;

    data(mx) = mx ;
    data(px) = px ;
    cout << (*this) << endl ;
}

// with the dimensions and output (dump) file name as input argument
//Field1D::Field1D(vector<unsigned int> dims, string name) : Field(dims, name)
//{
//    allocateDims(dims);
//}


// ---------------------------------------------------------------------------------------------------------------------
// Destructor for Field1D
// ---------------------------------------------------------------------------------------------------------------------
Field1D::~Field1D()
{


}

// ----------------------------------------------
// friend Overloaded operator << for Field1D
// ----------------------------------------------
ostream & operator << (ostream & output, Field1D f1)
{

    output << "Field name : " << f1.get_name() << endl ;
    output << f1.data << endl ;

    return output ;
}



