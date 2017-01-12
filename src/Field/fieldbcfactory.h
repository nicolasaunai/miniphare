#ifndef FIELDBCFACTORY_H
#define FIELDBCFACTORY_H


#include <memory>
#include <string>

#include "grid/gridlayout.h"

#include "Field/fieldbc.h"

#include "Field/fieldbctype.h"
#include "Field/periodicfieldbc.h"



class FieldBCFactory
{

public:

    static std::unique_ptr<FieldBC>
    createFieldBC( GridLayout const & layout,
                   std::string const & condition )
    {
        std::unique_ptr<FieldBCType> impl  ;
        std::unique_ptr<FieldBC> fieldBC ;

        if( condition == "periodic" )
        {
            impl = std::unique_ptr<PeriodicFieldBC>( new PeriodicFieldBC( condition, layout ) ) ;
            // update
        }
        else
        {
            throw std::runtime_error("Error : FieldBCFactory - available boundary conditions are :\
                                     - periodic ");
        }

        fieldBC = std::unique_ptr<FieldBC>( new FieldBC( std::move(impl) ) ) ;

        return  fieldBC ;
    }

};


#endif // FIELDBCFACTORY_H
