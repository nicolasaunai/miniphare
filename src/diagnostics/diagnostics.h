#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H


#include "amr/Hierarchy/hierarchy.h"
#include <string>



// that's the standard Diagnostic interface
// all diagnostics must respect this
// this interface is used by the DiagnosticManager
class Diagnostic
{
protected:
    std::string name_;
    std::string path_;
    uint32 id_;

public:
    Diagnostic(uint32 id, std::string name, std::string path)
        : name_{name}
        , path_{path}
        , id_{id}
    {
    }

    std::string const& name() const { return name_; }
    std::string const& path() const { return path_; }
    uint32 id() const { return id_; }

    virtual void compute(Hierarchy const& hierarchy) = 0;

    virtual ~Diagnostic() = default;
};




#endif // DIAGNOSTICS_H
