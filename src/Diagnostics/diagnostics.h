#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <vector>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "types.h"
#include "Time/time.h"
#include "Field/field.h"
#include "utilityphare.h"




template <class T>
class DiagnosticUnit
{
private:
    Box domain_;
    LogicalBox logicalDomain_;

public:
    Box const& domain() const {return domain_;}
    LogicalBox const& logicalDomain() const {return logicalDomain_;}

    virtual T const& getResult() = 0;

    virtual ~DiagnosticUnit() = default;
};




// class qui retourne les tableaux où sont les champs
// ces classes sont dans PatchData
class FieldDiagUnit : public DiagnosticUnit<Field>
{
public:

    void compute();
};




class ParticleDiagUnit : public DiagnosticUnit
{

};




// the diagnostics knows all diagnostics units
// it gets data and knows how to organier oy
class Diagnostic
{
private:
    // they do the work
    std::vector< std::shared_ptr<DiagnosticUnit> > units_;
};




// interface used to write data on disk
class ExportStrategy
{
private:

public:

};


// write Ascii files
class AsciiExportStrategy : public ExportStrategy
{

};




class HDF5ExportStrategy : public ExportStrategy
{

};






/**
 * @brief encapsulate all diagnostics (fields, particle, globals, etc.)
 */
/*
cette classe est celle qui est appelée par le scheduler
elle offre une interface pour appeler chacun des diagnostiques possibles
dans le code.
*/
class DiagnosticsManager
{
private:
    std::vector< std::shared_ptr<Diagnostic> > diags_;

public:





    virtual ~DiagnosticsManager() = default;
};




#if 0
class AsciiDiagnosticsManager  : public DiagnosticsManager
{
private:
    FILE* fieldFile_;
    FILE* particleFile_;

    std::vector< std::shared_ptr<FieldDiagUnit> > fieldDiagUnits_;
    std::vector< std::shared_ptr<ParticleDiagUnit> > particleDiagUnits_;

public:

    virtual void writeFields(double time) final
    {
        std::ostringstream strs;
        strs << "fields_" << std::setfill('0')
             << std::setw(6)
             << std::setprecision(6) << time << ".txt";

        std::string filename = strs.str();

        fieldFile_ = fopen(filename.c_str(), "w");


        for (uint32 iDiag=0; iDiag < fieldDiagUnits_.size(); ++iDiag)
        {
            //Box const& region = fieldDiagUnits_[iDiag]->region();
            //LogicalBox const& indices = fieldDiagUnits_[iDiag]->indexBox();
           // Field const& E = fieldDiagUnits_[iDiag]->electricField();
        }

        fclose(fieldFile_);
    }


    virtual void writeParticles(double time) final{}
    virtual void writeOrbits(double time) final{}

    virtual ~AsciiDiagnosticsManager() = default;
};



#endif




enum class DiagType {Field, Particle};



// en charge de savoir quel diagnostic faire a un temps donné
class DiagnosticScheduler
{
private:

    std::vector<uint32> schedule_;

public:

    void registerDiagnostics(std::vector<uint32> const& iterations);

    // go in a table
    // and at time t apply all diagnostics applicable at that time
    void applyDiagnostics(Time const& timeManager)
    {
        std::cout << "Applying diagnostics at time "
                  << timeManager.currentTime() << std::endl;

        uint32 iter = timeManager.currentIteration();

    }

};



#endif // DIAGNOSTICS_H
