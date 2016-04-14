#include <iostream>

#include <blitz/array.h>

#include "Field/field.h"
#include "Field/field1D.h"

using namespace std;
using namespace blitz;



int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;


//    Array<float,2> A(3,3), B(3,3), C(3,3);

//    A = 1, 0, 0,
//        2, 2, 2,
//        1, 0, 0;

//    B = 0, 0, 7,
//        0, 8, 0,
//        9, 9, 9;

//    C = A + B;

//    cout << "A = " << A << endl
//         << "B = " << B << endl
//         << "C = " << C << endl;

//    Array<int,2> Am8(8,8);
//    Am8 = 0;

//    Array<int,2> Bm3 = Am8(Range(1,7,3), Range(1,5,2));
//    Bm3 = 1;

//    Array<int,2> Cm3(Bm3*2.0) ;

//    cout << "Am8 = " << Am8 << endl;
//    cout << "Cm3 = " << Cm3 << endl;

    Field myField("undef_field") ;
    cout << myField ;


    Field1D ex_1(10,20) ;
//    cout << ex_1 ;

    Field1D ex_2("ex_2",10,20) ;
//    cout << ex_2 ;
























    return 0;
}
