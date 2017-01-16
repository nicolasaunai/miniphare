#ifndef PUSHERFACTORY_H
#define PUSHERFACTORY_H

#include <memory>
#include <string>

#include "grid/gridlayout.h"
#include "pusher/pusher.h"

#include "pusher/pushertype.h"
#include "pusher/modifiedboris.h"



class PusherFactory
{

public:

    static std::unique_ptr<Pusher>
    createPusher( const GridLayout & layout, const std::string & pusherType )
    {
        //uint32 nbDim = layout.nbDimensions() ;

        //std::unique_ptr<PusherType> impl  ;
        std::unique_ptr<Pusher> pusher ;


        if( pusherType == "modifiedBoris" )
        {
        //    impl = std::unique_ptr<ModifiedBoris>( new ModifiedBoris( pusherType, layout ) ) ;
            return std::unique_ptr<Pusher> ( new ModifiedBoris(layout) );
        }
        else
        {
            throw std::runtime_error("Error : PusherFactory - available pushers are :\
                                     - modifiedBoris ");
        }

    }
};



#endif // PUSHERFACTORY_H
