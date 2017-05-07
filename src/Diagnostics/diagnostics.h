#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "AMR/Hierarchy/hierarchy.h"
#include "Diagnostics/diagtype.h"
#include "Electromag/electromag.h"
#include "Field/field.h"
#include "Plasmas/particles.h"
#include "Time/time.h"
#include "grid/gridlayout.h"
#include "types.h"
#include "utilityphare.h"
#include "vecfield/vecfield.h"



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
        : name_{name}
        , id_{id}
    {
    }

    std::string const& name() const { return name_; }
    uint32 id() const { return id_; }

    virtual void compute(Hierarchy const& hierarchy) = 0;

    virtual ~Diagnostic() = default;
};




#endif // DIAGNOSTICS_H
