#ifndef PERIODICFIELDBC_H
#define PERIODICFIELDBC_H


#include "BoundaryConditions/fieldbc.h"



class PeriodicFieldBC : public FieldBC
{
private:

    void makeFieldPeriodic_(VecField& vecField) ;
    void makeFieldPeriodic1D_(VecField& vecField) ;
    void makeFieldPeriodic2D_(VecField& vecField) ;
    void makeFieldPeriodic3D_(VecField& vecField) ;

public:

    PeriodicFieldBC( std::string const & condition,
                     GridLayout const & layout,
                     Edge const &  edge  )
        : FieldBC(condition, layout, edge) {}

    virtual ~PeriodicFieldBC() = default ;

    virtual void applyElectricBC( VecField & E ) override ;

    virtual void applyMagneticBC( VecField & B ) override ;

    // TODO: this is not a FieldBC method, move to Class MomentBC !
//    virtual void applyMomentsBC( VecField & moment ) override ;

};




#endif // PERIODICFIELDBC_H
