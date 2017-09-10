#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H


#include "AMR/Hierarchy/hierarchy.h"
#include <string>



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
