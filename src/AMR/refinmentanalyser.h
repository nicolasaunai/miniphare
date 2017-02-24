#ifndef REFINMENTANALYSER_H
#define REFINMENTANALYSER_H

#include "vecfield/vecfield.h"

#include "utilityphare.h"




class RefinementAnalyser
{
private:
    // it tells where the mesh should be refined
    Box refinedArea_ ;

public:

    RefinementAnalyser()
        : refinedArea_{ 0, 10, 0, 10, 0, 10} {}

    bool operator()(VecField const& vecfield) const
    {
        bool result = false ;

        // analyze vecfield


        // if needed, build the refine box


        return result ;
    }

    Box refinedArea() { return refinedArea_; }
};



#endif // REFINMENTANALYSER_H
