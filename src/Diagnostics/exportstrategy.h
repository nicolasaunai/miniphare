#ifndef EXPORTSTRATEGY_H
#define EXPORTSTRATEGY_H


#include "Time/time.h"
#include "Diagnostics/diagnostics.h"




// interface used to write data on disk
// it is used by the DiagnosticManager, which does
// not know which concrete strategy is used to write data on disk
// all concrete ExportStrategy will implement the save() method
// which will take, for a diagnostic 'diag' a ref to a standard DiagData
// structure and know how to write it in a concrete file format
// this is implemented as a bridge pattern
class ExportStrategy
{
private:

public:
    virtual void save(Diagnostic const& diag, Time const& timeManager) = 0;

};




// a concrete ExportStrategy is to write Ascii files
// here we will write one file per time step and per diagnostic
// this is the simplest and dumbest we can do
// any smarter ascii format will just be a pain in the ass to read
// this is mainly for 1D debug stuff... not for real usage
class AsciiExportStrategy : public ExportStrategy
{
public:


    virtual void save(Diagnostic const& diag, Time const& timeManager) final
    {
        std::string const& name = diag.name();
        double time = timeManager.currentTime();
       // std::vector<DiagData> const& data = diag.data();


        // open file

        // now write data
       // fprintf(fp, "", di);

        // close file

    }
};




// A concrete ExportStrategy that writes Diagnostic objects
// into an HDF5 file
// the save() routine here will take a Diagnostic and a Time object
// from that it will be able to create a group, dataset etc. compliant to
// the specific format we've chosen.
class HDF5ExportStrategy : public ExportStrategy
{

};


enum class ExportStrategyType {ASCII, HDF5NATIVE, OPENPMD};


class ExportStrategyFactory
{
public:
    static std::unique_ptr<ExportStrategy> makeExportStrategy(ExportStrategyType type)
    {
        if (type == ExportStrategyType::ASCII)
        {
            return std::unique_ptr<ExportStrategy> {new AsciiExportStrategy{} };
        }

        return nullptr;
    }
};



#endif // EXPORTSTRATEGY_H
