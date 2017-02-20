#ifndef REFINMENTANALYSER_H
#define REFINMENTANALYSER_H

#include "vecfield/vecfield.h"


class RefinementAnalyser
{

public:
    bool operator()(VecField const& vecfield) const
    { return true ; }
};



#endif // REFINMENTANALYSER_H
