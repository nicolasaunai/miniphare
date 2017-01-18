#ifndef FIELDBC_H
#define FIELDBC_H

#include <memory>

#include "types.h"

#include "grid/gridlayout.h"
#include "vecfield/vecfield.h"



class FieldBC
{
protected:

    const uint32 EDGE_MIN_ = -1 ;
    const uint32 EDGE_MAX_ =  1 ;

    std::string  condition_ ;
    GridLayout  layout_ ;

    Edge edge_ ;
    Direction direction_ ;

    double dx_ ;
    uint32 nbrCellx_ ;


public:
    FieldBC( std::string const & condition, GridLayout const & layout,
             Edge const & edge  )
        : condition_{condition}, layout_{layout}, edge_{edge}
    {
        if( edge == Edge::Xmin || edge == Edge::Xmax )
        {
            direction_ = Direction::X ;
        }
        else if( edge == Edge::Ymin || edge == Edge::Ymax )
        {
            direction_ = Direction::Y ;
        }
        else
        {
            direction_ = Direction::Z ;
        }

    }

    FieldBC(FieldBC const& source) = delete;
    FieldBC& operator=(FieldBC const& source) = delete;

    FieldBC(FieldBC&& toMove)      = default;
    FieldBC& operator=(FieldBC&& source) = default;

    virtual ~FieldBC() = default ;

    virtual void applyElectricBC( VecField & E ) = 0 ;

    virtual void applyMagneticBC( VecField & B ) = 0 ;

};


#endif // FIELDBC_H
