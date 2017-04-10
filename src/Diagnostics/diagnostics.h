#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <vector>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include "types.h"
#include "Time/time.h"
#include "Field/field.h"
#include "AMR/hierarchy.h"
#include "utilityphare.h"
#include "grid/gridlayout.h"
#include "vecfield/vecfield.h"
#include "Plasmas/particles.h"
#include "Diagnostics/diagtype.h"
#include "Electromag/electromag.h"



// that's the standard Diagnostic interface
// all diagnostics must respect this
// this interface is used by the DiagnosticManager
class Diagnostic
{
protected:
    std::string name_;
    uint32 id_;

public:
    Diagnostic(uint32 id, std::string name)
        :name_{name}, id_{id} {}

    std::string const& name() const {return name_;}
    uint32 id() const {return id_;}

    virtual void compute(Hierarchy const& hierarchy) = 0;

    virtual ~Diagnostic() = default;

};















#endif // DIAGNOSTICS_H
