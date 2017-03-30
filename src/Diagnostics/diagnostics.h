#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <vector>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>

#include "types.h"
#include "Time/time.h"
#include "Field/field.h"
#include "utilityphare.h"
#include "amr/patchdata.h"
#include "grid/gridlayout.h"
#include "vecfield/vecfield.h"
#include "Plasmas/particles.h"
#include "Diagnostics/diagtype.h"
#include "Electromag/electromag.h"


// all data that will ever be written by PHARE
// will have to be put in there.
// this is the most general form of data we
// can use, that enable ExportStrategy to write
// any kind of concrete Diagnostic
// and therefore be implemented as a Bridge pattern

// this can take field data (depends=x, y, z; data= Ex, Ey, Ez)
// or particle data (depends = id_particle; data = x,y,z,vx,vy,vz)
// or 1 orbit data (depends=time; data=x,y,z)
// etc.
struct DiagPack
{
    std::unordered_map<std::string, std::vector<float>>  depends;
    std::unordered_map<std::string, std::vector<float>>  data;
    std::unordered_map<std::string, std::array<uint32,3>> nbrNodes;
};





// that's the standard Diagnostic interface
// all diagnostics must respect this
// this interface is used by the DiagnosticManager
class Diagnostic
{
protected:
    std::string name_;
    std::vector<DiagPack> diagPack_; // one diagData per patch
    //GridLayout layout_;

public:
    Diagnostic(std::string name)
        :name_{name}{}

    std::string const& name() const {return name_;}

    std::vector<DiagPack> const& data() const
    {
        std::cout << "getting Diganostic data" << std::endl;
        return diagPack_;
    }


    virtual void compute(PatchData const& patchData, GridLayout const& layout) = 0;



    virtual ~Diagnostic() = default;

};
















#endif // DIAGNOSTICS_H
