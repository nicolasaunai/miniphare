#ifndef FIELDBCFACTORY_H
#define FIELDBCFACTORY_H


#include <memory>
#include <string>

#include "grid/gridlayout.h"
#include "BoundaryConditions/fieldbc.h"
#include "BoundaryConditions/periodicfieldbc.h"



class FieldBCFactory
{

public:

    static std::unique_ptr<FieldBC>
    createFieldBC( GridLayout const & layout,
                   std::pair< Edge, std::string> edgeAndCondition )
    {
        std::unique_ptr<FieldBC> fieldBC ;

        Edge  edge = std::get<0>(edgeAndCondition) ;
        std::string condition = std::get<1>(edgeAndCondition) ;

        if( condition == "periodic" )
        {
            fieldBC = std::unique_ptr<PeriodicFieldBC>(
                        new PeriodicFieldBC( condition, layout, edge ) ) ;
        }
        else
        {
            throw std::runtime_error("Error : FieldBCFactory - available boundary conditions are :\
                                     - periodic ");
        }

        return  fieldBC ;
    }

};


#endif // FIELDBCFACTORY_H
