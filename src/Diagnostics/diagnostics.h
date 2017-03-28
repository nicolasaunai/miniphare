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
#include "Plasmas/particles.h"
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
struct DiagData
{
    std::vector<float> depends;
    std::vector<float> data;
};


enum class DiagType {EM, Ni};



// that's the standard Diagnostic interface
// all diagnostics must respect this
// this interface is used by the DiagnosticManager
class Diagnostic
{
protected:
    std::string name_;

public:
    Diagnostic(std::string name):name_{name} {}

    std::string const& name() const {return name_;}

    virtual void compute(PatchData const& patchData) = 0;

    virtual std::vector<DiagData> const& data() const = 0;


    virtual ~Diagnostic() = default;

};





// an example of diagnostic for the electromagnetic field
// the class implement compute() and data()
// compute() will put EM data as a DiagData
// data() return the reference to this DiagData
class ElectromagDiagnostic : public Diagnostic
{
private:
    std::vector<DiagData> data_;

public:
    ElectromagDiagnostic()
        : Diagnostic{"EM"}{}

    virtual void compute(PatchData const& patchData) final
    {
        std::cout << "computing EM diags" << std::endl;
        Electromag const& em = patchData.EMfields();

        // may need layout at some point
        // put all em data into the vector
        // format is:
        // x y z into data_[i].depends
        // Ex, Ey Ez into data_[i].data
    }

    virtual std::vector<DiagData> const& data() const final
    {
        std::cout << "getting EM data" << std::endl;
        return data_;
    }

    virtual ~ElectromagDiagnostic() = default;
};







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
        std::vector<DiagData> const& data = diag.data();


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


// trick seen here
// http://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key
// to use an enum class as a key in unordered_map
struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};


template <typename Key>
using HashType = typename std::conditional<std::is_enum<Key>::value, EnumClassHash, std::hash<Key>>::type;

template <typename Key, typename T>
using DiagUnorderedMap = std::unordered_map<Key, T, HashType<Key>>;









// this object is in charge of saying TRUE or FALSE if a particular
// diagnostic is to be computed or written.
// each of the diagnostics in the DiagnosticManager will have to
// register its compute and dump iteration numbers to the scheduler
// these vectors will at some point come from the initialization factory
// this object is manipulated by a DiagnosticManager
class DiagnosticScheduler
{
private:

    //std::unordered_map< uint32, std::vector<uint32> > computingIterations_;
    //std::unordered_map< uint32, std::vector<uint32> > writingIterations_;

    DiagUnorderedMap<DiagType, std::vector<uint32> > computingIterations_;
    DiagUnorderedMap<DiagType, std::vector<uint32> > writingIterations_;

public:

    DiagnosticScheduler() = default;

    void registerDiagnostic(DiagType type,
                            std::vector<uint32> const& computingIterations,
                            std::vector<uint32> const& writingIterations)
    {
        computingIterations_.insert({type, computingIterations}  );
        writingIterations_.insert(  {type, writingIterations} );
    }


    bool timeToWrite(Time const& timeManager, DiagType type)
    {
        return writingIterations_[type][timeManager.currentIteration()];
    }


    bool timeToCompute(Time const& timeManager, DiagType type)
    {
        return computingIterations_[type][timeManager.currentIteration()];
    }

};




class DiagnosticFactory
{
public:

    // for now no other option than the type.
    static std::unique_ptr<Diagnostic> makeDiagnostic(DiagType type)
    {
        if (type == DiagType::EM )
        {
            return std::unique_ptr<Diagnostic> {new ElectromagDiagnostic{} };
        }

        return nullptr;
    }
};






/**
 * @brief encapsulate all diagnostics (fields, particle, globals, etc.)
 */

class DiagnosticsManager
{
private:
    DiagUnorderedMap< DiagType, std::shared_ptr<Diagnostic> > diags_;
    std::unique_ptr<ExportStrategy> exportStrat_;
    DiagnosticScheduler scheduler_;

public:


    // hard coded for now in the initialization list
    // will have to have a add_diag() function at some point
    // this will come from the factory..
    DiagnosticsManager()
        : diags_{ { DiagType::EM, std::make_shared<ElectromagDiagnostic>()} },
          exportStrat_{ new AsciiExportStrategy{} },
          scheduler_{}
    {

    }


    void newDiagnostic(DiagType type,
                       std::vector<uint32> const& computingIterations,
                       std::vector<uint32> const& writingIterations)
    {
        // create a new diagnostic of type 'type'
        std::shared_ptr<Diagnostic> newDiag = DiagnosticFactory::makeDiagnostic(type);
        diags_.insert( {type, newDiag} );

        // register to the scheduler
        scheduler_.registerDiagnostic(type, computingIterations, writingIterations);
    }



    void compute(Time const& timeManager, PatchData const& patchData)
    {
        for (auto const& diagPair : diags_)
        {
            DiagType type    =  diagPair.first;
            Diagnostic& diag = *diagPair.second;

            if (scheduler_.timeToCompute(timeManager, type))
            {
                diag.compute(patchData);
            }
        }
    }


    void save(Time const& timeManager)
    {
        for (auto const& diagPair : diags_)
        {
            exportStrat_->save(*diagPair.second, timeManager);
        }
    }

    ~DiagnosticsManager() = default;
};








#endif // DIAGNOSTICS_H
